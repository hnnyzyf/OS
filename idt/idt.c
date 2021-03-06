/*
###################################################################
# File Name:idt.c
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年09月30日 星期五 00时12分24秒
# Last Modified2016年09月30日 星期五 00时12分24秒
####################################################################
*/

#include "idt.h"
#include "debug.h"
#include "string.h"
#include "common.h"

#define debug 0

//定义全局的中断描述符表
//最多有255个中断向量
#define IDT_LENGTH 256

//定义全局的中断描述符表
idt_t idt[IDT_LENGTH];
//定义全局的中断描述符表寄存器
idtr_t idtr;

//定义全局的中断处理函数数组
interrupt_handler_t handler[IDT_LENGTH];


//定义中断描述符表函数
static void set_idt_status(idt_t *idt,uint32_t offset,uint16_t seg_selector,uint16_t extra_info)
{
	idt->offset_low=offset& 0xffff;
	idt->offset_high=(offset>>16)& 0xffff;
	idt->seg_selector=seg_selector;
	idt->extra_info=extra_info;
#if debug==1
	printf("%x:%x:%x:%x\n",idt->offset_low,idt->offset_high,idt->extra_info);
#endif
}


//注册中断处理函数
//输入
//num:第几号中断描述符
//vector：中断函数的地址
void register_interrupt_handler(uint8_t num,interrupt_handler_t vector)
{
	//将已经定义好的中断处理函数注册
	//只需要加入到中断处理函数指针数组中即可
	handler[num]=vector;
}


//调用中断处理函数
void isr_handler(reg_t *regs)
{
	//先判断中断向量已经注册
	if(handler[regs->idt_code])
	{
		//调用中断处理函数
		handler[regs->idt_code](regs);
	}
	//未注册
	else
	{
		printf("The %d interrupt is not registered!\n",regs->idt_code);
	}
}

//----------------------------8259A PIC初始化---------------------
//初始化的过程需要是4个部分
//中断请求方式的设定
//中断类型号的设定
//中断级联方式的设定
//特定完全嵌套方式
//PIC对应端口号
//
void init_irq()
{
	//---------------初始化ICW1
	outb(0x20,0x11);
	outb(0xA0,0x11);
	//---------------初始化ICW2
	outb(0x21,0x20);		//设置主片从0x20开始
	outb(0xA1,0x28);		//设置从片从0x28开始
	//--------------初始化ICW3
	outb(0x21,0x04);		//设置竹片IR2引脚连接从片
	outb(0xA1,0x02);		//设置从偏输出已交和主片IR2连接
	//设置主从片按照8086方式工作
	outb(0x21,0x01);
	outb(0xA1,0x01);
	//--------------初始化ICW4
	//设置主从片允许中断
	outb(0x21,0x0);
	outb(0xA1,0x0);
	set_idt_status(&idt[32],(uint32_t)irq0,0x08,0x8e00);
	set_idt_status(&idt[33],(uint32_t)irq1,0x08,0x8e00);
	set_idt_status(&idt[34],(uint32_t)irq2,0x08,0x8e00);
	set_idt_status(&idt[35],(uint32_t)irq3,0x08,0x8e00);
	set_idt_status(&idt[36],(uint32_t)irq4,0x08,0x8e00);
	set_idt_status(&idt[37],(uint32_t)irq5,0x08,0x8e00);
	set_idt_status(&idt[38],(uint32_t)irq6,0x08,0x8e00);
	set_idt_status(&idt[39],(uint32_t)irq7,0x08,0x8e00);
	set_idt_status(&idt[40],(uint32_t)irq8,0x08,0x8e00);
	set_idt_status(&idt[41],(uint32_t)irq9,0x08,0x8e00);
	set_idt_status(&idt[42],(uint32_t)irq10,0x08,0x8e00);
	set_idt_status(&idt[43],(uint32_t)irq11,0x08,0x8e00);
	set_idt_status(&idt[44],(uint32_t)irq12,0x08,0x8e00);
	set_idt_status(&idt[45],(uint32_t)irq13,0x08,0x8e00);
	set_idt_status(&idt[46],(uint32_t)irq14,0x08,0x8e00);
	set_idt_status(&idt[47],(uint32_t)irq15,0x08,0x8e00);	
}


//irq_handller设置

