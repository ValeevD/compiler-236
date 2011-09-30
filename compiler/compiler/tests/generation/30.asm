.686
include \masm32\include\masm32rt.inc

.data
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
fib proc
	push ebp
	mov ebp, esp
	sub esp, 0
	push [ebp+8]
	push 1
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	sete dl
	push edx
	push [ebp+8]
	push 2
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	sete dl
	push edx
	pop ebx
	pop eax
	or eax, ebx
	push eax
	pop eax
	cmp eax, 0
	je label_1
	push 1
	mov eax, 12
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	jmp label_2
label_1:
	sub esp, 4
	push [ebp+8]
	push 1
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	call fib
	add esp, 4
	sub esp, 4
	push [ebp+8]
	push 2
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	call fib
	add esp, 4
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov eax, 12
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
label_2:
	mov esp, ebp
	pop ebp
	ret
fib endp

start:
	finit
	sub esp, 4
	push 7
	call fib
	add esp, 4
	push offset g_format_int
	call crt_printf
	add esp, 8
	INVOKE ExitProcess, 0
end start