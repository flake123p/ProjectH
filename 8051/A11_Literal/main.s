;
; This example has two warnings:
; *** WARNING L16: UNCALLED SEGMENT, IGNORED FOR OVERLAY PROCESS
;    SEGMENT: ?PR?_FUNC?FUNC
; *** WARNING L10: CANNOT DETERMINE ROOT SEGMENT
;
; 2 Way to fix warning:
;   1.[Target Option] -> [BL51 Misc] -> [Misc controls], add: NOOVERLAY
;   2.[Target Option] -> [BL51 Misc] -> [Overlay], add: * ! ?PR?_FUNC?FUNC
;

PROG SEGMENT CODE

EXTRN CODE (_func)

;Reset vector(0x0000) to MAIN
CSEG AT 0
	JMP MAIN
CSEG AT 3
	JMP DUMMY

;select a generic segment: PROG
RSEG  PROG
DUMMY:
    JMP DUMMY
MAIN:
    INC A
    INC A
	MOV A, #ASM_LIT_EOF
    LCALL _func

ASM_LIT_EOF:
	DB 'EOF'
END