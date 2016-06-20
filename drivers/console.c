/*###########################################
# File Name:console.c
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:Mon 20 Jun 2016 07:37:51 PM CST
# Last ModifiedMon 20 Jun 2016 07:37:51 PM CST
##########################################*/

//VGA的显示缓冲的起点是0xb8000

#include "types.h"
#include "console.h"

static uint16_t *video_memory=(uint16_t *)0xB8000;

//屏幕光标的坐标
static uint8_t cursor_x=0;
static uint8_t cursor_y=0;




//屏幕输入光标的移动
static void move_cursor()
{
	//屏幕是80字宽
	uint16_t cursorLocation=cursor_y*80+cursor_x;
	//在这里用到两个内部寄存器的编号，分别表示光标位置1415的低位与高位
	//告诉VGA我们要设置光标的高字节
	outb(0x3D4,14);				
	//发送高8位
	outb(0x3D5,cursorLocation>>8);
	//告诉VGA我们要设置光标的低字节
	outb(0x3D4,15);
	//发送低8位
	outb(0x3D5,cursorLocation);
}

//清屏操作
//用白底黑字的空格符覆盖整个区域
void console_clear()
{
	uint8_t attribute_byte=(0<<4)|(15&0x0F);
	uint16_t blank=0x20|(attribute_byte<<8);

	int i;
	for(i=0;i<80*25;i++)
	{
		video_memory[i]=blank;
	}
	cursor_x=0;
	cursor_y=0;
	move_cursor();
}

//屏幕滚动显示

static void scroll()
{
	//先定义一个8位格式，attribute_byte 被构造出一个黑底白字的描述格式
	uint8_t attribute_byte=(0<<4)|(15&0x0F);
	//space是0x20
	uint16_t blank=0x20|(attribute_byte<<8);

	//cursor_y是25的时候，就该换行了
	if(cursor_y>=25)
	{
		//将所有行的现实火速据复制到上一行，第一行永远的消失了
		int i;
		//前24行每个值等于后边地第80个数据
		for(i=0;i<24*80;i++)
		{
			video_memory[i]=video_memory[i+80];
		}
		//第25行的数据全部置为空格
		for(i=24*80;i<25*80;i++)
		{
			video_memory[i]=blank;
		}
		cursor_y=24;
	}
}



//屏幕输出一个字符带颜色
void console_putc_color(char c,real_color_t back,real_color_t fore)
{
	//先构造出后8位显示颜色
	//先将back数据变为8位，然后左移4位，然后fore变为8为，直接与
	uint8_t attribute_byte=(((back&(0x0F)))<<4)|(fore&(0x0F));
	//构造需要输出的字符
	//高8位为颜色控制，低8位为字符，其实就是内码asicc码
	uint16_t attribute=(attribute_byte<<8);
	//接下来开始判断c的内容，根据内容来判断要实现的操作
	//如果是退格符
	if(c==0x88 &&cursor_x!=0)
	{	
		//如果该退回上一行
		cursor_x--;
	}
	//如果是tab键,向后平移8位
	else if(c==0x09)
	{
		if(cursor_x<=71)
		{
			cursor_x=cursor_x+8;
		}
		else
		{
			cursor_x=79;
		}
	}
	//如果是换行
	else if(c=='\n')
	{
		cursor_x=0;
		cursor_y++
	}
	//如果是回车
	else if(c=='r')
	{
		cursor_x=0;
		cursor_y++;
	}
	//如果是各种字符,则需要赋值
	else if(c>=32 && c<=126)
	{
		video_memory[cursor_y*80+cursor_x]=c|attribute;
		cursor_x++;
	}

	//判断cursor_x是否应该换行了
	if(cursor_x>=80)
	{
		cursor_x=0;
		cursor_y++;
	}
	//判断是否换行
	scroll();
	//移动cursor
	move_cursor();

}


//打印一个字符串
void console_write(char *cstr)
{
	char *temp=*cstr;
	//cstr不等于零
	while(temp!='\0')
	{
		console_putc_color(*temp,0,15);
		*temp=*temp+1;
	}
}

//打印一个带颜色的字符串
void console_write_color(char *cstr,real_color_t back,real_color_t fore)
{
	char *temp=cstr;
	//cstr不等于零
	while(temp!='\0')
	{
		console_putc_color(*temp,back,fore);
		*temp=*temp+1;
	}
}

//输出十六进制的整型数
void console_write_hex(uint32_t n,real_color_t back,real_color_t fore)
{
	//就是输出0xdad这样一个字符串
	//先输出0x
	console_write("0x");
	uint32_t temp;
	//分别输出4位,从高到低分别移位
	for(i=1;i<9;i++)
	{
		temp=n;
		temp=(temp>>((8-i)*4));
		temp=temp&(0xF);
		//判断是0-9还是a-f
		if(temp<0xA)
		{
			console_putc_color(temp+'0',back,fore);
		}
		else
		{
			console_putc_color(temp- 0xA+'a',back,fore);
		}
	}
}


//屏幕输出一个10进制的整型数
void console_write_dec(uint32_t n,real_color_t back,real_color_t fore)
{
	//n本身为一个十进制的整型数
	//需要获取每一个位置的个数
	uint32_t temp=n;
	char str[32];
	int i=0;
	//初始化
	for(i=0;i<32;i++)
	{
		char str[i]=' ';
	}
	while(temp!=0)
	{
		i--;
		//最后一位为除以10的余数
		str[i]=temp%10;
		//缩小十倍
		temp=temp/10;
	}
	for(i;i<32;i++)
	{
		console_putc_color(str[i]+'0',back,fore);
	}
}






