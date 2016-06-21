/*###########################################
# File Name:vargs.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:Tue 21 Jun 2016 11:59:27 PM CST
# Last ModifiedTue 21 Jun 2016 11:59:27 PM CST
###########################################*/

#ifndef INCLUDE_VARGS_H_
#define INCLUDE_VARGS_H_

typedef __builtin_va_list va_list;

#define va_start(ap,last)		(__builtin_va_start(ap,last))
#define va_arg(ap,type)			(__builtin_va_arg(ap,type))
#define va_end(ap)


#endif //INCLUDE_VARGS_H_
