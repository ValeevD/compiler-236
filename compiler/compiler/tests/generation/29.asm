.686
include \masm32\include\masm32rt.inc

.data
	g_arr dd 10 dup(0)
	g_q dd 0
	g_str1 db " " , 0
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
qsort proc
	push ebp
	mov ebp, esp
	sub esp, 16
	push [ebp+8]
	mov eax, -4
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push [ebp+12]
	mov eax, -8
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push offset g_arr
	push [ebp+8]
	push [ebp+12]
	pop ebx
	pop eax
	add eax, ebx
	push eax
	push 2
	pop ebx
	pop eax
	xor edx, edx
	idiv ebx
	push eax
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	mov eax, -12
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
label_3:
label_4:
	push offset g_arr
	push [ebp-4]
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	push [ebp-12]
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	setl dl
	push edx
	pop eax
	cmp eax, 0
	je label_5
	push [ebp-4]
	push 1
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov eax, -4
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	jmp label_4
label_5:
label_6:
	push offset g_arr
	push [ebp-8]
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	push [ebp-12]
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	setg dl
	push edx
	pop eax
	cmp eax, 0
	je label_7
	push [ebp-8]
	push 1
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	mov eax, -8
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	jmp label_6
label_7:
	push [ebp-4]
	push [ebp-8]
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	setle dl
	push edx
	pop eax
	cmp eax, 0
	je label_8
	push offset g_arr
	push [ebp-4]
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	mov eax, -16
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push offset g_arr
	push [ebp-8]
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	push offset g_arr
	push [ebp-4]
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push esi
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push [ebp-16]
	push offset g_arr
	push [ebp-8]
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push esi
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push [ebp-4]
	push 1
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov eax, -4
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push [ebp-8]
	push 1
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	mov eax, -8
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
label_8:
	push [ebp-4]
	push [ebp-8]
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	setg dl
	push edx
	pop eax
	cmp eax, 0
	je label_3
	push [ebp+8]
	push [ebp-8]
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	setl dl
	push edx
	pop eax
	cmp eax, 0
	je label_10
	push [ebp-8]
	push [ebp+8]
	call qsort
	add esp, 8
label_10:
	push [ebp-4]
	push [ebp+12]
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	setl dl
	push edx
	pop eax
	cmp eax, 0
	je label_12
	push [ebp+12]
	push [ebp-4]
	call qsort
	add esp, 8
label_12:
	mov esp, ebp
	pop ebp
	ret
qsort endp

start:
	finit
	push 1
	push offset g_q
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push 10
	pop ebx
	push g_q
	pop ecx
label_1:
	cmp ecx, ebx
	jg label_2
	push offset g_q
	pop eax
	mov dword ptr [eax], ecx
	push ebx
	push ecx
	push g_q
	push g_q
	pop ebx
	pop eax
	xor edx, edx
	imul ebx
	push eax
	push g_q
	pop ebx
	pop eax
	xor edx, edx
	imul ebx
	push eax
	push 23
	pop ebx
	pop eax
	xor edx, edx
	idiv ebx
	push eax
	push offset g_arr
	push g_q
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push esi
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	pop ecx
	pop ebx
	inc ecx
	jmp label_1
label_2:
	push 10
	push 1
	call qsort
	add esp, 8
	push 1
	push offset g_q
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push 10
	pop ebx
	push g_q
	pop ecx
label_14:
	cmp ecx, ebx
	jg label_15
	push offset g_q
	pop eax
	mov dword ptr [eax], ecx
	push ebx
	push ecx
	push offset g_arr
	push g_q
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	push offset g_format_int
	call crt_printf
	add esp, 8
	push offset g_str1
	call crt_printf
	add esp, 4
	pop ecx
	pop ebx
	inc ecx
	jmp label_14
label_15:
	INVOKE ExitProcess, 0
end start