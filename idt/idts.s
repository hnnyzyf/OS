;###################################################################
;# File Name:idts.s
;# Author:YFzhang
;# mail:hnnyzyf2013@163.com
;# Created Time:2016年09月30日 星期五 01时28分32秒
;# Last Modified2016年09月30日 星期五 01时28分32秒
;####################################################################


[GLOBAL idt_flush]
idt_flush:
	push ebp
	mov ebp,esp
	mov eax,[esp+8]
	lidt [eax]
	mov esp,ebp
	pop ebp
	ret
