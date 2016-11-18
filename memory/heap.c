/*##################################################################
# File Name:heap.c
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年11月15日 星期二 22时23分59秒
# Last Modified2016年11月15日 星期二 22时23分59秒
####################################################################*/



#include "types.h"
#include "heap.h"
#include "pmm.h"
#include "vmm.h"
#include "debug.h"
#include "string.h"

//指的是当前堆的最大位置
static uint32_t heap_max=HEAP_START;

//内存管理头指针
static header_t *heap_first;

//申请一个内存页
//如果从链表中找不到需要的长度的内存，则需要申请完整的物理内存
//start指的是header的起始地址
//len指的应该是需要申请的内存的长度
static void alloc_chunk()
{
	//申请一个物理页
	uint32_t page=pmm_alloc_page();
	//将物理页添加到页表中
	//物理页对应的虚拟地址是heap_max，
	map(pgd_kern,heap_max,page,PAGE_PRESENT|PAGE_WRITE);
	//上限增加一个PAGE_SIZE
	heap_max+=PMM_PAGE_SIZE;
}

//释放内存块
//回收到物理内存中并取消映射
static void free_chunk(header_t *chunk)
{
	unmap(pgd_kern,(uint32_t)chunk);
}

//切分内存块
//当前的内存块不指针不变，将切割出来的部分重新生成一个新的内存块
static void split_chunk(header_t *chunk,uint32_t len)
{
	//剩余的内存满足添加一个内存头
	if(chunk->length-len>sizeof(header_t))
	{
		//获得下一个内存头的起始地址
		header_t *next_header=(header_t *)((uint32_t)chunk+len);
		//更新新的header的指针
		next_header->next=chunk->next;
		next_header->prev=chunk;
		//更新原chunk的下一个header头的父指针
		if(chunk->next!=NULL)
		{
			chunk->next->prev=next_header;
		}
		//更新chunk->next的指针
		chunk->next=next_header;
		//更新新header其他属性
		next_header->allocated=0;
		next_header->length=chunk->length-len-sizeof(header_t);
		//更新chunk的长度
		chunk->length=len;
	}
	//更新chunk的分配属性
	chunk->allocated=1;
}

//合并内存块
//判断能否和前后的内存合并并成为一个新页
static void combine_chunk(header_t *chunk)
{
	//中止条件是，到达一个页的最低地址和最高地址
	header_t *parent_node=chunk->prev;
	header_t *child_node=chunk->next;	
	//合并子节点
	//终止条件有两个
	//1.已经到最后一个节点了
	//2.下一个节点已经越界了
	//3.下一个节点已经分配过了
	while(child_node!=NULL)
	{	
		if((uint32_t)child_node<_align_down((uint32_t)chunk,PMM_PAGE_SIZE) && child_node->allocated==0)
		{
			//更新chunk节点
			chunk->next=child_node->next;
			chunk->length+=child_node->length+sizeof(header_t);
			//更新child_node的下一个节点
			child_node=child_node->next;
		}
		else
		{
			//跳出循环
			break;
		}
	}
	//合并父节点
	while(parent_node!=NULL)
	{
		if((uint32_t)parent_node>_align_up((uint32_t)chunk,PMM_PAGE_SIZE) && parent_node->allocated==0)
		{
			//更新父节点
			parent_node->next=chunk->next;
			parent_node->length+=chunk->length+sizeof(header_t);
			parent_node=parent_node->prev;
		}
		else
		{
			//跳出循环
			break;
		}
	}

	//根据最新的parent_node和child_node判断是否需要free page
	if(child_node==NULL)
	{
		//判断parent_node是不是null,表名没有到达最初始的位置
		if(parent_node!=NULL)
		{
			parent_node=parent_node->next;
			if((uint32_t)parent_node==_align_down((uint32_t)chunk,PMM_PAGE_SIZE))
			{
				free_chunk(parent_node);
			}
		}
		//表明到达head_list
		else
		{
			free_chunk(heap_first);
		}
	}

}




