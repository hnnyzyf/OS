/*###########################################
# File Name:debug.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:Tue 21 Jun 2016 11:57:17 PM CST
# Last ModifiedTue 21 Jun 2016 11:57:17 PM CST
###########################################*/

#include "console.h"
#include "vargs.h"
#include "elf.h"


//定义assert宏
//该断言在判断x为'0',空指针等，会调用panic函数
#define assert(x,info)				\
		do							\
		{						    \
			if(!(x))				\
					{				\
					panic(info)		\
					}				\
		}							\
		while(0)

//编译期间静态检测
//#define static_assert(x) switch(x)(case 0: case(x):;)



//内核打印函数
//函数流程
void printf(const char *format,...);

//带颜色的内核打印函数
void printf_color(real_color_t back,real_color_t fore,const char *format,...);


//初始化debug信息
void init_debug();


//打印当前的函数调用栈
void panic(const char *msg);

//打印当前的段寄存器值
void print_register_info();
