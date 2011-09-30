.686
include \masm32\include\masm32rt.inc

.data
	g_j dd 0
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
p proc
	push ebp
	mov ebp, esp
	sub esp, 0
	push 3
	push 3
	pop ebx
	pop eax
	xor edx, edx
	imul ebx
	push eax
	push 3
	pop ebx
	pop eax
	xor edx, edx
	imul ebx
	push eax
	push [ebp+8]
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	mov esp, ebp
	pop ebp
	ret
p endp

start:
	finit
	push offset g_j
	call p
	add esp, 4
	push g_j
	push 3
	pop ebx
	pop eax
	add eax, ebx
	push eax
	push offset g_format_int
	call crt_printf
	add esp, 8
	INVOKE ExitProcess, 0
end start