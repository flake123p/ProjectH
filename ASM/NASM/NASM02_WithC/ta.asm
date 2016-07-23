BITS 32 ; Tell NASM we're using 32-bit protected mode.

global	_myfunc
global	_myfunc2
extern	_printf

section .data
sum_result: 
	dw 0

section .text
_myfunc:
	push    ebp             ; create stack frame
	mov     ebp, esp

	mov     eax, [ebp+8]    ; grab the first argument
	mov     ecx, [ebp+12]   ; grab the second argument
	add     eax, ecx        ; sum the arguments
	mov		[sum_result], eax
	
	push	message         ; Print example
	call	_printf
	add     esp, 4          ; c / c++ function convention
	
	mov		eax, [sum_result]
	pop     ebp             ; restore the base pointer
	ret
message:
	db	'Hello, this string is from assembly~~', 10, 0
_myfunc2:
	push    ebp             ; create stack frame
	mov     ebp, esp

	mov     eax, [ebp+8]    ; grab the first argument
	mov     ecx, [ebp+12]   ; grab the second argument
	sub     eax, ecx        ; sub the arguments

	pop     ebp             ; restore the base pointer
	ret
