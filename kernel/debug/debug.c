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
//程序在中止的时候，ebp是存放的是栈顶指针sp的地址，地址内存储的是外层函数的调用的下一条命令的地址,因为内核栈是自下而上的
static void print_stack_trace()
{
	//ebp寄存器是32位的
	uint32_t *ebp;
	//获得中止状态的ebp寄存器的值
	asm volatile
	{
		"movd %%ebp,%0"
		:"=r"(ebp)
	}

	//ebp内存地址不为0，在启动的时候，所有的寄存器要初始化为0
	while(ebp)
	{

	}
}
