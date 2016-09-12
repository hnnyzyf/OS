//###################################################################
//# File Name:multiboot.h
//# Author:YFzhang
//# mail:hnnyzyf2013@163.com
//# Created Time:2016年09月08日 星期四 17时53分42秒
//# Last Modified2016年09月08日 星期四 17时53分42秒
//####################################################################


#ifndef INCLUDE_MULTIBOOT_H
#define INCLUDE_MULTIBOOT_H
#include "types.h"


//------------------------------------------------启动信息格式---------------------------------------------------
typedef struct multiboot_t
{
	uint32_t flags;
	//----------------flag[0]----------------------
	uint32_t mem_lower;
	uint32_t mem_upper;
	//----------------flag[1]---------------------
	uint32_t boot_device;
	//----------------flag[2]--------------------
	uint32_t cmdline;
	//----------------flag[3]-------------------
	uint32_t mods_count;
	uint32_t mods_addr;
	//--------------------以下是elf内核的段头段flag[5]-------------------------
	uint32_t num;//节区header表的个数
	uint32_t size;//节区header表的大小
	uint32_t addr;//存储的是elf的section_header地址,转换为对应类型指针类型即可获得相关信息
	uint32_t shndx;//字符串节区在节区header表中的索引位置
	//-------------------flag[6]----------------------
	uint32_t mmap_length;
	uint32_t mmap_addr;
	//-------------------flag[7]----------------------
	uint32_t drives_length;
	uint32_t drives_addr;
	//-------------------flag[8]----------------------
	uint32_t config_table;
	//-------------------flag[9]----------------------
	uint32_t boot_loader_name;
	//-------------------flag[10]---------------------
	uint32_t apm_table;
	//-------------------flag[11]---------------------
	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint16_t vbe_mode;
	uint16_t vbe_interface_seg;
	uint16_t vbe_interface_off;
	uint16_t vbe_interface_len;
}__attribute__((packed)) multiboot_t;



//--------------------------------------------flag的第5位设置了-------------------------------------------
//表示包由BIOS提供的一个机器内存映射的缓存的地址和长度。
//addr代表起始地址
//length代表大小
//
typedef struct mmap_t
{
	uint32_t size;
	uint32_t base_addr_low;
	uint32_t base_addr_high;
	uint32_t length_low;
	uint32_t length_high;
	uint32_t type;
}__attribute__((packed)) mmap_t;


//定义全局的multiboot的指针头
extern multiboot_t* glb_mboot_ptr;



#endif
