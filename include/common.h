/*###########################################
# File Name:common.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:Fri 17 Jun 2016 04:55:47 AM CST
# Last ModifiedFri 17 Jun 2016 04:55:47 AM CST
###########################################*/
#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include "types.h"
//端口号为0-65535，即0到2^16-1，所有用uint16_t
//端口一次读取一个字节，所以用uint8_t,一个char是一个字节
//端口写一个字节
void outb(uint16_t port,uint8_t value);

//端口读一个字节
uint8_t inb(uint16_t port);

//端口读一个字
uint16_t inw(uint16_t port);

#endif //INCLUDE_COMMON_H_
