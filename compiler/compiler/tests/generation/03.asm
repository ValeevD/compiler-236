.686
include \masm32\include\masm32rt.inc

.data
	g_i dd 0
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
	push 2
	fild dword ptr [esp]
	fstp dword ptr [esp]
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fdiv
	fstp dword ptr [esp]
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	INVOKE ExitProcess, 0
end start