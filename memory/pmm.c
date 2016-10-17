/*###################################################################
# File Name:pmm.c
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年10月14日 星期五 01时32分07秒
# Last Modified2016年10月14日 星期五 01时32分07秒
####################################################################*/


#include "multiboot.h"
#include "pmm.h"
#include "types.h"
#include "debug.h"


//获取操作系统的内存
void show_memory_map()
{
	//首先获得内存区域的地址指针
	mmap_t *mmap=(mmap_t *)glb_mboot_ptr->mmap_addr;
	//获得数组长度
	uint32_t mmap_length=glb_mboot_ptr->mmap_length/sizeof(mmap_t);
	//输出各个内存区域的属性
	int i=0;
	printf("Memory map:\n");
	for(i=0;i<mmap_length;i++)
	{
		printf("baseaddress:%x length:%x type:%x\n",(uint32_t)mmap[i].base_addr_low,(uint32_t)mmap[i].length_low,(uint32_t)mmap[i].type);
	}

}


//显示内核文件的位置信息
void show_kernel_map()
{
	printf("Kernel Memory Map:\n");
	printf("kernel in memory start:%x\n",kern_start);
	printf("kernel in memory stop:%x\n",kern_end);
	printf("kernel in memory used:%d KB\n",(kern_end-kern_start+1023)/1024);
}














