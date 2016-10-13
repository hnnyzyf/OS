/*###################################################################
# File Name:timer.c
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年10月13日 星期四 21时40分19秒
# Last Modified2016年10月13日 星期四 21时40分19秒
####################################################################*/

#include "timer.h"
#include "idt.h"
#include "debug.h"
#include "types.h"
#include "common.h"



static uint32_t tick=0;

//声明中断处理函数
void timer_callback(reg_t* regs)
{
	//判断中断次数是否正常
	if(tick<0xffffffff)
	{
		printf("Tick:%d\n",tick++);
	}
	else
	{
		printf("Tick:%d\n",tick);
		tick=0;
	}
}


//声明中断初始化函数
void init_timer(uint32_t frequency)
{
	//首先注册中断处理函数
	register_interrupt_handler(IRQ0,timer_callback);

	//----------------------设置芯片
	//端口处理范围是040h～043h
	//输入的频率为1193180   frequency为每秒中断的次数
	uint32_t interval=1193180/frequency;

	//D7 D6 D5 D4 D3 D2 D1 D0
	//0	 0  1  1  0  1  1  0
	//设置工作在模式3下
	outb(0x43,0x36);

	//查分低字节和高字节
	uint8_t low=(uint8_t)(interval& 0xff);
	uint8_t high=(uint8_t)((interval>>8)& 0xff);

	//分别写入低字节和高字节
	outb(0x40,low);
	outb(0x40,high);
}

//
