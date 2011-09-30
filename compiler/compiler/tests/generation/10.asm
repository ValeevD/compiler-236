.686
include \masm32\include\masm32rt.inc

.data
	g_rec dd 2 dup(0)
	g_real1 dd 3.4
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
start:
	finit
	push 2
	push offset g_rec
	pop esi
	add esi, 0
	push esi
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push g_real1
	push offset g_rec
	pop esi
	add esi, 4
	push esi
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push offset g_rec
	pop esi
	add esi, 0
	push dword ptr [esi]
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_rec
	pop esi
	add esi, 4
	push dword ptr [esi]
	fld dword ptr [esp]
	fstp qword ptr [esp]
	push offset g_format_real
	call crt_printf
	add esp, 12
	INVOKE ExitProcess, 0
end start