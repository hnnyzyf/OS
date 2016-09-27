/*
###################################################################
# File name:gdt.c
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年09月27日 星期二 23时36分43秒
# Last Modified:2016年09月27日 星期二 23时36分44秒
####################################################################
*/


#include "gdt.h"
#include "types.h"

//需要声明如下段描述符
#define GDT_LENGTH 5

//声明全局的GDTR
gdt_t gdt[GDT_LENGTH];
gdtr_t gdtr;



//该函数用于设定一个描述符的状态
static void set_gdt_status(gdt_t *gdt,uint32_t base,uint32_t limit,uint16_t info)
{
	//--------------------------设置base地址---------------------
	//设置高8位地址
	gdt->base_high=(base>>24)& 0xff;
	//设置低16位地址
	gdt->base_low=base& 0xffff;
	//设置中间的8位地址
	gdt->base_mid=(base>>16)& 0xff;
	//--------------------------设置limit低16位地址----------------------
	//设置低16位
	gdt->limit_low=(limit& 0xffff);
	//设置额外信息和limit高4位的地址
	gdt->extra_info=info|(((limit>>8)& 0x0f00));
}

//初始化gdt描述符
//因为使用flag model，所以
void init_gdt()
{
	//------------------------初始化段描述符GDTR的结构--------------------
	//base存储的是GDT的基地址
	gdtr.gdt_base=(uint32_t)&gdt[0];
	//limit为总字节数减1
	gdtr.gdt_limit=sizeof(gdt_t)*GDT_LENGTH-1;
	//------------------------初始化gdt的值-------------------------------
	//根据intel带保护模式的平坦模型要求
	//第一个为0
	set_gdt_status(&gdt[0],0,0,0);
	//内核数据段描述符和代码段描述符
	//base:0  limit:4GB DPL:0
	//代码段 
	//	15		G=1:表示limit的范围到4G
	//	14		D=1 默认值是32位的地址和32位或者8位的操作数
	//	13		0：表示为代码段
	//	12		AVL：设为0
	//	8-11	段限长保留
	//	7		p段是否在内存中:1
	//	5-6		DPL:00
	//
	//	4		S=0:表示是代码段或者数据段
	//	0-3		Type:A创建描述符时清0:0
	//				R:1 表示代码段可读可执行
	//				C:0,数据段全部用0
	//				1:1
	//代码段的额外信息描述符为1100,0000,1001,1010,即0xc09a;
	set_gdt_status(&gdt[1],0,0xffffffff,0xc09a);//内核代码段
	//数据段的额外信息描述符为1100,0000,1001,0010.即0xc092;
	set_gdt_status(&gdt[2],0,0xffffffff,0xc092);//内核数据段
	//用户数据段描述符和代码段描述符
	//base:0 limit:4GB DPL:3
	//代码段的额外信息描述符为1100,0000,1111,1010
	set_gdt_status(&gdt[3],0,0xffffffff,0xc0fa);//用户代码段
	//数据段的额外信息描述符为1100,0000,1111,0010
	set_gdt_status(&gdt[4],0,0xffffffff,0xc0f2);//用户数据段

	//加载全局描述符表到GPDT寄存器
	gdt_flush((uint32_t)&gdt[0]);
}

