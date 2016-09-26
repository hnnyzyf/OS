#include "multiboot.h"
#include "debug.h"




//获得multiboot的所有信息
//因为定义了全局指针指向multiboot头，所以直接使用即可
void get_multiboot_structure()
{
	multiboot_t mboot_header;
	//获得头信心
	mboot_header=*glb_mboot_ptr;
	//输出flag的信息，查看定义了哪些内容
	printf("flags:%x\n",mboot_header.flags);
	//判断定义了哪些位，然后根据位来输出结果
	//判断flag[0]是否存在，存在则不为0
	if(mboot_header.flags & 0x1)
	{
		printf("The flag 1 sectiion:\n");
		printf("mem_lower:%d	mem_upper:%d\n",mboot_header.mem_lower,mboot_header.mem_upper);
	}
	//判断flag[1]

}
