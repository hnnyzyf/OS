//###################################################################
//# File Name:entry.c
//# Author:YFzhang
//# mail:hnnyzyf2013@163.com
//# Created Time:2016年06月13日 星期一 12时03分37秒
//# Last Modified2016年06月13日 星期一 12时03分37秒
//####################################################################
#include "types.h"
#include "console.h"
#include "common.h"
int kern_entry()
{
	
	console_clear();
	char *str="hello world!";
	console_write(str);
	return 0;
}
