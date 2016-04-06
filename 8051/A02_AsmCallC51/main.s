
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
    LCALL _func

END