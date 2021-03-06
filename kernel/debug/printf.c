/*###########################################
# File name:printf.c
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:Wed 22 Jun 2016 05:58:26 AM CST
# Last Modified:2016年09月07日 星期三 18时10分20秒
##########################################*/


#include "console.h"
#include "debug.h"
#include "vargs.h"
#include "types.h"

//定义一个location和va的structure
typedef struct 
{
	va_list va;
	int location;
}result;


//带颜色的内核打印函数
void printf_color(real_color_t back,real_color_t fore,const char *foramt,...)
{

}

//int根据不同进制转字符串
static int itoc(char *buff,uint32_t src,int radix,char word[],int location)
{
	//字符串原地逆序
	int start=location+1;
	int end=start;
	//src标准化
	if(src<0)
	{
		buff[++location]='-';
		src=0-src;
		start++;
	}
	else
	{
		src=src;
	}
	//十六进制加0x
	if(radix==16)
	{
		buff[++location]='0';
		buff[++location]='X';
		start=start+2;
	}
	if(src==0)
	{
		buff[++location]='0';
	}
	//记录起始位置
	while(src!=0)
	{
		//将余数插入到buff中
		buff[++location]=word[src%radix];
		src=src/radix;
	}
	//原地逆序
	//如果是十六进制
	if(radix==16)
	{
		while(location-start+1<8)
		{
			buff[++location]='0';
		}
	}
	end=location;
	//中止条件是start=end或者start>end
	while(start<end)
	{
		buff[location+1]=buff[start];
		buff[start++]=buff[end];
		buff[end--]=buff[location+1];
	}
	return location;
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
//10进制获得对应n进制每一位的数据
//该函数是处理format，根据format来获得要输出几个参数，输出的样式，以及类型

static void vsprintf(char *buff,const char *format,va_list va)
{
	//记录输出字符的宽度,-1表示宽度未定义
	int word_width=-1;
	//记录输出字符的精度,-1表示精度未定义
	int precision=-1;
	//记录要输出的数据的格式
	unsigned char data_format=0;
	//记录buff中最后的一个字符所在位置
	int location=-1;
	//字符串
	char word[]="0123456789abcdef";
	char *temp=NULL;
	//中止的条件,是format字符串到达末尾
	while(*format!='\0')
	{
		//首先判断是不是%号，不是的话则将其放入到buff中
		if(*format!='%')
		{
			buff[++location]=*format++;
			
			//#if PRINTF_MARCO==1
			//	printf("%c+++1\n",buff[location]);
			//#endif
		
		}
		//如果是%号的话,则开始读取下一个字符
		else
		{
			//指向下一个字符
			format++;
			//---------------------------------------------输出格式部分判断------------------------------
			//---------------------------------------------输出类型部分判断------------------------------
			//格式化
			switch(*format)
			{
				//整形
				case 'd':
				location=itoc(buff,va_arg(va,int),10,word,location);
				break;
				//二进制
				case 'b':
				case 'B':
				location=itoc(buff,va_arg(va,int),2,word,location);
				break;
				//十六进制
				case 'x':
				case 'X':
				location=itoc(buff,va_arg(va,uint32_t),16,word,location);
				break;
				//字符
				case  'c':
				buff[++location]=va_arg(va,int);
				break;
				//字符串
				case 's':
				temp=va_arg(va,char *);
				while(*temp!='\0')
				{
					buff[++location]=*temp;
					temp++;
				}
				break;
				default:break;
			}
			format++;
		}
	}
	//字符串终结
	buff[++location]='\0';
	//printf("%s\n",buff);
}

//函数流程
//声明静态变量buff------>根据每一个参数写入buff------->将buff视为字符串进行输出
void printf(const char *format,...)
{
	//声明变量
	//声明一个静态变量，生存周期是在该函数内
	static char buff[2048];
	va_list va;
	//指向第一个参数
	va_start(va,format);
	//停止参数读取的终点是format
	//将每一个参数写入buff中
	vsprintf(buff,format,va);
	va_end(va);
	console_write(buff);
}























