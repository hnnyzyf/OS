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





//函数流程
//声明静态变量buff------>根据每一个参数写入buff------->将buff视为字符串进行输出
void printf(const char *format,...)
{
	//声明变量
	//声明一个静态变量，生存周期是在该函数内
	static char buff[2048];
	va_list va;
	char *format_temp=format;
	//指向第一个参数
	va_start(va,format);
	//停止参数读取的终点是format
	//将每一个参数写入buff中
	vsprintf(buff,format,va);
	va_end(va);
	console_write(buff);
}


//带颜色的内核打印函数
void printf_color(real_color_t back,real_color_t fore,const char *foramt,...)
{

}







//输出每一个类型的数据
//vsprintf只用于本文件内，所以定义为静态
//该函数的流程如下
//读取每一个字

//一下宏定义的目的是为了判断输出的格式
//定义为2进制，每一位表示是否存在对应的输出格式
//二进制表示1111
//第一位表示是否有左右对齐
#define left		 1;  //左对齐1，右对齐0，默认左对齐
//#define Hashtag		 8;//根据需要输出#对应的情况，1为需要，0为不需要
#define width		2;//整个输出需要的宽度，1为存在，0为不存在
#define dot			 4;//根据精度输出固定的长度,1为存在，0为不存在


//该函数用来将内容赋予buff
static void content(char *buff,unsigned char data_format,pr_type_t type,int word_width,int precision,va_list va)
{
	//用来记录每一个数字位的数据
	char digit[16]="0123456789ABCDEF";
	char temp[32]
	int temp1;
	//如果是有符号数，默认需要判断类型，是否输出-号
	if(type==pr_int)
	{
		temp1=va_va_arg(va,int);

	}
	

}





//10进制获得对应n进制每一位的数据





static void vsprintf(char *buff,char *format,va_list va)
{
	char * buff_temp=buff;
	//记录输出字符的宽度,默认为0
	int word_width=0;
	//记录输出字符的精度,-1表示默认精度
	int precision=-1;
	//记录要输出的数据的格式
	unsigned char int data_format=0;
	//中止的条件,是format字符串到达末尾
	while(*format!='\0')
	{
		//首先判断是不是%号，不是的话则将其放入到buff中
		if(*format!='%')
		{
			*buff_temp++=*format;
			format++;
		}
		//如果是%号的话,则开始读取下一个字符
		else
		{
			format++;
			//在指定地方跳出循环
			
			//%后的判断步骤
			//1.首先判断格式控制符 -
			//2.然后判断-后的格式是1.1还是5的格式
			//3.然后判断
			//开始判断下一个字符是什么
			
			//---------------------------------------------输出格式部分判断------------------------------
			//存在对齐符号
			if(*format=='-')
			{
				data_format=data_format+left;
				//移动到下一个字符字符
				format++;
			}
			//如果存在数字，需要设置宽度
			if(*format>='0'&& *format<='9')
			{
				word_width=*format;
				data_format=data_format+width;
				format++;
			}
			//设置完宽度，需要看下一位的精度
			if(*format=='.')
			{
				data_format=data_format+dot;
				format++;
				//获得小数的精度信息
				precision=*format;
				format++;
			}
			//---------------------------------------------输出类型部分判断------------------------------
			switch(*format)
			{
				//输出int类型的十进制数
				case 'd':
				case 'i':
					content(buff_temp,data_foramt,pr_int,word_width,precision,va);
					break;
				//输出无符号的8进制数
				case 'o':
					break;
				//输出无符号的十六进制数
				case 'x':
				case 'X':
					break;
				//输出无符号的十进制数
				case 'u':
					break;
				//输出单个字符
				case 'c':
					break;
				//输出字符串
				case 's':
				case 'S':
					break;
				//双精度类型，默认为6
				case 'f':
					break;

			}

















		}

		}
	}
}










































