/*###################################################################
# File Name:pmm.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年10月14日 星期五 01时34分59秒
# Last Modified2016年10月14日 星期五 01时34分59秒
####################################################################*/

#ifndef INCLUDE_PMM_H_
#define INCLUDE_PMM_H_

#include "types.h"
#include "multiboot.h"


//内核文件在内存中的起始和结束位置
//在连接器脚本中要求连接器定义
extern uint8_t kern_start[];
extern uint8_t kern_end[];


//定义获取操作系统内存
void  show_memory_map();

//获得内核内存分布
void  show_kernel_map();

//获得操作系统的内存
uint32_t get_memory_start();

//获得内核的内存
uint32_t get_kernel_length();

//-----------------------------------物理内存管理--------------------------------

//线程栈大小为8M
#define STACK_SIZE 8192


//支持的最大物理内存(512M)
#define PMM_MAX_SIZE 0x20000000

//物理内存页框大小(4KB)
#define PMM_PAGE_SIZE 0x1000

//最多支持的物理页面个数
#define PAGE_MAX_SIZE (PMM_MAX_SIZE/PMM_PAGE_SIZE)

//页掩码按照4096对齐
#define PHY_PAGE_MASK 0xffffc000

//动态分配物理内存的总数
extern uint32_t phy_page_count;

//初始化物理内存管理
void init_pmm();

//返回一个页的物理内存
uint32_t pmm_alloc_page();

//释放申请的内存
void pmm_free_page(uint32_t page);
#endif
