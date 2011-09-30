.686
include \masm32\include\masm32rt.inc

.data
	g_str1 db "input-output" , 0
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
.code
start:
	push offset g_str1
	call crt_printf
	add esp, 4
end start