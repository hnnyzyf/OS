//###################################################################
//# File Name:elf.c
//# Author:YFzhang
//# mail:hnnyzyf2013@163.com
//# Created Time:2016年09月09日 星期五 23时23分31秒
//# Last Modified2016年09月09日 星期五 23时23分31秒
//####################################################################

#include "string.h"
#include "elf.h"


//从multiboot_t中获取结构信息elf

elf_t elf_from_multiboot(multiboot_t *src)
{
	int i;
	elf_t elf;
	//将multiboot_t中的src->addr指针转换为section指针类型，则可以获取指针指向的目标地址的对应section结构
	elf_section_t *section=(elf_section__t *)src->addr;
	//获取section结构体数组的长度
	uint32_t length=src->shndx;
	//向elf中开始填入数据
	for(i=0;i<length;i++)
	{
		
	}

}
