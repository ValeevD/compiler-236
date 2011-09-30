.686
include \masm32\include\masm32rt.inc

.data
	g_i dd 0
	g_real1 dd 3.4
	g_real2 dd 3.4
	g_real3 dd 3.4
	g_str1 db "fasle" , 0
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
start:
	finit
	push g_real1
	fld dword ptr [esp]
	fchs
	fstp dword ptr [esp]
	push offset g_i
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push g_i
	push g_real2
	fld dword ptr [esp]
	fchs
	fstp dword ptr [esp]
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	sete dl
	push edx
	pop eax
	cmp eax, 0
	je label_1
	push 1
	push 2
	pop ebx
	pop eax
	add eax, ebx
	push eax
	push 3
	pop ebx
	pop eax
	add eax, ebx
	push eax
	fild dword ptr [esp]
	fstp dword ptr [esp]
	push g_real3
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fadd
	fstp dword ptr [esp]
	push g_i
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fadd
	fstp dword ptr [esp]
	push offset g_i
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push g_i
	push 6
	fild dword ptr [esp]
	fstp dword ptr [esp]
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fsub
	fstp dword ptr [esp]
	push 1
	fild dword ptr [esp]
	fstp dword ptr [esp]
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fadd
	fstp dword ptr [esp]
	push offset g_i
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push g_i
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	jmp label_2
label_1:
	push offset g_str1
	call crt_printf
	add esp, 4
label_2:
	INVOKE ExitProcess, 0
end start