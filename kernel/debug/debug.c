/*
###################################################################
# File Name:debug.c
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年09月14日 星期三 21时48分56秒
# Last Modified2016年09月14日 星期三 21时48分56秒
####################################################################
*/


#include "debug.h"
#include "multiboot.h"
#include "elf.h"


//定义全局静态变量
static elf_t kernel_elf;

//初始化debug
void init_debug()
{
	//因为已经定义了全局的multiboot结构体指针,所以直接提取从全局的multiboot中提取elf信息即可
	//获取elf文件的字符串表和符号表
	kernel_elf=elf_from_multiboot(glb_mboot_ptr);
}


//打印出内核调用栈的调用关系,该关系是从最底层到最高层
//程序在中止的时候，内核栈还是存在的，只需要获得对应的内核栈即可
//程序在中止的时候，ebp是存放的是当前栈顶指针esp的地址，地址指向的是被调用函数执行完毕后的下一条命令
//栈底的地址是高地址，栈顶的地址是低地址，内核栈自低地址向高地址发展
//因为push %ebp的时候使用了4个字节，所以使用Uint32_t
static void print_stack_trace()
{
	//ebp寄存器是32位的
	uint32_t *ebp;
	//用来指代在要返回的指令的位置
	uint32_t *eip;
	//获得中止状态的ebp寄存器的值
	//此时的ebp指向的是调用print_stack_trace()函数的位置
	asm volatile
	{
		"movd %%ebp,%0"
		:"=r"(ebp)
	}
	
	//ebp内存地址不为0，在启动的时候，所有的寄存器要初始化为0，所以中止条件为0,在boot.s中已经声明
	while(ebp)
	{
		//获得被调用函数结束后的下一条指令的函数栈指针地址
		//因为栈顶指向的是数据的下一个内存单元，所以需要先+1
		eip=ebp+1;
		//需要打印出栈顶指针内的地址,*esp取出地址,elf_lookup_symbol提取地址对应的符号
		printf("address:0x%x    %s\n",*esp,elf_lookup_symbol(ebp,&kernel_elf));
		//ebp指向下一个ebp
		ebp=(uint32_t)*ebp;
	}
}

//panic函数，当系统中止是打印出中止时的内核调用栈
void panic(const char *msg)
{
	printf("error message:%s\n",msg);
	printf("------------kernel stack----------------\n");
	print_stack_trace();
	printf("----------------end---------------------\n");
}
