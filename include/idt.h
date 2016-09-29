/*
###################################################################
# File Name:idt.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年09月29日 星期四 02时06分40秒
# Last Modified2016年09月29日 星期四 02时06分40秒
####################################################################
*/

#ifndef INCLUDE_IDT_H_
#define INCLUDE_IDT_H_

#include "types.h"


//定义中断描述符的格式

typedef struct
{
	uint16_t offset_low;//
	uint16_t seg_selector;
	uint16_t extra_info;
	uint16_t offset_high;
}__attribute__((packed)) idt_t;




//定义中段描述符寄存器的格式
typedef struct
{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed)) idtr_t;


//定义初始化中断描述符寄存器的函数

void init_idt();

//定义载入中断描述符寄存器的汇编函数
extern void flush_idt(uint32_t);

#endif
