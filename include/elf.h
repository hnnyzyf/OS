//###################################################################
//# File name:elf.h
//# Author:YFzhang
//# mail:hnnyzyf2013@163.com
//# Created Time:2016年09月08日 星期四 22时31分33秒
//# Last Modified:2016年09月08日 星期四 22时31分44秒
//####################################################################

#ifndef INCLUDE_ELF_H
#define INCLUDE_ELF_H

#include "types.h"

#define EI_NIDENT

//-----------------------------------------elf的header数据结构-------------------------------------

typedef struct elf_header_t
{
	uint8_t e_ident[EI_NIDENT];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
}__attribute__((packed)) elf_header_t;

//------------------------------------------elf的section头部----------------------

typedef struct elf_section_t
{
	uint32_t sh_name;//指的是名字在字符串表中的索引位置
	uint32_t sh_type;
	uint32_t sh_flags;
	uint32_t sh_addr;
	uint32_t sh_offset;
	uint32_t sh_size;
	uint32_t sh_link;
	uint32_t sh_info;
	uint32_t sh_addralign;
	uint32_t sh_entsize;
}__attribute__((packed)) elf_section_t;


//-------------------------------------------elf的符号表-----------------------


typedef struct elf_symbol_t
{
	uint32_t st_name;
	uint32_t st_value;
	uint32_t st_size;
	uint8_t st_info;
	uint8_t st_other;
	uint16_t st_shndx;
}__attribute__((packed)) elf_symbol_t;





//--------------------------------------------elf信息---------------------------

typedef struct elf_t
{
	//符号表
	elf_symbol_t *symtab;
	uint32_t symtabsize;
	//字符串表
	const char *strtab;
	uint32_t strtabsize;
}elf_t;


//从multiboot中读取elf信息
elf_t elf_from_multiboot(multiboot_t *src);


//查看符号表信息
const char * elf_lookup_symbol(uint32_t addr,elf_t *elf);
#endif
