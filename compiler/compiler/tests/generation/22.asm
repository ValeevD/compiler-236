.686
include \masm32\include\masm32rt.inc

.data
	g_i dd 0
	g_j dd 0
	g_str1 db " " , 0
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
start:
	finit
	push 1
	push offset g_i
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push 50
	pop ebx
	push g_i
	pop ecx
label_1:
	cmp ecx, ebx
	jg label_2
	push offset g_i
	pop eax
	mov dword ptr [eax], ecx
	push ebx
	push ecx
	push 100
	push offset g_j
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
label_3:
	push g_j
	push g_i
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	setne dl
	push edx
	pop eax
	cmp eax, 0
	je label_4
	push g_j
	push 1
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	push offset g_j
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	jmp label_3
label_4:
	push g_j
	push 2
	pop ebx
	pop eax
	xor edx, edx
	idiv ebx
	push edx
	push 0
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	sete dl
	push edx
	pop eax
	cmp eax, 0
	je label_5
	push g_j
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str1
	call crt_printf
	add esp, 4
label_5:
	pop ecx
	pop ebx
	inc ecx
	jmp label_1
label_2:
	INVOKE ExitProcess, 0
end start