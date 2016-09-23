//###################################################################
//# File Name:elf.c
//# Author:YFzhang
//# mail:hnnyzyf2013@163.com
//# Created Time:2016年09月09日 星期五 23时23分31秒
//# Last Modified2016年09月09日 星期五 23时23分31秒
//####################################################################

#include "string.h"
#include "elf.h"
#include "multiboot.h"
#include "debug.h"

#define debug 1

//从multiboot_t中获取结构信息elf

elf_t elf_from_multiboot(multiboot_t *src)
{
	int i;
	elf_t elf;
	//elf初始化
	memset(&elf,0,sizeof(elf_t));
	//----------------------------------------------------------获取elf的section头信息--------------------------------
	//section头部是一个数组，数组类型是哦elf_section_t
	//
	//将multiboot_t中的src->addr指针转换为section指针类型，则可以获取指针指向的目标地址的对应section结构
	//获取section数组的地址
	elf_section_t *section=(elf_section_t *)src->addr;
	//获取section数组的长度
	int length=src->num;
	//----------------------------------------------------------获取字符串表的信息------------------------------------
	//获取字符串表的地址,转化为指针类型
	//serc->shndx存储的是在section头数组中字符串表的位置
	//整数类型
	uint32_t sh_addr=section[src->shndx].sh_addr;
	//----------------------------------------------------------elf中填入数据-----------------------------------------
	//向elf中开始填入数据
	for(i=0;i<length;i++)
	{
		//从第一个节区开始获取名称
		//------------------------------------------------获取节区名称字符串的起始位置--------------------------------
		//sh_name是从节区头部开始的偏移位置
		//
		//-------------------------------------------------0
		//
		//-------------------------------------------------section[0].sh_name
		//
		//
		//-------------------------------------------------section[1].sh_name
		//
		//指向字符串表中第section[i].sh_name个字符串,因为sg_addr是整数类型，所以加sh_name就是偏移sh_name的字节数
		//转化为字符串指针
		const char *name=(const char *)(sh_addr+section[i].sh_name);
#ifdef debug
		printf("the %dth symbol is %s\n",i,name);
#endif
		//判断是否是字符串表
		if(strcmp(name,".strtab")==0)
		{
			//section不为指针
			elf.strtab=(const char *)section[i].sh_addr;
			elf.strtabsize=section[i].sh_size;
		}
		//判断是否是符号表
		if(strcmp(name,".symtab")==0)
		{
			//section不为指针
			elf.symtab=(elf_symbol_t *)section[i].sh_addr;
			elf.symtabsize=section[i].sh_size;
		}
	}
	return elf;

}


//查看符号信息
//输入一个地址，查看对应的符号的情况
const char *elf_lookup_symbol(uint32_t addr,elf_t *elf)
{
	int i;
	//对于符号表中的每一个项目
	//符号表的大小/每一项的大小
#ifdef debug	
	printf("the number of symbol is %d\n",elf->symtabsize/sizeof(elf_symbol_t));
#endif
	for(i=0;i<elf->symtabsize/sizeof(elf_symbol_t);i++)
	{
		//首先判断符号表中的符号是否是函数名称,为2时表示该符号是函数名称符号
		if(ELF32_ST_TYPE(elf->symtab[i].st_info)==0x02)
		{
			//如果地址是在符号范围内
			if((addr>=elf->symtab[i].st_value)&&(addr<elf->symtab[i].st_value+elf->symtab[i].st_size))
			{
				return (elf->strtab+elf->symtab[i].st_name);
			}
		}
	}
	//无结果返回NULL
	return NULL;
}

