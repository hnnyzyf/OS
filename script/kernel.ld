/*###################################################################
# File name:kernel.ld
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年06月06日 星期一 04时38分58秒
# Last Modified:2016年06月05日 星期日 23时16分37秒
####################################################################*/


ENTRY(start)
SECTIONS
{
	PROVIDE( kern_start = 0xc0100000 );
	/*段起始位置*/
	. = 0x100000;
	/*新添加的初始化段*/
	.init.text :
	{
		*(.init.text )
		. = ALIGN(4096);
	}
	.init.data :
	{
		*(.init.data )
		. = ALIGN(4096);
	}
	/*kernel是从这里被加载到虚拟内存空间的*/
	/*实际的物理地址还是从0x100000开始的*/
	. += 0xc0000000;
	/*LMA是实际在内存中的地址*/
	/*因为如此，所以在进入kernel前，必须设定好虚拟内存分配机制，即开启分页*/
	.text : AT(ADDR(.text)- 0xc0000000)
	{
		*(.text)
		. = ALIGN(4096);
	}
	.data : AT(ADDR(.data)- 0xc0000000)
	{
		*(.data)
		*(.rodata )
		. = ALIGN(4096);
	}
	.bss : AT(ADDR(.bss)- 0xc0000000)
	{
		*(.bss)
		. = ALIGN(4096);
	}
	.stab : AT(ADDR(.stab)- 0xc0000000) 
	{
		*( .stab )
		. = ALIGN(4096);
	}
	.stabstr : AT(ADDR(.stabstr)- 0xc0000000)
	{
		*(.stabstr )
		. = ALIGN(4096);
	}

	PROVIDE( kern_end = . );
	/DISCARD/ :
	{
		*(.comment )
		*(.eh_frame )
	}
}
