.686
include \masm32\include\masm32rt.inc

.data
	g_i dd 0
	g_j dd 0
	g_real1 dd 2.0
	g_real2 dd 3.0
	g_str1 db " " , 0
	g_str2 db " " , 0
	g_str3 db " " , 0
	g_str4 db " " , 0
	g_str5 db " " , 0
	g_str6 db " " , 0
	g_str7 db " " , 0
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
	push g_real2
	push offset g_j
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push g_i
	push g_j
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fadd
	fstp dword ptr [esp]
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	push offset g_str1
	call crt_printf
	add esp, 4
	push g_i
	push g_j
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fsub
	fstp dword ptr [esp]
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	push offset g_str2
	call crt_printf
	add esp, 4
	push g_i
	push g_j
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fmul
	fstp dword ptr [esp]
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	push offset g_str3
	call crt_printf
	add esp, 4
	push g_j
	push g_i
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
	push offset g_str4
	call crt_printf
	add esp, 4
	push 2
	fild dword ptr [esp]
	fstp dword ptr [esp]
	push g_i
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fadd
	fstp dword ptr [esp]
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	push offset g_str5
	call crt_printf
	add esp, 4
	push 2
	fild dword ptr [esp]
	fstp dword ptr [esp]
	push g_i
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fsub
	fstp dword ptr [esp]
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	push offset g_str6
	call crt_printf
	add esp, 4
	push 2
	fild dword ptr [esp]
	fstp dword ptr [esp]
	push g_i
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fmul
	fstp dword ptr [esp]
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	push offset g_str7
	call crt_printf
	add esp, 4
	push 2
	fild dword ptr [esp]
	fstp dword ptr [esp]
	push g_i
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