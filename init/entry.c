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
	/*
    //初始化段式管理
	init_gdt();
	init_debug();
	//初始化中断
	init_idt();	
	console_clear();
	*/
    printf("Welcome to My OS!\n");
	printf("This is only a demo\n");
	printf("The function list is as followings:\n");
	printf("1.print kernel stack!\n");
	printf("2.print GDT setting!\n");
	printf("3.print IDT setting!\n");
	printf("4.print Clock setting!\n");
	printf("5.Memory Management!\n");
	
	/*
#if debug==1	
	panic("this is a kernal stack information!\n");
#endif
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
#endif
#if debug==3
	get_multiboot_structure();
#endif 
	//此处需要中止，调用htl,防止跳出程序
	//初始化虚拟内存管理模块
	init_vmm();
	*/
	while(0)
	{
		asm volatile("htl");
	}
}

//--------------------以下部分是kern_entry----------------------------
//该部分的功能是在grub载入内核时建立临时的页表，开启虚拟内存映射,以保证VMA能够被正确使用
//目标是：将kernel映射到0xc0000000位置上，即3GB-4GB这个虚拟内存空间中
//需要建立以下内容
//1.临时的页目录，页表
//2.开启分页模式
//3.切换新的地址
//4.新建内核栈



//全局的multiboot头声明，其他文件使用需要使用extern关键字
multiboot_t * glb_mboot_ptr;
extern multiboot_t *mboot_ptr_tmp;
//开启分页机制后的内核栈
//栈大小在pmm.h中声明


//声明内核栈段，供以后使用,栈的起始位置是随机的
uint8_t kern_stack[STACK_SIZE];


//建立临时的页目录和页表
//该地址需要4KB对齐，所以使用0-640K的内存空间建立临时页表
//需要将如下变量声明为.init.data段
//页目录项只有一页
//将页目录项声明为数据,该数组起始地址使用物理地址0x1000
//pde_t为4字节，需要存储1024个页表项
__attribute__((section(".init.data"))) pgd_t *temp_pgd=(pgd_t *)0x1000;
//声明第一个页表,页表为4kb
__attribute__((section(".init.data"))) pgd_t *low_pgd=(pgd_t *)0x2000;
//声明第二个页表,页表为4KB
__attribute__((section(".init.data"))) pgd_t *high_pgd=(pgd_t *)0x3000;
//只使用一个临时页表的原因为默认核心部的init部分不会大于4M


//声明全局的kernel入口函数
//该函数位于.init.text段
__attribute__((section(".init.text"))) void kern_entry()
{
	//------------------设置页表项-------------------------
	//页目录项里需要存储两个页表low_pgd和high_pgd
	//low里存储的是0x100000,化为二进制为0000,0000,0001,0000,0000,0000,0000,0000
	//所以前十位为0000,0000,00，故为页表项的第0项,将low页表的地址存储如pgd中
	temp_pgd[0]=(uint32_t)low_pgd|PAGE_PRESENT|PAGE_WRITE;
	//high里存储的是PAGE_OFFSET，回味二进制为1100,0,0,0,0,0,0,0
	//所以前十位为11，0000，0000，故为页表项的第0x300项，将high页表的地址存储如pgd中
	//特权级:P位有效，R/W位有效，U/S位无效
	temp_pgd[PGD_INDEX(PAGE_OFFSET)]=(uint32_t)high_pgd|PAGE_PRESENT|PAGE_WRITE;

	//------------------设置页表内容------------------------
	//页表内容代表的是实际的物理页地址
	//一个页表可以映射4MB的物理地址空间
	//将虚拟地址空间中的0x00000000----0x00400000映射为物理地址的前4M
	//将虚拟地址空间中的0xc0000000----0xc0400000映射为物理地址的前4M
	//页表中存储的是物理地址页
	uint32_t i=0;
	for(i=0;i<1024;i++)
	{
		//low
		low_pgd[i]=(uint32_t)(i*PMM_PAGE_SIZE)|PAGE_PRESENT|PAGE_WRITE;
		//high
		high_pgd[i]=(uint32_t)(i*PMM_PAGE_SIZE)|PAGE_PRESENT|PAGE_WRITE;
	}
	//------------------开启分页模式------------------------
	//1.将页目录地址放到cr3中
	asm volatile
		( 
			"movl %0,%%cr3"
			:
			:"r" (temp_pgd)
		);
	//2.将cr0的PG位设置为1
	uint32_t cr0;
	asm volatile
		(
			"movl %%cr0,%0"
			:"=r"(cr0)
			:
		);
	cr0=cr0|0x80000000;
	asm volatile
		(
			"movl %0,%%cr0"
			:
			:"r"(cr0)
		);
	//----------------从此处开始寻址变为分页------------------
	//开始启用新的地址
	//设置全局的multiboot指针
	glb_mboot_ptr=(multiboot_t *)((uint32_t)mboot_ptr_tmp+PAGE_OFFSET);
	//----------------开始切换新的内核栈-------------------
	//新的内核栈，需要修改的是栈顶指针esp的位置
	//首先确定esp的地址，需要4字节对齐
	uint32_t kern_stack_top=(uint32_t)(kern_stack+STACK_SIZE)& 0xfffffff0;
	//需要修改ebp和esp
	asm volatile
		(
			"movl %0,%%esp\n"
			"xor %%ebp,%%ebp\n"
			:
			:"r"(kern_stack_top)
		);
	
	//--------------跳转到内核处理模块-------------------
	init_kern();
	
}
