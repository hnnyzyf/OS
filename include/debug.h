/*###########################################
# File Name:debug.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:Tue 21 Jun 2016 11:57:17 PM CST
# Last ModifiedTue 21 Jun 2016 11:57:17 PM CST
###########################################*/

#include "console.h"
#include "vargs.h"


//内核打印函数
//函数流程
void printf(const char *format,...);

//带颜色的内核打印函数
void printf_color(real_color_t back,real_color_t fore,const char *format,...);


