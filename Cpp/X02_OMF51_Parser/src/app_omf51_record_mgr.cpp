
#include <stdio.h>
#include <string.h>

#include "LibUart.hpp"
#include "LibTime.hpp"
#include "LibFileIO.hpp"
#include "LibUtility.hpp"
#include "LibOS.hpp"
#include "LibTui.hpp"
#include "LibSysCmd.hpp"
#include "My_Basics.hpp"
#include "app_omf51_parser.hpp"

typedef void(*RECORD_CALLBACK_t)(u8 *buf, u32 record_length);

typedef struct {
	u8 rec_type;
	RECORD_CALLBACK_t callback;
}RECORD_TABLE_t;

void CB02_ModuleHeader(u8 *buf, u32 record_length)
{
	/*
		Byte 3: Contains a number between 0 and 40, inclusive, that indicates the number of remaining bytes
		Byte 4...: The remaining bytes are interpreted as a byte string; each byte must represent a member of 
		           the following subset of the ASCII character set:  {upper case letters (A..Z), decimal digits (0..9), 
		           the special characters "_", "?" and "@"}.

		Byte -1: Check Sum
		Byte -3: The byte identifies the program which has generated this module:  0FDH - ASM51, 0FEH - PL/M-51, 0FFH - RL51.
	*/
	BASIC_ASSERT(buf[3] <= 40);

	printf("MODULE \"");
	for (u8 i = 0; i < buf[3]; i++) {
		printf("%c", buf[4+i]);
	}
	printf("\" -- ");

	switch(buf[3+buf[3]+1]) {
	case 0xFD:
		printf("from ASM51\n");
	break;

	case 0xFE:
		printf("from PL/M-51\n");
	break;

	case 0xFF:
		printf("from RL51\n");
	break;

	default:
		BASIC_ASSERT(0);
	break;
	}
	
	//DUMPX(buf[0]);
	//ARRAYDUMPX(buf, record_length+3);
}

void CBXX_DUMMY(u8 *buf, u32 record_length)
{
	//DUMPX(buf[0]);
}

RECORD_TABLE_t gRecordTable[] = {
	{0x02, CB02_ModuleHeader},
	{0x04, CBXX_DUMMY},
	{0xFF, CBXX_DUMMY},
};
u32 gRecordTableSize = sizeof(gRecordTable) / sizeof(RECORD_TABLE_t);

void RecordMgr(u8 *buf, u32 buf_length, u32 record_length)
{
	u32 i;
	for (i = 0; i < gRecordTableSize; i++) {
		if (gRecordTable[i].rec_type == buf[0]) {
			if (gRecordTable[i].callback != NULL) {
				(*gRecordTable[i].callback)(buf, record_length);
			}
			return;
		}
	}
	if (i == gRecordTableSize) {
		//CBXX_DUMMY(buf, record_length);
	}
}
