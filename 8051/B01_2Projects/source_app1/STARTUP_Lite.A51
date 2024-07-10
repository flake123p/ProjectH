$NOMOD51
;------------------------------------------------------------------------------
;  This file is part of the C51 Compiler package
;  Copyright (c) 2006 Keil Elektronik GmbH and Keil Software, Inc.
;------------------------------------------------------------------------------
;  STARTUP_Lite.A51:  This code is called from the Boot Loader to startup
;                     the application program
;------------------------------------------------------------------------------
                NAME    ?C_STARTUP


?C_C51STARTUP   SEGMENT   CODE
                EXTRN CODE (?C_START)
                PUBLIC  ?C_STARTUP

                CSEG    AT      0x2000
?C_STARTUP:     LJMP    ?C_START
                END
