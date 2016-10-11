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


//---------------------定义中断描述符表注册框架------------------------
//该部分用于将中断函数加载入cpu中,供中断时查找对应的中断函数在哪里
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
extern void idt_flush(uint32_t);


//------------------------定义中断函数处理框架------------------------
//中断分为三个阶段:
//1.保护现场环境
//2.处理中断函数
//3.返回原来的环境

//1.保护现场环境的过程,CPU会自动压入栈中,其余的需要我们自己保存



//在第三个阶段中需要保护的寄存器情况
typedef struct
{
	
	uint32_t ds;//保存用户的数据段描述符
	//------------------------1.pusha压入栈中的内容-------------------
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	//------------------------1.end-------------------------------
	uint32_t idt_code;//自己压入的
	//-----------由CPU自动保护的寄存器列表和信息列表--------
	uint32_t error_code;//错误代码
	uint32_t eip;//下一条指令
	uint32_t cs;//代码段
	uint32_t eflags;//flag寄存器
	uint32_t user_esp;//用户栈顶指针
	uint32_t user_ss;//用户栈
}reg_t;


//定义一个中断处理函数指针
//传入的参数是保护的所有寄存器的值
typedef void (*interrupt_handler_t)(reg_t *);

//注册一个中断处理函数
//因为中断向量只有256个，所以使用uint8_t
//处理一个中断处理函数,就是将其加入中断函数向量表中
void register_interrupt_handler(uint8_t num,interrupt_handler_t vector);


//调用中断处理函数
void isr_handler(reg_t *regs);

//声明所有的中断处理函数，作为入口地址构建中断向量描述符，并传入中断向量表中
//0~19号中断向量属于CPU的异常中断
void isr0();		//0		#DE 除出错				无错误号
void isr1();		//1		#DB 调试				无错误号
void isr2();		//2			NMI中断				无错误号
void isr3();		//3		#BP	断点				无错误号
void isr4();		//4		#OF 溢出				无错误号
void isr5();		//5		#BR	边界范围超出		无错误号
void isr6();		//6		#UD	无效操作码			无错误号
void isr7();		//7		#NM	设备不存在			无错误号
void isr8();		//8		#DF	双重错误			有错误号
void isr9();		//9			协处理器段超越		无错误号
void isr10();		//10	#TS	无效的任务状态段TSS	有错误号
void isr11();		//11	#NP 段不存				有错误号
void isr12();		//12	#SS 堆栈段错误			有错误号
void isr13();		//13	#GP	一般保护错误		有错误号
void isr14();		//14	#PF 页面错误			有错误号
void isr15();		//15		保留
void isr16();		//16	#MF	FPU浮点错误			无错误号
void isr17();		//17	#AC	对齐检查			有错误号
void isr18();		//18	#MC	机器检查			无错误号
void isr19();		//19	#XF	SIMD浮点异常		无错误号

//20-31 intel自己保留
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

//255
void isr255();
//----------------------------设置中断8259A------------------------
//定义irq初始化函数
void init_irq();
//定义irq处理函数
void irq_handler(reg_t *regs);

//定义32--47中断号
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47


//定义中断处理函数
void irq0();
void irq1();
void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();


#endif
