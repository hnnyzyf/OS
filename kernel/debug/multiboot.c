#include "multiboot.h"
#include "debug.h"


#define ifdefine(flag,bit) ((flag>>bit)& 0x1)



//获得multiboot的所有信息
//因为定义了全局指针指向multiboot头，所以直接使用即可
void get_multiboot_structure()
{
	multiboot_t mboot_header;
	//获得头信息
	mboot_header=*glb_mboot_ptr;
	//输出flag的信息，查看定义了哪些内容
	printf("flags:%b\n",mboot_header.flags);
	//判断定义了哪些位，然后根据位来输出结果
	//判断flag[0]是否存在，存在则不为0
	if(ifdefine(mboot_header.flags,0))
	{
		printf("The flag[0] sectiion:\n");
		printf("mem_lower:%d	mem_upper:%d\n",mboot_header.mem_lower,mboot_header.mem_upper);
	}
	else
	{
		printf("The flag[0] no define\n");
	}
	//判断flag[1]
	if(ifdefine(mboot_header.flags,1))
	{
		printf("the flag[1] section:\n");
		printf("boot_device:%x\n",mboot_header.boot_device);
	}
	else
	{
		printf("the flag[1] no define\n");
	}
	//判断flag[2]
	if(ifdefine(mboot_header.flags,2))
	{
		printf("the flag[2] section:\n");
		printf("the cmdline is %s\n",(char *)mboot_header.cmdline);
	}
	else
	{
		printf("the flag[2] no define\n");
	}
	//判断flag[3]
	if(ifdefine(mboot_header.flags,3))
	{
		printf("the flag[3] section:\n");
		printf("modes_count:%d   mods_addr:%x\n",mboot_header.mods_count,mboot_header.mods_addr);
	}
	else
	{
		printf("the flag[3] no define\n");
	}


}
