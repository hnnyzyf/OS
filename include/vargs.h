/*###########################################
# File name:vargs.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:Tue 21 Jun 2016 11:59:27 PM CST
# Last Modified:Sat 25 Jun 2016 02:51:56 AM CST
###########################################*/

#ifndef INCLUDE_VARGS_H_
#define INCLUDE_VARGS_H_

typedef __builtin_va_list va_list;

#define va_start(ap,last)		(__builtin_va_start(ap,last))
#define va_arg(ap,type)			(__builtin_va_arg(ap,type))
#define va_end(ap)


#endif //INCLUDE_VARGS_H_


//关于可变个数参数类型的描述
//#define _intsizeof(n) ((sizeof(n)+sizeof(int)-1))&~(sizeof(int)-1)
//这个的原因是使参数的地址对齐
//因为cpu在处理4的倍数的地址时更有效率
//以上的宏保证将n原本的调整为4的倍数因为后两位a2位或者4位(分别是32位机和64位机)
//
//#define va_start(ap,last)		(ap=(va_list *)&ap+_intsizeof(last))
//首先将ap的地址强制转换为字符串，然后加上类型的对齐4的倍数，此为第一个参数的内存地
//
//define va_arg(ap,type) (*(type*)((ap+=_intsizeof(type))-_intsizeof(type)))
//比如
//char * format,int a,int b



//内核打印函数
//获取参数，根据每一个name，输出相应的参数
//---读取format
//---while(format)
//------获得参数，输出结果

//类型枚举
typedef
enum pr_type
{
	pr_int=1,
	pr_unsigned_int8=2,
	pr_unsigned_int10=3,
	pr_unsigned_int16=4,
	pr_float=5,
	pr_double=6,
	pr_char=7,
	pr_string=8,
	pr_void=9
}pr_type_t;

