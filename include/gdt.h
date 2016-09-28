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



//因为在内存中存储的时候,有高位和地位的划分
//intel-i386采用的是liitle endian的方式存储的	
//
//0-----------15	16------------31  
//	limit_low（）		base_low
//
//
//0-----------7		8-------------23	24-----------31
//	base_mid			extra_info			base_high
//
//定义段描述符
//注意按照顺序进行设计结构体的形式
typedef struct
{
	uint16_t limit_low;//limit
	uint16_t base_low;//base
	uint8_t	base_mid;//base
	uint16_t extra_info;//extra
	uint8_t base_high;//base
}__attribute__((packed)) gdt_t;

//定义GDTR的结构 


typedef struct
{
	uint16_t gdt_limit;
	uint32_t gdt_base;
}__attribute__((packed)) gdtr_t;



//初始化全局描述符
void init_gdt();


//GDT加载实现
extern void gdt_flush(uint32_t);

#endif //INCLUDE_GDT_H_
