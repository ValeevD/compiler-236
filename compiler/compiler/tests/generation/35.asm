.686
include \masm32\include\masm32rt.inc

.data
	g_q dd 11 dup(0)
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
p proc
	push ebp
	mov ebp, esp
	sub esp, 0
	push [ebp+8]
	pop esi
	add esi, 0
	push dword ptr [esi]
	push [ebp+8]
	pop esi
	add esi, 4
	push esi
	push 3
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
	mov esp, ebp
	pop ebp
	ret
p endp

start:
	finit
	push 99
	fild dword ptr [esp]
	fstp dword ptr [esp]
	push offset g_q
	pop esi
	add esi, 0
	push esi
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push offset g_q
	call p
	add esp, 4
	push offset g_q
	pop esi
	add esi, 4
	push esi
	push 3
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	INVOKE ExitProcess, 0
end start