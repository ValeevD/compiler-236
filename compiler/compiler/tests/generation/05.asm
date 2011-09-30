.686
include \masm32\include\masm32rt.inc

.data
	g_i dd 0
	g_real1 dd 3.4
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
start:
	finit
	push g_real1
	push offset g_i
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push g_i
	fld dword ptr [esp]
	fchs
	fstp dword ptr [esp]
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	INVOKE ExitProcess, 0
end start