/*###################################################################
# File Name:timer.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年10月13日 星期四 21时44分39秒
# Last Modified2016年10月13日 星期四 21时44分39秒
####################################################################*/


#ifndef INCLUDE_TIMER_H_
#define INCLUDE_TIMER_H_

#include "types.h"
#include "idt.h"
//声明时钟中断初始化函数

void init_timer(uint32_t frequency);

void timer_callback(reg_t* regs);

#endif
