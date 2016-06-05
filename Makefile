###################################################################
# File name:Makefile
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年06月06日 星期一 04时39分50秒
# Last Modified:2016年06月06日 星期一 06时26分32秒
####################################################################


#!Makefile
###########shell命令获得当前目录下的所有c文件
C_SOURCES=$(shell find . -name "*.c")
###########patsubst命令获得 将C_SOURCES中后缀为.c文件全部变为.o
C_OBJECTS=$(patsubst %.c,%.o,$(C_SOURCES))
S_SOURCES=$(shell find . -name "*.s")
S_OBJECTS=$(patsubst %.s,%.o,$(S_SOURCES))

#C语言编译器
CC=gcc
#链接器
LD=ld
#汇编编译器
ASM=nasm

#C语言编译器的编译选项
#-c 只执行到编译，输出目标文件。
#-W，在编译中开启一些额外的警告（warning）信息。-Wall，将所有的警告信息全开。
#-m32生成32位机器的汇编代码
#-ggdb 此选项将尽可能的生成gdb的可以使用的调试信息
#-gstabs+ 此选项以stabs格式声称调试信 息,并且包含仅供gdb使用的额外调试信息
#nostdinc 使 编译器不再系统缺省的头文件目录里面找头文件,一般和-I联合使用,明确限定头 文件的位置
#-fno-builtin不使用C语言的内建函数
#-fno-stack-protector 禁用栈溢出保护机制
#-I 指定目录include
C_FLAGS=-c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include


#LD链接器的链接选项

LD_FLAGS=-T tools/kernel.ld -m elf_i386 -nostdlib
ASM_FLAGS=-f elf -g -F stabs

all:$(S_OBJECTS) $(C_OBJECTS) link update_image

.c.o:
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

.s.o:
	@echo 编译汇编文件 $< ...
	$(ASM) $(ASM_FLAGS) $<
link:
	@echo 编译内核文件..
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o hx_kernel

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) hx_kernel

.PHONY:update_image
update_image:
	sudo mount floppy.img /mnt/kernel
	sudo cp hx_kernel /mnt/lernel/hx_kernel
	sleep 1
	sudo umount /mnt/kernel

.PHONY:mount_image
mout_image:
	sudo mount floppy.img /mnt/kernel

.PHONY:umount_image
umount_image:
	sudo umount /mnt/kernel

.PHONY:qemu
qemu:
	qemu -fda floppy.img -boot a

.PHONY:bochs
bochs:
	bochs -f tools/bochsrc.txt

.PHONY:debug
debug:
	qemu -S -s -fda floppy.img -boot a &
	sleep 1 
	cgbd -x tools/gdbinit
	
	














