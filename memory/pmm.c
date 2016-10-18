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


#define debug 0

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
	//uint32_t used=(kern_end-kern_start+1023);
	//printf("kernel in memory used:%d B\n",used);
}

//----------------------------物理内存分配函数----------------------------------

//定义物理内存页面管理栈
static uint32_t pmm_page_stack[PAGE_MAX_SIZE+1];

//定义栈顶指针
static uint32_t pmm_page_stack_top;

uint32_t phy_page_count;

//初始化内存分配
void init_pmm()
{
	//首先内核本身的内存是不做分配的,所以要回的内核的终点位置
	//以及可以分配的内存段的起始和初始位置
	//获得内存地址数组
	mmap_t *mmap=(mmap_t *)glb_mboot_ptr->mmap_addr;
	//获得数据的长度
	uint32_t mmap_length=glb_mboot_ptr->mmap_length/sizeof(mmap_t);
	//声明页面地址
	uint32_t page_addr=0;
	//初始化物理页面数量
	phy_page_count=0;
	//初始化栈顶
	pmm_page_stack_top=0;
	//获得可供分配的内存区域
	int i=0;
	for(i=0;i<mmap_length;i++)
	{
		//判断不是保留区域且是内核的其实区域
		if(mmap[i].type==0x1 &&mmap[i].base_addr_low==0x100000)
		{
			//获得page_addr的起始地址，是在内核部分后的第一页
			//起始地址需要4k对齐
			page_addr=(mmap[i].base_addr_low+(kern_end-kern_start+1)+1+PMM_PAGE_SIZE)& PHY_PAGE_MASK;
#if debug==1
			printf("the first page is %x\n",page_addr);
#endif
			//终止条件是
			//1.page_addr的地址超出区域大小
			//2.物理页面数量超过最大物理页面个数
			while(page_addr<mmap[i].base_addr_low+mmap[i].length_low && phy_page_count<=PAGE_MAX_SIZE)
			{
				//需要判断最后一个页面的大小是否小于4KB，小于的话则不分配
				if(mmap[i].base_addr_low+mmap[i].length_low-page_addr>=0x1000)
				{
					//入栈
					pmm_free_page(page_addr);
				}
				page_addr+=PMM_PAGE_SIZE;
			}
		}
	}

}


//返回一个页的物理内存
//判断栈是否为空
uint32_t pmm_alloc_page()
{
	//如果没有页可分
	assert(pmm_page_stack_top!=0,"Out of Memory!");
	return pmm_page_stack[pmm_page_stack_top--];
}

//释放申请的内存
//就是将当前的内存也放入到栈里边
//第一个页不分配，作为保护
void pmm_free_page(uint32_t page)
{
	//判断栈是否溢出，溢出的话，抛出错误
	//判断栈顶是否已经超过页面最大上限
	assert(pmm_page_stack_top!=PAGE_MAX_SIZE,"Out of PMM Stack!");
	pmm_page_stack[++pmm_page_stack_top]=page;
	phy_page_count++;
#if debug==1
	printf("the freed address:%x the stack top:%d",page,pmm_page_stack_top);
	printf("\n");
#endif
}











