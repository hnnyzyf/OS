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
void show_memory_map();

void show_kernel_map();

#endif
