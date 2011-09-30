.686
include \masm32\include\masm32rt.inc

.data
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
write_num proc
	push ebp
	mov ebp, esp
	sub esp, 0
	push [ebp+8]
	push offset g_format_int
	call crt_printf
	add esp, 8
	mov esp, ebp
	pop ebp
	ret
write_num endp

start:
	finit
	push 3
	call write_num
	add esp, 4
	INVOKE ExitProcess, 0
end start