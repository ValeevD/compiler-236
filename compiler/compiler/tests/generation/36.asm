.686
include \masm32\include\masm32rt.inc

.data
	g_a dd 0
	g_b dd 0
	g_x dd 0
	g_real1 dd 0.00001
	g_str1 db " " , 0
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
f proc
	push ebp
	mov ebp, esp
	sub esp, 0
	push [ebp+8]
	push [ebp+8]
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fmul
	fstp dword ptr [esp]
	push 5
	fild dword ptr [esp]
	fstp dword ptr [esp]
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fsub
	fstp dword ptr [esp]
	mov eax, 12
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	mov esp, ebp
	pop ebp
	ret
f endp

start:
	finit
	push 0
	fild dword ptr [esp]
	fstp dword ptr [esp]
	push offset g_a
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push 5
	fild dword ptr [esp]
	fstp dword ptr [esp]
	push offset g_b
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
label_1:
	push g_b
	push g_a
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fsub
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
	seta dl
	push edx
	pop eax
	cmp eax, 0
	je label_2
	push g_a
	push g_b
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fadd
	fstp dword ptr [esp]
	push 2
	fild dword ptr [esp]
	fstp dword ptr [esp]
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fdiv
	fstp dword ptr [esp]
	push offset g_x
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push g_x
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	push offset g_str1
	call crt_printf
	add esp, 4
	sub esp, 4
	push g_x
	call f
	add esp, 4
	push 0
	fild dword ptr [esp]
	fstp dword ptr [esp]
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	seta dl
	push edx
	pop eax
	cmp eax, 0
	je label_3
	push g_x
	push offset g_b
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	jmp label_4
label_3:
	push g_x
	push offset g_a
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
label_4:
	jmp label_1
label_2:
	INVOKE ExitProcess, 0
end start