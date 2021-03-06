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

#define debug 0


//定义全局静态变量
static elf_t kernel_elf;

//初始化debug
void init_debug()
{
	//因为已经定义了全局的multiboot结构体指针,所以直接提取从全局的multiboot中提取elf信息即可
	//获取elf文件的字符串表和符号表
	kernel_elf=elf_from_multiboot(glb_mboot_ptr);
#if (debug==1)
	printf("the address of kernel file is %x\n",&kernel_elf);
#endif
}


//打印出内核调用栈的调用关系,该关系是从最底层到最高层
//程序在中止的时候，内核栈还是存在的，只需要获得对应的内核栈即可
//函数调用的时候,分三个步骤
//将下一条指令的地址压栈，由call完成
//将ebp存入栈中
//将当前栈顶esp存入ebp中
//所以esp+4+4即为外层函数的下一条指令的地址，则我们可以相爱符号表中查到对应的函数名称，获得函数调用栈
//
static void print_stack_trace()
{
	//ebp寄存器是32位的，声明32位int型指针
	uint32_t *ebp;
	//用来指代在要返回的指令的位置
	uint32_t *esp;
	//获得中止状态的ebp寄存器的值
	//该函数返回的是一个地址，将其赋值给指针ebp
	asm volatile("movl %%ebp,%0":"=r"(ebp));
	
	//ebp内存地址不为0，在启动的时候，所有的寄存器要初始化为0，所以中止条件为0,在boot.s中已经声明
	while(ebp)
	{
		//获得call压入栈中的地址的位置
		//int指针+1即esp+4
		//示意图
		//h--------------------------------esp
		//
		//此处存储的是外层函数的下一条指令的地址
		//
		//m--------------------------------esp1
		//
		//此处存储的是压入栈中的ebp的内容，该ebp内存储的是外层函数指向的esp2的地址
		//
		//l--------------------------------esp2，此时该地址在ebp中存储
		//*esp2取的是l-m之间的数据
		//*esp1取的是m-h之间的数据
		//当前ebp指向esp2，所以想获得call压入栈中的下一个指令的地址，只需要ebp+1
		esp=ebp+1;
		//esp取出esp中存储的地址，即call压入栈中的下一条指令的地址
		//elf_lookup_symbol根据下一条指令的地址来提取符号符号文件中的符号
		printf("address:%x:",*esp);
		printf("%s\n",elf_lookup_symbol(*esp,&kernel_elf));
		//当前函数中ebp存储的是esp，而*esp指向的内存单元中存储的是调用函数起始push进去的ebp
		//所以新的ebp就是当前ebp存储的内容
		ebp=(uint32_t *)*ebp;
	}
}

//panic函数，当系统中止是打印出中止时的内核调用栈
void panic(const char *msg)
{
	//初始化elf文件信息
	printf("----------step 1:initialization---------\n");
	printf("  Begin to init the elf file information\n");
	printf("The elf file has been init\n");
	printf("------step 1:initialiazation end--------\n");
	printf("----------step 2:kernel stack-----------\n");
	printf("error message:%s\n",msg);
	print_stack_trace();
	printf("-------------step 2:end-----------------\n");
}
