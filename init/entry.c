//###################################################################
//# File Name:entry.c
//# Author:YFzhang
//# mail:hnnyzyf2013@163.com
//# Created Time:2016年06月13日 星期一 12时03分37秒
//# Last Modified2016年06月13日 星期一 12时03分37秒
//####################################################################
#include "console.h"
#include "debug.h"
#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"
#include "vmm.h"

#define debug 0

//用于初始化操作系统的如下内容
//初始化段寄存器GDT
//初始化中断寄存器IDT
//初始化中断控制器和时钟timer
//初始化物理内存管理PMM
//初始化虚拟内存管理VMM，开启分页管理模式
void init_kern()
{
	console_clear();
    printf("Welcome to My OS!\n");
	printf("This is only a demo\n");
	printf("The function list is as followings:\n");
	printf("1.print kernel stack!\n");
	printf("2.print GDT setting!\n");
	printf("3.print IDT setting!\n");
	printf("4.print Clock setting!\n");
	printf("5.Memory Management!\n");
#if debug==1	
	panic("this is a kernal stack information!\n");
#endif
	//初始化段式管理
	init_gdt();
	init_debug();
	//初始化中断
	init_idt();
	//初始化可编程中断，初始化时钟
	init_timer(60);
	//因为初始化过程中关闭了中断，需要开中断
	asm volatile("sti");
	//初始化物理内存管理模块
	init_pmm();
#if debug==2
	uint32_t page=pmm_alloc_page();
	printf("the page address is %x\n",page);
	pmm_free_page(page);
	show_memory_map();
	show_kernel_map();
#if debug==3
	get_multiboot_structure();
#endif 
	//此处需要中止，调用htl,防止跳出程序
	while(0)
	{
		asm volatile("htl");
	}
}

//--------------------以下部分是kern_entry----------------------------
//该部分的功能是在grub载入内核时建立临时的页表，开启虚拟内存映射
//目标是：将kernel映射到0xc0000000位置上，即3GB-4GB这个虚拟内存空间中
//需要建立以下内容
//1.临时的页目录，页表
//2.开启分页模式
//3.切换新的地址
//4.新建内核栈



//全局的multiboot头声明，其他文件使用需要使用extern关键字
multiboot_t * glb_mboot_ptr;
//开启分页机制后的内核栈
//栈大小在pmm.h中声明
uint8_t kern_stack[STACK_SIZE];

//建立临时的页目录和页表
//该地址需要4KB对齐，所以使用0-640K的内存空间建立临时页表
//需要将如下变量声明为.init.data段
//页目录项只有一页
//将页目录项声明为数据,该数组起始地址使用物理地址0x1000
__attribute__((section(".init.data"))) pde_t *temp_pgd=(pde_t *)0x1000;
//声明页表,每个页表负责4MB
__attribute__((section(".init.data"))) pgd_t 
