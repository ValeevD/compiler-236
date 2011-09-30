.686
include \masm32\include\masm32rt.inc

.data
	g_i dd 0
	g_str1 db " " , 0
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
start:
	finit
	push 0
	push offset g_i
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
label_1:
	push g_i
	push 2
	pop ebx
	pop eax
	add eax, ebx
	push eax
	push offset g_i
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push g_i
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str1
	call crt_printf
	add esp, 4
	push g_i
	push 50
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	sete dl
	push edx
	pop eax
	cmp eax, 0
	je label_1
	INVOKE ExitProcess, 0
end start