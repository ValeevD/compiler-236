.686
include \masm32\include\masm32rt.inc

.data
	g_ar dd 4 dup(0)
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
start:
	finit
	push 1
	push offset g_ar
	push 1
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 8
	pop esi
	add esi, ebx
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
	push 2
	push offset g_ar
	push 1
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 8
	pop esi
	add esi, ebx
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
	push 3
	push offset g_ar
	push 2
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 8
	pop esi
	add esi, ebx
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
	push 4
	push offset g_ar
	push 2
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 8
	pop esi
	add esi, ebx
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
	push offset g_ar
	push 1
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 8
	pop esi
	add esi, ebx
	push esi
	push 1
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_ar
	push 1
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 8
	pop esi
	add esi, ebx
	push esi
	push 2
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_ar
	push 2
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 8
	pop esi
	add esi, ebx
	push esi
	push 1
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_ar
	push 2
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 8
	pop esi
	add esi, ebx
	push esi
	push 2
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	push offset g_format_int
	call crt_printf
	add esp, 8
	INVOKE ExitProcess, 0
end start