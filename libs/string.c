/*###########################################
# File name:string.c
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:Tue 21 Jun 2016 07:25:17 PM CST
# Last Modified:Tue 21 Jun 2016 10:49:34 PM CST
##########################################*/


#include "string.h"


//memcpy函数的实现
//输入是地址 dest代表目标地址  src代表源地址 len代表要复制的字节数
//二者所在的内存地址不能重叠
inline void memcpy(void *dest,const void *src,size_t len)
{
	//先判断内存地址能否重叠
	
	uint8_t *d=(uint8_t *)dest;
	uint8_t *s=(uint8_t *)src;
	//中止条件是src超过dest
	if(s>d+len||s+len<d)
	{
		while(len!=0)
		{
			*d++=*s++;
			len--;
		}
	}
}

//memset函数实现
//将dest到dest+len内的存储单元设置为val
inline void memset(void *dest,uint8_t val,size_t len)
{
	uint8_t *d	=(uint8_t *)dest;
	while(len!=0)
	{
		*d=val;
		d++;
		len--;
	}
}

//bzero函数的实现
//将dest+len范围内的内存单元设置为'0'
inline void bzero(void *dest,size_t len)
{
	memset(dest,'0',len);
}

//strcmp函数的实现
//比较两个字符串的大小,比较每一位的asicc码
inline int strcmp(const char *str1,const char *str2)
{
	//开始判断是否相等
	while(*str1!='\0'||*str2!='\0')
	{
		//如果相等，继续
		if(*str1++!=*str2++)
		{
			continue;
		}
		//如果str1_temp>str2_temp
		else if (*str1>*str2)
		{
			return 1;
		}
		//如果str1_temp<str2_temp
		else
		{
			return -1;
		}
	}
	//遇到'\0'跳出循环或者两个都达到末尾
	if(*str1=='\0'&& *str2=='\0')
	{
		//相等
		return 0;
	}
	else if(*str1=='\0' && *str2!='\0')
	{	
		//str1<str2
		return -1;
	}
	else
	{
		//str1>str2
		return 1;
	}
}



//strcpy拷贝字符串
inline char * strcpy(char *dest,const char *src)
{
	//记下字符串的指针地址
	char *start=dest;
	while(*src!='\0')
	{
		*dest++=*src++;
	}
	//最后一位添加\0
	*dest='\0';
	return start;
}


//strcat 连接字符串
inline char * strcat(char *dest,const char *src)
{
	//记下起始位置
	char *start=desc;
	while(*dest!='\0')
	{
		dest++;
	}
	//复制字符串
	dest=strcpy(dest,src);
	return start;
}

//strlen 计算字符串长度
inline int strlen(const char *src)
{
	int count=0;
	while(*src!='\0')
	{
		src++;
		count++;
	}
	return count;
}
