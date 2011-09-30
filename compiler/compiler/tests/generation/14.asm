.686
include \masm32\include\masm32rt.inc

.data
	g_i dd 0
	g_real1 dd 3.5
	g_str1 db "true" , 0
	g_str2 db "false" , 0
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
start:
	finit
	push 3
	push offset g_i
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push g_i
	fild dword ptr [esp]
	fstp dword ptr [esp]
	push g_real1
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	setbe dl
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