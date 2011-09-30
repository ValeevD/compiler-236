.686
include \masm32\include\masm32rt.inc

.data
	g_ar1 dd 5 dup(0)
	g_ar2 dd 5 dup(0)
	g_i dd 0
	g_str1 db " " , 0
	g_format_int db "%d" , 0
	g_format_real db "%f" , 0
	g_new_line db "\n" , 0
.code
sort proc
	push ebp
	mov ebp, esp
	sub esp, 12
	push 1
	mov eax, -4
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push 5
	pop ebx
	push [ebp-4]
	pop ecx
label_3:
	cmp ecx, ebx
	jg label_4
	mov eax, -4
	add eax, ebp
	push eax
	pop eax
	mov dword ptr [eax], ecx
	push ebx
	push ecx
	push [ebp-4]
	mov eax, -8
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push 5
	pop ebx
	push [ebp-8]
	pop ecx
label_5:
	cmp ecx, ebx
	jg label_6
	mov eax, -8
	add eax, ebp
	push eax
	pop eax
	mov dword ptr [eax], ecx
	push ebx
	push ecx
	mov eax, 8
	add eax, ebp
	push eax
	push [ebp-4]
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	mov eax, 8
	add eax, ebp
	push eax
	push [ebp-8]
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push dword ptr [esi]
	xor edx, edx
	pop ebx
	pop eax
	cmp eax, ebx
	setg dl
	push edx
	pop eax
	cmp eax, 0
	je label_7
	mov eax, 8
	add eax, ebp
	push eax
	push [ebp-8]
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push esi
	mov eax, 8
	add eax, ebp
	push eax
	push [ebp-4]
	pop ebx
	mov eax, 1
	sub ebx, eax
	imul ebx, 4
	pop esi
	add esi, ebx
	push esi
	call swap
	add esp, 8
label_7:
	pop ecx
	pop ebx
	inc ecx
	jmp label_5
label_6:
	pop ecx
	pop ebx
	inc ecx
	jmp label_3
label_4:
	sub esp, 20
	mov esi, 8
	add esi, ebp
	mov edi, esp
	mov ecx, 5
	rep movsd
	mov eax, 28
	add eax, ebp
	push eax
	pop edi
	mov esi, esp
	push ecx
	mov ecx, 5
	rep movsd
	pop ecx
	add esp, 20
	mov esp, ebp
	pop ebp
	ret
sort endp

swap proc
	push ebp
	mov ebp, esp
	sub esp, 4
	mov eax, [ebp+8]
	push [eax]
	mov eax, -4
	add eax, ebp
	push eax
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	mov eax, [ebp+12]
	push [eax]
	push [ebp+8]
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push [ebp-4]
	push [ebp+12]
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	mov esp, ebp
	pop ebp
	ret
swap endp

start:
	finit
	push 1
	push offset g_i
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push 5
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
	push g_i
	push g_i
	pop ebx
	pop eax
	xor edx, edx
	imul ebx
	push eax
	push 3
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	push 25
	pop ebx
	pop eax
	xor edx, edx
	idiv ebx
	push edx
	push 3
	push g_i
	pop ebx
	pop eax
	xor edx, edx
	imul ebx
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	push 4
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	push offset g_ar1
	push g_i
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
	sub esp, 20
	sub esp, 20
	mov esi, offset g_ar1
	mov edi, esp
	mov ecx, 5
	rep movsd
	call sort
	add esp, 20
	push offset g_ar2
	pop edi
	mov esi, esp
	push ecx
	mov ecx, 5
	rep movsd
	pop ecx
	add esp, 20
	push 1
	push offset g_i
	pop ebx
	pop eax
	mov dword ptr [ebx], eax
	push 5
	pop ebx
	push g_i
	pop ecx
label_9:
	cmp ecx, ebx
	jg label_10
	push offset g_i
	pop eax
	mov dword ptr [eax], ecx
	push ebx
	push ecx
	push offset g_ar2
	push g_i
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
	jmp label_9
label_10:
	INVOKE ExitProcess, 0
end start