;###################################################################
;# File Name:irqf.s
;# Author:YFzhang
;# mail:hnnyzyf2013@163.com
;# Created Time:2016年10月12日 星期三 01时37分29秒
;# Last Modified2016年10月12日 星期三 01时37分29秒
;####################################################################


%marco IRQ_NOERRORCODE 2
[GLOBAL irq%1]
irq%1:
	cli			;关中断
	push 0		;插入虚拟错误中断码
	push %2		;push中断号
	jump irq_common_stub
%endmacro

;定义中断处理函数
IRO_NOERRORCODE 0	32
IRO_NOERRORCODE 1	33
IRO_NOERRORCODE 2	34
IRO_NOERRORCODE 3	35
IRO_NOERRORCODE 4	36
IRO_NOERRORCODE 5	37
IRO_NOERRORCODE 6	38
IRO_NOERRORCODE 7	39
IRO_NOERRORCODE 8	40
IRO_NOERRORCODE 9	41
IRO_NOERRORCODE 10	42
IRO_NOERRORCODE 11	43
IRO_NOERRORCODE 12	44
IRO_NOERRORCODE 13	45
IRO_NOERRORCODE 14	46
IRO_NOERRORCODE 15	47


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
	call irq_common_stub

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


















