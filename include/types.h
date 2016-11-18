/*###########################################
# File Name:types.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:Fri 17 Jun 2016 04:42:12 AM CST
# Last ModifiedFri 17 Jun 2016 04:42:12 AM CST
###########################################*/
#ifndef INCLUDE_TYPES_H
#define INCLUDE_TYPES_H

//定义NULL
#ifndef NULL
#define NULL 0
#endif

//定义TRUE和FALSE
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif


//定义size_t
typedef unsigned int size_t;
//定义无符号32位二进制
typedef	unsigned int uint32_t;
//定义有符号32位二进制
typedef	int	int32_t;
//定义有符号16位二进制
typedef	unsigned short uint16_t;
//定义无符号16位二进制
typedef short int16_t;
//定义无符号8位二进制
typedef unsigned char uint8_t;
//定义有符号8位二进制
typedef char int8_t;



//按照size对齐
#define _align(a,size)	(((a)+(size)-1) & ~((size)-1))
//求地址a以size为规模的上界
#define _align_down(a,size)	((a) & ~(size-1))
//求下界
#define _align_up(a,size)	((a+size-1) & ~(size-1))

#endif //INCLUDE_TYPES_H_
