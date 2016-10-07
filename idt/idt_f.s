;###################################################################
;# File Name:idt_f.s
;# Author:YFzhang
;# mail:hnnyzyf2013@163.com
;# Created Time:2016年10月07日 星期五 22时53分06秒
;# Last Modified2016年10月07日 星期五 22时53分06秒
;####################################################################

;定义两个构造中断函数宏，分有错误代码和无错误代码
;用与无错误代码的中断
%macro ISR_NOERRORCODE 1
[GLOBAL isr%1]
isr%1:
	cli			;关中断
	push 0		;插入虚拟错误中断码
	push %1		;psuh 中断号
	jmp isr_common_stub
%endmacro

;用于有错误代码的中断
%macro ISR_ERRORCODE 1
[GLOBAL isr%1]
isr%1:
	cli
	push %1
	jmp isr_common_stub
%endmacro

;intel
ISR_NOERRORCODE 0			
ISR_NOERRORCODE 1
ISR_NOERRORCODE 2
ISR_NOERRORCODE 3
ISR_NOERRORCODE 4
ISR_NOERRORCODE 5
ISR_NOERRORCODE 6
ISR_NOERRORCODE 7
ISR_ERRORCODE	8
ISR_NOERRORCODE 9
ISR_ERRORCODE	10
ISR_ERRORCODE	11
ISR_ERRORCODE	12
ISR_ERRORCODE	13
ISR_ERRORCODE	14
ISR_NOERRORCODE 15
ISR_NOERRORCODE 16
ISR_ERRORCODE	17
ISR_NOERRORCODE 18
ISR_NOERRORCODE 19

;保留
ISR_NOERRORCODE 20
ISR_NOERRORCODE 21
ISR_NOERRORCODE 22
ISR_NOERRORCODE 23
ISR_NOERRORCODE 24
ISR_NOERRORCODE 25
ISR_NOERRORCODE 26
ISR_NOERRORCODE 27
ISR_NOERRORCODE 28
ISR_NOERRORCODE 29
ISR_NOERRORCODE 30
ISR_NOERRORCODE 31

;用户自定义
ISR_NOERRORCODE 255



;定义中断服务程序
[GLOBAL isr_common_pub]
;声明调用外部的具体中断处理函数
[EXTERN isr_handler]
;中断服务程序
isr_common_stub:
	pusha				;压入通用寄存器
