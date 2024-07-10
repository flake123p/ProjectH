; This module defines the function entries in APP1 and BOOT. 
; BOOT contains a counter part in the file App1Entry.ASM.

EXTRN CODE (_app_func1)
EXTRN CODE (_app_func2)
//EXTRN XDATA (v1)
//PUBLIC v1
AppBase     CODE   2000H        ; Base address of Application
LibBase     CODE   0080H        ; Base address for Library Entries in BOOT
		
; This module defines an additional function entry in APP1. 
CSEG AT AppBase+80H
           LJMP  _app_func1
CSEG AT AppBase+83H		   
           LJMP  _app_func2
; Entry table for library functions
;CSEG AT LibBase
;PUBLIC ?C?CLDPTR
;PUBLIC ?C?CSTPTR
EXTRN CODE (?C?CLDPTR)
EXTRN CODE (?C?CSTPTR)
;?C?CLDPTR  CODE LibBase+0 
;?C?CSTPTR  CODE LibBase+3

//	XSEG  AT 0x1009
//v1:     DS    2

		END

