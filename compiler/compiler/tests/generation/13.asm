.686
include \masm32\include\masm32rt.inc

.data
	g_str1 db "false" , 0
	g_str2 db "true" , 0
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
start:
	finit
	push 1
	push 2
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	setg dl
	push edx
	pop eax
	cmp eax, 0
	je label_1
	push offset g_str1
	call crt_printf
	add esp, 4
	jmp label_2
label_1:
	push offset g_str2
	call crt_printf
	add esp, 4
label_2:
	INVOKE ExitProcess, 0
end start