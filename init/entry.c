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
int kern_entry()
{
	console_clear();
	init_gdt();
	init_debug();
	init_idt();
	printf("Welcome to My OS!\n");
	printf("This is only a demo\n");
	printf("The function list is as followings:\n");
	printf("1.print kernel stack!\n");
	printf("2.print GDT setting!\n");
	printf("3.print IDT setting!\n");
	//panic("this is a kernal stack information!\n");
	//get_multiboot_structure();
	asm volatile("int $0x3");
	return 0;
}
