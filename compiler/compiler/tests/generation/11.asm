.686
include \masm32\include\masm32rt.inc

.data
	g_rec dd 4 dup(0)
	g_real1 dd 3.4
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
start:
	finit
	push 3
	push offset g_rec
	pop esi
	add esi, 0
	push esi
	push 1
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push esi
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push 5
	push offset g_rec
	pop esi
	add esi, 0
	push esi
	push 2
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push esi
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push offset g_rec
	pop esi
	add esi, 0
	push esi
	push 1
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	push 5
	pop ebx
	pop eax
	add eax, ebx
	push eax
	push offset g_rec
	pop esi
	add esi, 8
	push esi
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push offset g_rec
	pop esi
	add esi, 8
	push dword ptr [esi]
	fild dword ptr [esp]
	fstp dword ptr [esp]
	push g_real1
	fld dword ptr [esp+4]
	fld dword ptr [esp]
	add esp, 4
	fadd
	fstp dword ptr [esp]
	push offset g_rec
	pop esi
	add esi, 12
	push esi
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push offset g_rec
	pop esi
	add esi, 0
	push esi
	push 1
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	push offset g_rec
	pop esi
	add esi, 0
	push esi
	push 2
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	pop ebx
	pop eax
	add eax, ebx
	push eax
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_rec
	pop esi
	add esi, 8
	push dword ptr [esi]
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_rec
	pop esi
	add esi, 12
	push dword ptr [esi]
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	INVOKE ExitProcess, 0
end start