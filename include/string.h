/*###########################################
# File name:string.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:Tue 21 Jun 2016 07:14:04 PM CST
# Last Modified:Tue 21 Jun 2016 07:14:21 PM CST
###########################################*/

#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include "types.h"

void memcpy(uint8_t *dest,const uint8_t *src,uint32_t len);

void memset(void *dest,uint8_t val,uint32_t len);

void bzero(void *dest,uint32_t len);

int strcmp(const char *str1,const char *str2);

char *strcpy(char *dest,const char *src);

char *strcat(char *dest,const char *src);

int strlen(const char *src);

#endif //INCLUDE_STRING_H_
