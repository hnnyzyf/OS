/*
###################################################################
# File Name:debug.c
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年09月14日 星期三 21时48分56秒
# Last Modified2016年09月14日 星期三 21时48分56秒
####################################################################
*/


#include "debug.h"
#include "multiboot.h"
#include "elf.h"


//定义全局静态变量
static elf_t kernel_elf;

//初始化debug
void init_debug()
{
	//因为已经定义了全局的multiboot结构体指针,所以直接提取从全局的multiboot中提取elf信息即可
	//获取elf文件的字符串表和符号表
	kernel_elf=elf_from_multiboot(glb_mboot_ptr);
}


//
