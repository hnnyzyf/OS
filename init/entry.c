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
int kern_entry()
{
	console_clear();
    printf("Welcome to My OS!\n");
	printf("This is only a demo\n");
	printf("The function list is as followings:\n");
	printf("1.print kernel stack!\n");
	printf("2.print GDT setting!\n");
	printf("3.print IDT setting!\n");
	printf("4.print Clock setting!\n");
	//panic("this is a kernal stack information!\n");
	init_gdt();
	init_debug();
	init_idt();
	init_timer(60);
	asm volatile("sti");
	show_memory_map();
	//get_multiboot_structure();
	return 0;
}
