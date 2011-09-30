.686
include \masm32\include\masm32rt.inc

.data
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
p proc
	push ebp
	mov ebp, esp
	sub esp, 0
	push [ebp+8]
	mov eax, 12
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	mov esp, ebp
	pop ebp
	ret
	push 100000
	mov eax, 12
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	mov esp, ebp
	pop ebp
	ret
p endp

start:
	finit
	sub esp, 4
	push 5
	call p
	add esp, 4
	push offset g_format_int
	call crt_printf
	add esp, 8
	INVOKE ExitProcess, 0
end start