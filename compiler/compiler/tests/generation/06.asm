.686
include \masm32\include\masm32rt.inc

.data
	g_real1 dd 2.4
	g_real2 dd 3.4
	g_str1 db " " , 0
	g_real3 dd 3.4
	g_real4 dd 2.4
	g_str2 db " " , 0
	g_real5 dd 2.4
	g_real6 dd 3.4
	g_str3 db " " , 0
	g_real7 dd 3.4
	g_real8 dd 2.4
	g_str4 db " " , 0
	g_real9 dd 2.4
	g_real10 dd 3.4
	g_str5 db " " , 0
	g_real11 dd 3.4
	g_real12 dd 2.4
	g_str6 db " " , 0
	g_real13 dd 2.4
	g_real14 dd 2.4
	g_str7 db " " , 0
	g_real15 dd 2.4
	g_real16 dd 3.4
	g_str8 db " " , 0
	g_real17 dd 3.4
	g_real18 dd 2.4
	g_str9 db " " , 0
	g_real19 dd 2.4
	g_real20 dd 2.4
	g_str10 db " " , 0
	g_real21 dd 2.4
	g_real22 dd 3.4
	g_str11 db " " , 0
	g_real23 dd 3.4
	g_real24 dd 3.4
	g_str12 db " " , 0
	g_real25 dd 2.4
	g_real26 dd 3.4
	g_str13 db " " , 0
	g_real27 dd 3.4
	g_real28 dd 3.4
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
start:
	finit
	push g_real1
	push g_real2
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	seta dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str1
	call crt_printf
	add esp, 4
	push g_real3
	push g_real4
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	seta dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str2
	call crt_printf
	add esp, 4
	push g_real5
	push g_real6
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	setb dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str3
	call crt_printf
	add esp, 4
	push g_real7
	push g_real8
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	setb dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str4
	call crt_printf
	add esp, 4
	push g_real9
	push g_real10
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	setae dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str5
	call crt_printf
	add esp, 4
	push g_real11
	push g_real12
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	setae dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str6
	call crt_printf
	add esp, 4
	push g_real13
	push g_real14
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	setae dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str7
	call crt_printf
	add esp, 4
	push g_real15
	push g_real16
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	setbe dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str8
	call crt_printf
	add esp, 4
	push g_real17
	push g_real18
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	setbe dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str9
	call crt_printf
	add esp, 4
	push g_real19
	push g_real20
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	setbe dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str10
	call crt_printf
	add esp, 4
	push g_real21
	push g_real22
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	sete dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str11
	call crt_printf
	add esp, 4
	push g_real23
	push g_real24
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	sete dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str12
	call crt_printf
	add esp, 4
	push g_real25
	push g_real26
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	setne dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str13
	call crt_printf
	add esp, 4
	push g_real27
	push g_real28
	xor edx, edx
	fld dword ptr [esp]
	add esp, 4
	fld dword ptr [esp]
	add esp, 4
	fcompp
	fstsw ax
	sahf
	setne dl
	push edx
	push offset g_format_int
	call crt_printf
	add esp, 8
	INVOKE ExitProcess, 0
end start