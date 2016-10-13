;###################################################################
;# File Name:irqf.s
;# Author:YFzhang
;# mail:hnnyzyf2013@163.com
;# Created Time:2016年10月12日 星期三 01时37分29秒
;# Last Modified2016年10月12日 星期三 01时37分29秒
;####################################################################


%macro IRQ_NOERRORCODE 2
[GLOBAL irq%1]
irq%1:
	cli			;关中断
	push 0		;插入虚拟错误中断码
	push %2		;push中断号
	jmp irq_common_stub
%endmacro

;定义中断处理函数
IRQ_NOERRORCODE	0,	32
IRQ_NOERRORCODE 1,	33
IRQ_NOERRORCODE 2,	34
IRQ_NOERRORCODE 3,	35
IRQ_NOERRORCODE 4,	36
IRQ_NOERRORCODE 5,	37
IRQ_NOERRORCODE 6,	38
IRQ_NOERRORCODE 7,	39
IRQ_NOERRORCODE 8,	40
IRQ_NOERRORCODE 9,	41
IRQ_NOERRORCODE 10,	42
IRQ_NOERRORCODE 11,	43
IRQ_NOERRORCODE 12,	44
IRQ_NOERRORCODE 13,	45
IRQ_NOERRORCODE 14,	46
IRQ_NOERRORCODE 15,	47


;定义中断服务程序框架
[GLOBAL irq_common_stub]
;声明外部调用
[EXTERN irq_handler]


;中断服务程序
irq_common_stub:
	pusha
	mov ax,ds
	push eax
	
	mov ax,0x10
	mov ds,ax
	mov ss,ax
	mov es,ax
	mov gs,ax
	mov fs,ax
	
	push esp
	call irq_handler

	add esp,4

	pop ebx
	mov ds,bx
	mov ss,bx
	mov es,bx
	mov gs,bx
	mov fs,bx

	popa
	add esp,8
	iret

.end:


















