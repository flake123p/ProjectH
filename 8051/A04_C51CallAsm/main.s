;
; Call Asm from C:
;   1. "PUBLIC _ASM_FUNC"
;   2. Write code in tag "_ASM_FUNC:"
;   3. Declair function prototype: "unsigned char ASM_FUNC(unsigned char arg0);" in C file.
;   4. Underscore prefix name is in assembly, NOT in C file.
;

EXTRN CODE (_func)
PUBLIC _ASM_FUNC

PROG SEGMENT CODE

;Reset vector(0x0000) to MAIN
CSEG AT 0
	JMP MAIN
CSEG AT 3
	JMP DUMMY

;select a generic segment: PROG
RSEG  PROG
DUMMY:
    JMP DUMMY
_ASM_FUNC:
    MOV A, R7
    ADD A, #5
    MOV R7, A
    RET
MAIN:
    INC A
    INC ACC
    MOV R7, #6
    LCALL _func


END