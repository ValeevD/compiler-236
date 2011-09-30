.686
include \masm32\include\masm32rt.inc

.data
	g_i dd 0
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
.code
start:
	push 2
	push offset g_i
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push g_i
	push offset g_format_int
	call crt_printf
	add esp, 8
end start