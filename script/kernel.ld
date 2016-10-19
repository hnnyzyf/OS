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
	PROVIDE( kern_start= 0xc0100000 )
	/*段起始位置*/
	. =  0x100000;
	/*新添加的初始化段*/
	.init.text :
	{
		*(.init.text)
		. =ALIGN(4096);
	}
	.init.data :
	{
		*(.init.data)
		. =ALIGN(4096);
	}
	. += 0xc0000000
	.text : AT(ADDR(.text)- 0xc0000000)
	{
		*(.text)
		. = ALIGN(4096);
	}
	.data : AT(ADDR(.text)- 0xc0000000)
	{
		*(.data)
		*(.rodata )
		. = ALIGN(4096);
	}
	.bss : AT(ADDR(.text)- 0xc0000000)
	{
		*(.bss)
		. = ALIGN(4096);
	}
	.stab : AT(ADDR(.text)- 0xc0000000) 
	{
		*( .stab )
			. = ALIGN(4096);
	}
	.stabstr : AT(ADDR(.text)- 0xc0000000)
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
