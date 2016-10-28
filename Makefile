###################################################################
# File name:Makefile
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年06月06日 星期一 04时39分50秒
# Last Modified:2016年06月05日 星期日 23时16分23秒
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
#-T tools/kernel.ld 读链接文件
#-m elf_i386 指定
# -nostdlib 仅搜索那些在命令行上显式指定的库路径. 在连接脚本中(包含在命令行上指定的连接脚本)指定的库路径都被忽略.
LD_FLAGS=-T script/kernel.ld -m elf_i386 -nostdlib

#汇编编译器NASM的编译选项
#-f elf  编译成elf格式的文件
#-g 该选项可用来在指定格式的输出文件中产生调试信息
#-F 该选项可以用来为输出文件选择一个调试格式
ASM_FLAGS=-f elf -g -F stabs


#all这个伪目标是所有目标的目标，其功能一般是编译所有的目标。
all:$(S_OBJECTS) $(C_OBJECTS) link update_image




#$<表示所有的依赖目标集
#$@表示所有的目标集
#".c.o"是双后缀规则，意义就是".c"是源文件的后缀，".o"是目标文件的后缀
.c.o:
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

.s.o:
	@echo 编译汇编文件 $< ...
	$(ASM) $(ASM_FLAGS) $<

link:
	@echo 编译内核文件..
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o mykernel


.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) mykernel

.PHONY:update_image
update_image:
	sudo mount floppy.img /mnt/kernel -o loop
	sudo cp mykernel /mnt/kernel/mykernel
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
	qemu -fda floppy.img -boot a -curses

.PHONY:bochs
bochs:
	bochs -f script/bochsrc.txt

.PHONY:debug
debug:
	qemu -S -s -fda floppy.img -boot a -curses &
	sleep 2 
	gdb -x script/gdbinit
	
	














