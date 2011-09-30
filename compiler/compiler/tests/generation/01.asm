.686
include \masm32\include\masm32rt.inc

.data
	g_real1 dd 2.3
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
start:
	finit
	push g_real1
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	INVOKE ExitProcess, 0
end start