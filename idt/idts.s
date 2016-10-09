;###################################################################
;# File Name:idts.s
;# Author:YFzhang
;# mail:hnnyzyf2013@163.com
;# Created Time:2016年09月30日 星期五 01时28分32秒
;# Last Modified2016年09月30日 星期五 01时28分32秒
;####################################################################


[GLOBAL idt_flush]
idt_flush:
	mov eax,[esp+4]
	lidt [eax]
	ret
.end
