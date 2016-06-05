###################################################################
# File name:kernel.ld
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年06月06日 星期一 04时38分58秒
# Last Modified:2016年06月05日 星期日 23时16分37秒
####################################################################



ENTRY(start)
SECTIONS
{
	/*段起始位置*/
	.=0x100000;
	.text:
	{
		*(.text)
			.=ALIGN(4096);
	}
	.data：
	{
		*(.data)
		*(.rodata)
		.=ALIGN(4096);
	}
	.bss：
	{
		*(.bss)
		.=ALGIN(4096);
	}
	.stab:
	{
		*(.stab)
			.=ALGIN(4096);
	}
	.stabstr:
	{
		*(.stabstr)
			.=ALGIN(4096);
	}

/DISCARD/:
	{
		*(.comment)
		*(.eh_frame)
	}

}
