/*###########################################
# File name:printf.c
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:Wed 22 Jun 2016 05:58:26 AM CST
# Last Modified:Sat 25 Jun 2016 09:37:13 AM CST
##########################################*/


#include "console.h"
#include "common.h"
#include "debug.h"
#include "vargs.h"



void printf(const char *format,...)
{
	//声明变量
	va_list va;
	char *format_temp=format;
	//指向第一个参数
	va_start(va,format);
	//停止参数读取的终点是format
	vsprintf(format,va);
	va_end(va);
}


//带颜色的内核打印函数
void printf_color(real_color_t back,real_color_t fore,const char *foramt,...)
{

}


//获得字符类型
//
static pr_type_t gettype(char *format)
{
	char type=*format;
	switch(type)
	{
		case 'd':return pr_int;break;
		case 'o':return pr_unsigned_int8;break;
		case 'u':return pr_unsigned_int10;break;
		case 'x':
		case 'X':return pr_unsigned_int16;break;
		case 'f':return pr_float;break;
		case 'l':return pr_double;break;
		case 'c':return pr_char;break;
		case 's':
		case 'S':return pr_string;break;
		case 'p':return pr_void;break;
		default:break;
	}
	//不存在该类型的输出
	return false;
}








//输出每一个类型的数据
static void vsprintf(char *format,va_list va)
{
	char *format_temp=format;
	//中止条件是'\0'
	int temp1=0;
	unsigned int temp2=0;
	float temp3=0;
	double temp4=0;
	char temp5=0;
	char *temp6=(char *)0;
	while(format_temp!='\0')
	{
		//不等于%,则直接在屏幕上打印
		if(*format_temp!='%')
		{
			console_putc_color(*format_temp,0,15);
			format_temp++;
		}
		//等于%直接读取下一个字符
		else
		{
			//读取下一个字符
			format_temp++;
			//开始根据条件判断
			switch(gettype(format_temp))
			{
				//int类型
				case pr_int:
					temp1=va_arg(va,int);
					if(temp1<0)
					{
						//先输出一个减号
						console_putc_color('-',0,15);
						//再输出十进制数
						console_write_dec(-temp1,0,15);
					}

					break;
				//usigned int 8进制 以后再写
				case pr_unsigned_int8:
					temp2=va_arg(va,unsigned int);

					break;
				//unsigned int 10进制
				case pr_unsigned_int10:
					temp2=va_arg(va,unsigned int);
					console_write_dec(temp2,0,15);
					break;
				//unsigned int 16进制
				case pr_unsigned_int16:
					temp2=va_arg(va,unsigned int);
					console_write_hex(temp2,0,15);
					break;
				case pr_float:
					/*
					//temp3=va_arg(va,float);
					//先获得符号位,最高位为1,则为负,最低位为0,则为正
					//需要将

					//if((((uint32_t)temp3)>>31)& 0xF==0x1)
					//{
					//	console_putc_color('-',0,15);
					//}
					//非负，则什么都不做，读取下8位为指数位
					//右移15位，获得对应的十六进制数
					//else
					//{
						((temp>>15)& 0x0FF)
					}
					*/
					break;
				case pr_double:
					break;
					//输出单个字符
				case pr_char:
					temp5=va_arg(va,char);
					console_putc_color(temp5,0,15);
					break;
				case pr_string:
					temp5=va_arg(va,char *);
					console_
				case pr_void:

					break;
				default:break;
			}
		}
	}
}













































