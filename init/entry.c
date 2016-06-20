//###################################################################
//# File Name:entry.c
//# Author:YFzhang
//# mail:hnnyzyf2013@163.com
//# Created Time:2016年06月13日 星期一 12时03分37秒
//# Last Modified2016年06月13日 星期一 12时03分37秒
//####################################################################
#include "types.h"
int kern_entry()
{
	uint8_t *input=(uint8_t *)0xB8000;
	uint8_t color=(0<<4)|(15&0x0F);

	*input++='H';*input++=color;
	*input++='H';*input++=color;
	*input++='H';*input++=color;
	*input++='H';*input++=color;
	*input++='H';*input++=color;
	*input++='H';*input++=color;
	*input++='H';*input++=color;
	*input++='H';*input++=color;
	*input++='H';*input++=color;
	return 0;
}
