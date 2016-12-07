/*###########################################
# File Name:common.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:Fri 17 Jun 2016 04:55:47 AM CST
# Last ModifiedFri 17 Jun 2016 04:55:47 AM CST
###########################################*/
#ifndef INCLUDE_BOOTMEM_H_
#define INCLUDE_BOOTMEM_H_

#include "multiboot.h"
#include "types.h"


struct bootmem
{
	uint32_t size;
	uint32_t base_addr;
	uint32_t length;
	uint32_t type;
};

typedef struct bootmem bootmem_t;


//生成bootmem_t结构
void bootmem_t(uint32_t start_addr,unit32_t end_addr);

