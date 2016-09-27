/*
###################################################################
# File Name:gdt.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年09月27日 星期二 22时29分22秒
# Last Modified2016年09月27日 星期二 22时29分22秒
####################################################################
*/

#ifndef INCLUDE_GDT_H_
#define INCLUDE_GDT_H_

#include "types.h"

//定义段描述符
typedef struct
{
	uint8_t base_high;//base address 的31-24位
	uint16_t extra_info;//存储控制信息，19-16存储的是limit的位
	uint8_t base_mid;//存储base address的23-16位
	uint16_t base_low;//存储base address的16-0位
	uint16_t limit_low;//存储limit的低16位
}__attribute__((packed)) gdt_t;

//定义GDTR的结构 


typedef struct
{
	uint32_t gdt_base;
	uint16_t gdt_limit;
}__attribute__((packed)) gdtr_t;



//初始化全局描述符
void init_gdt();


//GDT加载实现
extern void gdt_flush(uint32_t);

#endif //INCLUDE_GDT_H_
