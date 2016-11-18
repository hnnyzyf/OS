/*###################################################################
# File Name:heap.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年11月15日 星期二 22时18分01秒
# Last Modified2016年11月15日 星期二 22时18分01秒
####################################################################*/


#ifndef INCLUDE_HEAP_H_
#define INCLUDE_HEAP_H_

#include "pmm.h"
#include "types.h"

//定义堆的起始位置

#define HEAP_START 0xE0000000

//为每个内存块添加一个头
typedef struct header
{
	struct header *prev;		//内存块管理指针
	struct header *next;
	uint32_t allocated:1;		//是否已经分配
	uint32_t length:31;			//当前内存块的长度			
}header_t;

//初始化堆
void init_heap();

//内存申请
void *kmalloc(uint32_t len);

//内存释放
void kfree(void *pointer);

void test_heap();
#endif
