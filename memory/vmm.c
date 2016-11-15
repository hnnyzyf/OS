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
	//内核结束点
	uint32_t kern_end_addr=(uint32_t)kern_end;
	uint32_t i,j;
	//设置内核页目录项
	for(i=PGD_INDEX(PAGE_OFFSET),j=0;i<=PGD_INDEX(kern_end_addr);i++,j++)
	{
		pgd_kern[i]=((uint32_t)pte_kern[j]-PAGE_OFFSET)|PAGE_PRESENT|PAGE_WRITE;
	}
	//设置内核页表项
	uint32_t phy_base_addr=0x0;
	for(i=0;i<=PGD_INDEX(kern_end_addr-PAGE_OFFSET);i++)
	{
		for(j=0;phy_base_addr+PAGE_OFFSET<kern_end_addr&&(j<1024);j++)
		{
			pte_kern[i][j]=phy_base_addr|PAGE_PRESENT|PAGE_WRITE;
			phy_base_addr+=VIRTUAL_PAGE_SIZE;
		}
	}

	//---------------------注册14号页故障中断----------------
	register_interrupt_handler(IRQ14,&page_fault);
	//--------------------更新Cr3寄存器-------------------------
	uint32_t kern_stack_phy_address=(uint32_t)pgd_kern-PAGE_OFFSET;
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
void map(pgd_t *pgd_now,uint32_t virtual_addr,uint32_t physical_addr,uint32_t flags)
{
	//获得线性地址的页目录项索引和页表索引
	uint32_t pgd_index=PGD_INDEX(virtual_addr);
	uint32_t pte_index=PTE_INDEX(virtual_addr);

	//先判断是否已经在页表中映射了物理内存
	//获得页目录项，页目录项中存储的是物理地址
	pgd_t *pgd_temp=(pgd_t *)pgd_now[pgd_index];
	//声明页表，访问页表需要访问虚拟地址
	pte_t *pte_temp;
	//1.判断页目录项是否已经指向了一个页表，如果没有，则需要申请一个物理页
	if(!pgd_temp)
	{
		//获得一个物理页，且插入页目录项的物理地址
		pgd_temp=(pgd_t *)pmm_alloc_page();
		pgd_now[pgd_index]=(uint32_t)pgd_temp|PAGE_PRESENT|PAGE_WRITE;
		//获得页表的虚拟的地址(线性地址)

	}
	//2.如果已经存在该页目录项，则去该目录对应的页表查询是否存在对应的物理页
	pte_temp=(pte_t *)(pgd_now[pgd_index]+PAGE_OFFSET);
	//3.将物理页插入到对应的页表项中以完成映射
	pte_temp[pte_index]=(uint32_t)physical_addr|flags;

	//4.通知cpu更新页缓存
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
	pte_t * pte_temp=(pte_t *)((uint32_t)pgd_temp+PAGE_OFFSET);
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
	pte_t *pte_temp=(pte_t *)((uint32_t)pgd_temp+PAGE_OFFSET);
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

