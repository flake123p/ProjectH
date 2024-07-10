; This module defines the entries into Application and contains the interrupt vector
; redirections.



AppBase     CODE   2000H        ; Base address of Application
LibBase     CODE   0080H        ; Base address for Library Entries in BOOT
XDataBase	XDATA   1000H
;PUBLIC v1
;	v1		XDATA 	XDatabase+9
		
		
PUBLIC  app_main                                ; 'main' function of Application (see STARTUP_LITE.A51)
app_main        CODE   AppBase          ; Entry for app_main  is at 0x2000

; This module defines the function entries in APP1. 
; APP1 contains a counter part in the file App1Call.ASM.
PUBLIC  _app_func1              ; APP1 contains the function ´app_func1´
_app_func1   CODE   AppBase+80H ; Entry for app_func1 is at AppBase+0x80

PUBLIC  _app_func2              ; APP1 contains the function ´app_func1´
_app_func2   CODE   AppBase+83H ; Entry for app_func1 is at AppBase+0x80
; In addition it is required to redirect interrupt vectors so that Application may
; use interrupt functions.      In case that you need to share interrupt vectors
; with the boot application refer to the knowledgebase article
; www.keil.com/support/docs/132.htm - GENERAL: REDIRECTING INTERRUPT VECIORS
CSEG AT 3H
        LJMP       AppBase+3H 
CSEG AT 0BH
        LJMP       AppBase+0BH 
CSEG AT 13H
        LJMP       AppBase+13H 
CSEG AT 1BH
        LJMP       AppBase+1BH 
CSEG AT 23H
        LJMP       AppBase+23H 
CSEG AT 2BH
        LJMP       AppBase+2BH 
CSEG AT 33H
        LJMP       AppBase+33H 
CSEG AT 3BH
        LJMP       AppBase+3BH 
CSEG AT 43H
        LJMP       AppBase+43H 
CSEG AT 4BH
        LJMP       AppBase+4BH 
CSEG AT 53H
        LJMP       AppBase+53H 
CSEG AT 5BH
        LJMP       AppBase+5BH 
CSEG AT 63H
        LJMP       AppBase+63H 
CSEG AT 6BH
        LJMP       AppBase+6BH 
CSEG AT 73H
        LJMP       AppBase+73H 
CSEG AT 7BH
        LJMP       AppBase+7BH 

; Entry table for library functions. Define the library functions as needed 
; in your application. Refer to the knowledgebase at www.keil.com/support
; for the names of the library functions.
CSEG AT LibBase
EXTRN CODE (?C?CLDPTR)
EXTRN CODE (?C?CSTPTR)
                LJMP       ?C?CLDPTR
                LJMP       ?C?CSTPTR

;PUBLIC		?C?CLDPTR
;PUBLIC		?C?CSTPTR
;CLDPTR CODE 0x90
;CSTPTR CODE 0x96
; Reserve Memory for Bit space 0x20.0 - 0x27.7 and the registerbanks 1 - 3
; for using within the Application.
;EXTRN XDATA (v1)		
;XSEG    AT		1000H
;	APP1:		DS		10


DSEG    AT 8
                DS      8       ; register bank 1
                DS      8       ; register bank 2
                DS      8       ; register bank 3
                DS      8       ; bit space 0x20.0 - 0x27.7


;PUBLIC  asdff
;PUBLIC system_app_fg
;segbit SEGMENT BIT
;RSEG segbit

;BSEG AT 64
;system_app_fg: DBIT 1
;asdf:      DBIT    1 
;asdff:     DBIT    1 

PUBLIC  aazz
PUBLIC  aazz0
PUBLIC  aazz1
PUBLIC  aazz2
DSEG  AT 0x20
aazz:		DS 1
	aazz0	BIT aazz.0
	aazz1	BIT aazz.1
	aazz2	BIT aazz0+2

PUBLIC	aacc
PUBLIC	aacc0
PUBLIC	aacc1
DATA_R        SEGMENT DATA BITADDRESSABLE
RSEG            DATA_R
aacc:           DS      1
aacc0           BIT     aacc.0        ; Bit 0 in aacc0
aacc1     	    BIT     aacc0+1       ; Bit 1 in aacc0

                END

