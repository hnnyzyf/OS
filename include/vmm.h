/*###################################################################
# File Name:vmm.h
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年10月19日 星期三 21时13分14秒
# Last Modified2016年10月19日 星期三 21时13分14秒
####################################################################*/

#ifndef INCLUDE_VMM_H_
#define INCLDUE_VMM_H_


#include "types.h"
#include "idt.h"

//内核偏移地址
//仿照linux，将内核的地址映射到3G以上
#define PAGE_OFFSET 0xc0000000

//P---位0是存在(present)的标志，指明表项对地址转换是否有效
//p=1表示有效  p=0表示无效
//在也转换过程中，如果涉及的页目录或页表表项无效，则会引发14号异常
//如果p=0，除表示表项无效外，其余为可供程序自由使用
//操作系统可以用这些位来保存已存储在磁盘上的页面的序号
#define PAGE_PRESENT 0x1


//R/W---位1是读/写(read/write)标志.
//如果等于1，表示页面可以被读，写和执行
//如果等于0，表示页面只读或执行
//当处理器运行在超级用户特权级0，1，2时，该位无效
//页目录项中的R/W位对其所映射的所有页面起效
#define PAGE_WRITE 0x2


//U/S---位2是用户/超级用户（user/Supervisor）标志
//如果等于1，那么运行在任何特权级上的程序都可以访问该页面
//如果等于0，那么页面只能被运行在超级哟牛股特权级0，1，2上的程序访问
//页目录项中的U/S位对其所映射的所有页面有效
#define PAGE_USER 0x4

//虚拟分页的大小
#define VIRTUAL_PAGE_SIZE 0x1000

//虚拟页掩码，用来4K对齐
#define VIRTUAL_PAGE_MASK 0xffffc000

//获取一个地址的页目录项
#define PGD_INDEX(x) ((x>>22)& 0xfff)

//获取一个地址的页表偏移
#define PTE_INDEX(x) ((x>>12)& 0x3ff)

//获取一个地址的页内偏移
#define OFFSET_INDEX(x)	(x& 0xfff) 

//--------------定义数据结构-------------------------
//页目录表的表项数据结构
//因为所有的页均是4kb对齐，所以后12位均为0
//前20位记录所有的页表的地址
typedef uint32_t pgd_t;

//页表的表项数据结构
//每一项记录一个物理地址
typedef uint32_t pte_t;

//页目录项的数量
//存储的是
#define PGD_SIZE (VIRTUAL_PAGE_SIZE/sizeof(pte_t))

//页表成员数量
#define PTE_SIZE (VIRTUAL_PAGE_SIZE/sizeof(uint32_t))

//映射512M内存所需要的页表数
#define PTE_COUNT (512/4)

//内核页目录区域
extern pgd_t pgd_kern[PGD_SIZE];

//初始化虚拟内存管理
void init_vmm();

//更换当前的页目录
void switch_pgd(uint32_t pgd_phy_addr);

//使用flags指出页权限，将物理地址pa映射到虚拟的值va
void map(pgd_t *pgd_now,uint32_t virtual_addr,uint32_t physical_addr,uint32_t flags);

//取消虚拟地址对物理地址的映射
void unmap(pgd_t *pgd_now,uint32_t virtual_addr);

//如果虚拟地址va映射到物理地址返回1
//同时如果pa不是空指针则把物理地址写入到pa参数
uint32_t get_mapping(pgd_t *pgd_now,uint32_t va,uint32_t *pa);

//页错误中断函数处理
void page_fault(reg_t *regs);

//
#endif