void irq_handler(reg_t *regs)
{
	//发送中断信号给芯片
	//大于40的信号由从片发送
	if(regs->idt_code>=40)
	{
		outb(0xA0,0x20);
	}
	//发送重设信号给主片
	outb(0x20,0x20);
	//调用中断处理函数
	isr_handler(regs);
}






//----------------------------------全局初始化
//定义初始化中断描述符表
void init_idt()
{
	//-------------初始化中断描述符表数组和中断处理函数数组--------
	//初始化中断描述符表,全部置为0
	//因为不确定用哪些
	memset(&idt,0,sizeof(idt_t)*256);
	//初始化全局的中断处理函数数组
	memset(&handler,0,sizeof(interrupt_handler_t)*256);
	//首先设置中断描述符表寄存器
	idtr.limit=sizeof(idt_t)*IDT_LENGTH-1;
	idtr.base=(uint32_t)&idt[0];
	//-----------------设置中断描述符---------------------
	//seg_selector默认选择内核代码段(GDT表中的第二个)0x08
	//因为中断只能操作系统执行
	//P=1
	//DPL=00
	//0D1100000000
	//D=1
	//故extra info=1000,1110,0000,0000
	//设置0-19号中断描述符
	set_idt_status(&idt[0],(uint32_t)isr0,0x08,0x8e00);
	set_idt_status(&idt[1],(uint32_t)isr1,0x08,0x8e00);
	set_idt_status(&idt[2],(uint32_t)isr2,0x08,0x8e00);
	set_idt_status(&idt[3],(uint32_t)isr3,0x08,0x8e00);
	set_idt_status(&idt[4],(uint32_t)isr4,0x08,0x8e00);
	set_idt_status(&idt[5],(uint32_t)isr5,0x08,0x8e00);
	set_idt_status(&idt[6],(uint32_t)isr6,0x08,0x8e00);
	set_idt_status(&idt[7],(uint32_t)isr7,0x08,0x8e00);
	set_idt_status(&idt[8],(uint32_t)isr8,0x08,0x8e00);
	set_idt_status(&idt[9],(uint32_t)isr9,0x08,0x8e00);
	set_idt_status(&idt[10],(uint32_t)isr10,0x08,0x8e00);
	set_idt_status(&idt[11],(uint32_t)isr11,0x08,0x8e00);
	set_idt_status(&idt[12],(uint32_t)isr12,0x08,0x8e00);
	set_idt_status(&idt[13],(uint32_t)isr13,0x08,0x8e00);
	set_idt_status(&idt[14],(uint32_t)isr14,0x08,0x8e00);
	set_idt_status(&idt[15],(uint32_t)isr15,0x08,0x8e00);
	set_idt_status(&idt[16],(uint32_t)isr16,0x08,0x8e00);
	set_idt_status(&idt[17],(uint32_t)isr17,0x08,0x8e00);
	set_idt_status(&idt[18],(uint32_t)isr18,0x08,0x8e00);
	set_idt_status(&idt[19],(uint32_t)isr19,0x08,0x8e00);
	//设置20-31 intel保留中断描述符
	set_idt_status(&idt[20],(uint32_t)isr20,0x08,0x8e00);
	set_idt_status(&idt[21],(uint32_t)isr21,0x08,0x8e00);
	set_idt_status(&idt[22],(uint32_t)isr22,0x08,0x8e00);
	set_idt_status(&idt[23],(uint32_t)isr23,0x08,0x8e00);
	set_idt_status(&idt[24],(uint32_t)isr24,0x08,0x8e00);
	set_idt_status(&idt[25],(uint32_t)isr25,0x08,0x8e00);
	set_idt_status(&idt[26],(uint32_t)isr26,0x08,0x8e00);
	set_idt_status(&idt[27],(uint32_t)isr27,0x08,0x8e00);
	set_idt_status(&idt[28],(uint32_t)isr28,0x08,0x8e00);
	set_idt_status(&idt[29],(uint32_t)isr29,0x08,0x8e00);
	set_idt_status(&idt[30],(uint32_t)isr30,0x08,0x8e00);
	set_idt_status(&idt[31],(uint32_t)isr31,0x08,0x8e00);
	//设置操作系统自定义中断描述符
	set_idt_status(&idt[255],(uint32_t)isr255,0x08,0x8e00);
	//初始化
	init_irq();
	
	//-------------加载中断描述符表寄存器-----------------
	idt_flush((uint32_t)&idtr);
}

