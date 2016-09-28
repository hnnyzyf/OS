;###################################################################
;# File Name:gdt.s
;# Author:YFzhang
;# mail:hnnyzyf2013@163.com
;# Created Time:2016年09月28日 星期三 01时40分06秒
;# Last Modified2016年09月28日 星期三 01时40分06秒
;####################################################################

[GLOBAL gdt_flush]
gdt_flush:
		;push ebp
		;mov ebp,esp
		mov eax,[esp+4]	;参数存入eax寄存器
		lgdt [eax]			;加载到GDTR
		
		mov ax,0x10			;加载数据段描述符
		mov ds,ax				;更新所有可以更新的寄存器
		mov es,ax				
		mov fs,ax
		mov gs,ax
		mov ss,ax
		jmp 0x08:.flush			;远程跳转，0x08是代码段描述符
								;清空流水线并串行化处理器
.flush:
		;mov esp,ebp
		;pop ebp
		ret
