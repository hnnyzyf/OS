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
	//获取节区header的指针
	elf_section_t *section=(elf_section__t *)src->addr;
	//获取节区header中项的总数
	int length=src->num;
	//获取字符串表的指针
	uint32_t sh_addr=section[src->shndx]->sh_addr;
	//向elf中开始填入数据
	for(i=0;i<length;i++)
	{
		//从第一个节区开始获取名称
		//指向字符串表中第section[i].sh_name个字符串
		const char *name=(const char *)(sh_addr+section[i].sh_name);
		//判断是否是字符串表
		if(strcmp(name,".strtab")==0)
		{
			elf.strtab=(uint32_t *)section[i].sh_addr;
			elf.strtabsize=section[i].sh_size;
		}
		//判断是否是符号表
		if(strcmp(name,".symtab")==0)
		{
			elf.strtab=(elf_symbol_t *)section[i].sh_addr;
			elf.symtabsize=section[i].sh_size
		}
	}
	return elf;

}




