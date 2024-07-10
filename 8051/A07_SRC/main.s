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

EXTRN CODE (func)

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
    MOV SP, #0B0h
    MOV A,  #0E4h
    MOV 0F0h, #0E8h
    PUSH 0E0h
    PUSH 0F0h
    INC A
    INC A
    LCALL func

END