//初始化堆
//一般0x0是无用的
void init_heap()
{
	//初始指针为0
	heap_first=(header_t *)0x0;
}

//内存申请
//申请置顶长度的内存,并返回链表内存地址
//申请到的是连续的物理内存
//len的长度判断，不能超过一页
void *kmalloc(uint32_t len)
{
	//先判断分配的内存是否超过1页
	assert(len<=4096,"Out of One Page!\n");
	//判断当前链表中是否存在足够的内存供分配
	//实际分配的内存是链表头+需要的内存
	//从链表头开始扫描
	header_t *heap_pointer=heap_first;
	header_t *parent_header=0x0;
	//判断是不是起始heap_first或者是不是最后一个header
	while(heap_pointer!=0)
	{
		//判断当前内存是否分配并且内存块是否满足需求
		if(heap_pointer->allocated==0 && heap_pointer->length>=len)
		{
			//分配内存
			split_chunk(heap_pointer,len);
			//指明该内存已经分配
			return (void *)((uint32_t)heap_pointer+sizeof(header_t));	 
		}
		parent_header=heap_pointer;
		heap_pointer=heap_pointer->next;
	}
	//如果没有找到合适的分区，则需要申请新的内存页
	//新的header应该从最后一个header的内存中获得
	//先判断parent_header是否为0
	//split_chunk保证如果最后一个header是可分配的，则剩余的空间足够添加一个header_t
	//声明新的header的位置
	header_t * heap_start=(header_t *)0x0;
	if(parent_header!=0)
	{
		//从当前最后一个header分离出内存申请
		if(parent_header->allocated==0)
		{
			//获得新的header的起始位置
			heap_start=(header_t *)((uint32_t)parent_header+parent_header->length);
			//分配一个内存页
			alloc_chunk();
			//修改原始的节点
			parent_header->length=parent_header->length-sizeof(header_t);
			parent_header->next=heap_start;
			//初始化heap_start
			heap_start->prev=parent_header;
			heap_start->next=0;
			heap_start->allocated=0;
			heap_start->length=4096;
			split_chunk(heap_start,len);
			return (void *)((uint32_t)heap_start+sizeof(header_t));
		}
		//最后一个header不可分配且没有满足需求的内存块
		else
		{
			//获得header的位置
			heap_start=(header_t *)heap_max;
		}
	}	
	//表明当前是初始化状态
	else
	{
		heap_start=(header_t *)heap_max;
	}
	//需要立刻分配内存，因为此时虚拟页对应的物理内存，会立刻报警缺页中断
	alloc_chunk();
	//如果header需要放在新的内存块中，初始化新的header
	heap_start->prev=parent_header;
	heap_start->next=0;
	heap_start->allocated=0;
	heap_start->length=PMM_PAGE_SIZE-sizeof(header_t);
	//修改原来的链表尾部
	parent_header->next=heap_start;
	//判断要申请的内存是否是满足条件
	//再重新调用kmalloc
	return kmalloc(len);
}


//内存释放
//从堆中释放掉内存
//首先确认释放掉的内存的位置，然后判断是否能合并，如果合并为一个页的话，则释放掉
void kfree(void *pointer)
{
	//将所有的内存给释放掉，内存全部清零
	memset((header_t *)pointer,0,((header_t *)pointer)->length);
	((header_t *)pointer)->allocated=0;
	combine_chunk((header_t *)pointer);
}



//--------------------------------------------------------------------------------------
void test_heap()
{
	printf("begin to test kmalloc and kfree\n");
	void *addr1=kmalloc(50);
	printf("kmalloc 50 bytes in %x\n",addr1);
	void *addr2=kmalloc(4096);
	printf("kmalloc 4096 bytes in %x\n",addr2);
	void *addr3=kmalloc(100);
	printf("kmalloc 100 bytes in %x\n",addr3);
	
	printf("free memory in %x\n",addr3);
	kfree(addr3);
	printf("free memory in %x\n",addr2);
	kfree(addr2);
	printf("free memory in %x\n",addr1);
	kfree(addr1);
}

