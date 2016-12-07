/*###################################################################
# File Name:vmm.c
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年10月19日 星期三 22时11分00秒
# Last Modified2016年10月19日 星期三 22时11分00秒
####################################################################*/


#include "types.h"
#include "vmm.h"
#include "debug.h"
#include "idt.h"
#include "pmm.h"
#include "string.h"



extern uint32_t phy_memory_start;
extern uint32_t phy_memory_length;
//-------------------------------虚拟页管理数据结构--------------------------------
//内核进程页目录项表数据结构
//成员是pgd_t页目录项
//长度是一页存储的页目录项个数 4kb/4b=1024
pgd_t pgd_kern[PGD_SIZE] __attribute__((aligned(VIRTUAL_PAGE_SIZE)));

//内核页表项数据结构，为二维数组
//一维代表的是页表的数量,最多映射512M内存
//二维代表的是页表内页框的个数
//成员是页框的物理地址
//长度是一页存储的页框物理地址个数 4kb/4b=1024
//使用静态类型，只在本文件内有效
//操作系统最大是512M
static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute__((aligned(VIRTUAL_PAGE_SIZE)));

//------------------------------虚拟页管理函数-----------------------------------

//初始化虚拟内存管理
//初始化内核页表即可，因为现在只有内核地址被分配了
//在临时页表中,已经将前4M的内存映射到0xc000,0000这个地址以上，所以当前.text和.data
//段的地址都为0xc000,0000以上的地址
//页目录项和页表存储的地址均为物理地址
void init_vmm()
{
	//---------------------需要重新设置页目录项-------------------
	int i,j;
	//前3G的页目录置0
	for(i=0;i<PGD_INDEX(PAGE_OFFSET);i++)
	{
		pgd_kern[i]=0;
	}
	//物理内存区的映射最多1024-128M
	for(i=PGD_INDEX(PAGE_OFFSET);i<PTE_COUNT+PGD_INDEX(PAGE_OFFSET);i++)
	{
		for(j=0;j<PTE_COUNT;j++)
		{
			pgd_kern[i]=__PA((uint32_t)pte_kern[j])|PAGE_PRESENT|PAGE_WRITE;
		}
	}
	//只是映射了页表，但是并没有物理映射
	for(i=PTE_COUNT+PGD_INDEX(PAGE_OFFSET);i<PGD_INDEX(0xffffffff);i++)
	{
		pgd_kern[i]=0;
	}
	//-------------------------设置页表项----------------------------
	//设置内核起始的物理映射,映射了前4M为内核映射区
	for(i=0;i<1024;i++)
	{
		pte_kern[0][j]=(uint32_t)(i*PMM_PAGE_SIZE)|PAGE_PRESENT|PAGE_WRITE;
	}
	/*
	//将物理内存映射到对应的映射区中
	for(i=phy_memory_start;i<=phy_memory_start+phy_memory_length;i=i+PMM_PAGE_SIZE)
	{
		pte_kern[PGD_INDEX(__VA(i))][PTE_INDEX(__VA(i))]=i;
	}
	*/
	//---------------------注册14号页故障中断----------------
	register_interrupt_handler(IRQ14,&page_fault);
	//--------------------更新Cr3寄存器-------------------------
	uint32_t kern_stack_phy_address=__PA((uint32_t)pgd_kern);
	switch_pgd(kern_stack_phy_address);
}


//更换页目录
void switch_pgd(uint32_t pgd_phy_addr)
{
	asm volatile
	(
		"movl %0,%%cr3"
		:
		:"r"(pgd_phy_addr)
	);
}

//------------------------虚拟内存管理-------------------------------
//指出页权限,将物理页加入到虚拟地址映射中去
//传入的参数是虚拟地址，但是需要的是物理地址
//未完成
void map(pgd_t *pgd_now,uint32_t virtual_addr,uint32_t physical_addr,uint32_t flags)
{
	//获得线性地址的页目录项索引和页表索引
	uint32_t pgd_index=PGD_INDEX(virtual_addr);
	uint32_t pte_index=PTE_INDEX(virtual_addr);
	asm volatile("invlpg (%0)"::"a"(virtual_addr));
}

//取消映射
void unmap(pgd_t *pgd_now,uint32_t virtual_addr)
{
	//获得虚拟(线性)地址的页目录项索引和页表索引
	uint32_t pgd_index=PGD_INDEX(virtual_addr);
	uint32_t pte_index=PTE_INDEX(virtual_addr);
	//先判断是否已经确实存在映射关系
	pgd_t * pgd_temp=(pgd_t *)pgd_now[pgd_index];
	pte_t * pte_temp=(pte_t *)__VA((uint32_t)pgd_temp);
	//如果不存在物理映射，跳出
	assert((!pgd_temp || !pte_temp[pte_index]),"DO not exsits mapping!");
	//存在物理映射
	//1.回收物理页
	pmm_free_page((uint32_t)pte_temp[pte_index]);
	//2.取消页表对物理页的映射,s设为0即可
	pte_temp[pte_index]=0;
	//3.更新缓存
	asm volatile("invlpg (%0)"::"a"(virtual_addr));
}

//判断映射是否存在
uint32_t get_mapping(pgd_t *pgd_now,uint32_t virtual_addr,uint32_t *physical_addr)
{
	//获得虚拟(线性)地址的页目录项索引和页表索引
	uint32_t pgd_index=PGD_INDEX(virtual_addr);
	uint32_t pte_index=PTE_INDEX(virtual_addr);

	//判断映射关系是是否存在
	pgd_t *pgd_temp;
	pgd_temp=(pgd_t*)pgd_now[pgd_index];
	if(!pgd_temp)
	{
		return 0;
	}
	pte_t *pte_temp=(pte_t *)__VA((uint32_t)pgd_temp);
	if(!pte_temp[pte_index])
	{
		return 0;
	}
	//获得虚拟地址所指向的物理地址
	uint32_t *address=(uint32_t *)((uint32_t)pte_temp[pte_index]+OFFSET_INDEX(virtual_addr));
	//判断是否正确
	if((uint32_t)address==(uint32_t)physical_addr &&physical_addr!=NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

