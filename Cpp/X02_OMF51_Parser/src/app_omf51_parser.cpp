
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

LibFileIoClass gInputObjFile("zzmain.OBJ", "r+b");

#define BUF_SIZE (65536)
u8  gGlobal_Record_Buffer[BUF_SIZE];
u32 gGlobal_Record_Buffer_Ctr;
u32 gGlobal_Record_Length;
u32 gGlobal_Ctr;

void InitGlobalRecordBuffer(void)
{
	gGlobal_Record_Buffer_Ctr = 0;
	gGlobal_Record_Length = 0;
}

void OMF51_Parser(void)
{
enum {
	S0_Scanning_3_Byte_Header,
	S1_Scanning_Whole_Record,
} 	state = S0_Scanning_3_Byte_Header;
	int ch;
	
	gInputObjFile.FileOpen();
	
	InitGlobalRecordBuffer();
	gGlobal_Ctr = 0;
	printf("OFFSET       REC    LENGTH\n");
	
	while (1) {
		ch = gInputObjFile.GetCharacter();
		BASIC_ASSERT(gGlobal_Record_Buffer_Ctr < BUF_SIZE)
		gGlobal_Record_Buffer[gGlobal_Record_Buffer_Ctr] = ch;
		gGlobal_Record_Buffer_Ctr++;
		gGlobal_Ctr++;
		
		switch (state) {
		case S0_Scanning_3_Byte_Header:
			if (gGlobal_Record_Buffer_Ctr == 3) {
				state = S1_Scanning_Whole_Record;
				gGlobal_Record_Length = (gGlobal_Record_Buffer[2] << 8) | gGlobal_Record_Buffer[1];
			}
		break;

		case S1_Scanning_Whole_Record:
			//DUMPX(gGlobal_Record_Length);
			//ARRAYDUMPX(gGlobal_Record_Buffer, gGlobal_Record_Buffer_Ctr)
			//return;
			//if (gGlobal_Record_Buffer_Ctr == gGlobal_Record_Length)
			if (gGlobal_Record_Buffer_Ctr == gGlobal_Record_Length + 3) {
				//Whole Record Scan "DONE"
				//DUMPX(gGlobal_Record_Buffer[0]);
				printf("0x%08X   0x%02X   0x%04X\n", gGlobal_Ctr-gGlobal_Record_Buffer_Ctr, gGlobal_Record_Buffer[0], gGlobal_Record_Length);
				RecordMgr(gGlobal_Record_Buffer, gGlobal_Record_Buffer_Ctr, gGlobal_Record_Length);
				
				state = S0_Scanning_3_Byte_Header;
				InitGlobalRecordBuffer();
			}
		break;

		default:
			BASIC_ASSERT(0);
		break;
		}
		
		if (ch == EOF) {
			break;
		}
	}
	
	//printf("ss %x %x\n", gInputObjFile.GetCharacter(), gInputObjFile.GetCharacter());
	gInputObjFile.FileClose();
}