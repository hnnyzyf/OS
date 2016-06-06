;###################################################################
;# File Name:boot.s
;# Author:YFzhang
;# mail:hnnyzyf2013@163.com
;# Created Time:2016年06月06日 星期一 14时45分44秒
;# Last Modified2016年06月06日 星期一 14时45分44秒
;####################################################################

;Multiboot 魔数，由规范决定的
MBOOT_HEADER_MAGIC equ 0x1BADB002

;0号位表示所有的引导模块将按(4KB)边界对齐
MBOOT_PAGE_ALIGN euq 1<<0

;1号位通过Multiboot信息结构的mem_*域包括可用内存的信息
;告诉(把内存空间的信息包含在信息结构中GRUBMultiboot)
MBOOT_MEM_INFO equ 1<<1

;定义我们使用的Multiboot的标记
MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALGIN|MBOOT_MEM_INFO

;域是一个位的无符号值，当与其他域checksum32magic,也就是和magicflags相加时，要求其结果必须是位的无符号值32 0，即(magic+flags+checksum=0)
MBOOT_CHECKSUM equ -(MBOOT_HEADER_MAGIC+MBOOT_HEADER_FLAGS)

;符合规范的Multiboot OS映像需要这样一个magic Multiboot头
;Multiboot头的分布必须如下表所示
;--------------------------------------------------------------------
;偏移量类型域名备注
;
;0 u32 magic 必须
;4 u32 flags 必须
;8 u32 checksum 必须
;
;我们只使用这些就够了
;--------------------------------------------------------------------

;--------------------------------------------------------------------
[BITS 32]			;所有代码以32-bit的方式编译
section .text		;代码段从这里开始
;在代码段的起始位置设置符合Multiboot规范的标记

dd MBOOT_HEADER_MAGIC	;GRUB会通过这个魔术判断该映像是否支持
dd MBOOT_HEADER_FLAGS	;GRUB的一些加载时选项，其详细注释在定义处
dd MBOOT_CHECKSUM		;检测数值，其含义在定义处

[GLOBAL start]			;向外部声明内核代码入口，此处提供该声明给链接器
[GLOBAL hlb_mboot_ptr]	;向外部声明struct









































