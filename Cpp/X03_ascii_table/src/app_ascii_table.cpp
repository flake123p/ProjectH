
#include <stdio.h>
#include <string.h>

#include "My_Basics.hpp"

#include "LibUart.hpp"
#include "LibTime.hpp"
#include "LibFileIO.hpp"
#include "LibUtility.hpp"
#include "LibOS.hpp"
#include "LibTui.hpp"
#include "LibSysCmd.hpp"
#include "LibScreenLogic.hpp"
#include "LibArgParser.hpp"

#include "app_ascii_table.hpp"

typedef struct {
	u32 index;
	const char *abbreviation;
	const char *caretNotation;
	const char *escapeSequecesInC;
	const char *description;
} CTRL_CHAR_ATTRIB_t;

CTRL_CHAR_ATTRIB_t gCtrlCharAttribTable[] = {
	{0,   "NUL",  "^@",   "\\0",  "Null"},
	{1,   "SOH",  "^A",   " ",    "Start of Heading"},
	{2,   "STX",  "^B",   " ",    "Start of Text"},
	{3,   "ETX",  "^C",   " ",    "End of Text"},
	{4,   "EOT",  "^D",   " ",    "End of Transmission (Not the same as ETB)"},
	{5,   "ENQ",  "^E",   " ",    "Enquiry"},
	{6,   "ACK",  "^F",   " ",    "Acknowledgement"},
	{7,   "BEL",  "^G",   "\\a",  "Bell - Caused teletype machines to ring a bell. Causes a beep in many common terminals and terminal emulation programs."},
	{8,   "BS",   "^H",   "\\b",  "Backspace - Moves the cursor (or print head) move backwards (left) one space."},
	{9,   "HT",   "^I",   "\\t",  "Horizontal Tab - Moves the cursor (or print head) right to the next tab stop. The spacing of tab stops is dependent on the output device, but is often either 8 or 10."},
	{10,  "LF",   "^J",   "\\n",  "Line Feed (NL line feed, new line) - Moves the cursor (or print head) to a new line. On Unix systems, moves to a new line AND all the way to the left."},
	{11,  "VT",   "^K",   "\\v",  "Vertical Tab"},
	{12,  "FF",   "^L",   "\\f",  "Form Feed - Advances paper to the top of the next page (if the output device is a printer)."},
	{13,  "CR",   "^M",   "\\r",  "Carriage Return - Moves the cursor all the way to the left, but does not advance to the next line."},
	{14,  "SO",   "^N",   " ",    "Shift Out - Switches output device to alternate character set."},
	{15,  "SI",   "^O",   " ",    "Shift In - Switches output device back to default character set."},
	{16,  "DLE",  "^P",   " ",    "Data Link Escape"},
	{17,  "DC1",  "^Q",   " ",    "Device Control 1 (Often XON)"},
	{18,  "DC2",  "^R",   " ",    "Device Control 2"},
	{19,  "DC3",  "^S",   " ",    "Device Control 3 (Often XOFF)"},
	{20,  "DC4",  "^T",   " ",    "Device Control 4"},
	{21,  "NAK",  "^U",   " ",    "Negative Acknowledgement"},
	{22,  "SYN",  "^V",   " ",    "Synchronous Idle"},
	{23,  "ETB",  "^W",   " ",    "End of Transmission Block (Not the same as EOT)"},
	{24,  "CAN",  "^X",   " ",    "Cancel"},
	{25,  "EM",   "^Y",   " ",    "End of Medium"},
	{26,  "SUB",  "^Z",   " ",    "Substitute"},
	{27,  "ESC",  "^[",   "\\e",  "Escape"},
	{28,  "FS",   "^\\",  " ",    "File Separator"},
	{29,  "GS",   "^]",   " ",    "Group Separator"},
	{30,  "RS",   "^^",   " ",    "Record Separator (^^ means Ctrl+^ (pressing the \"Ctrl\" and caret keys).)"},
	{31,  "US",   "^_",   " ",    "Unit Separator"},
	{127, "DEL",  "^?",   " ",    "Delete"},
};
u32 gCtrlCharAttribTable_Len = sizeof(gCtrlCharAttribTable)/sizeof(CTRL_CHAR_ATTRIB_t);

void DumpControlCharTable(void)
{
	printf("================================================================================\n");
	printf("[[[ CONTROL CHARACTERS ]]]: ( 0 ~ 31, and 127 )\n\n");
	printf("   [DEC] [HEX] [Abbrev] [Caret] [Escape] [Description]\n");
	for (u32 i = 0; i < gCtrlCharAttribTable_Len; i++) {
		printf("--> %-3d   0x%02X %5s   %5s   %6s     %s\n",
			gCtrlCharAttribTable[i].index,
			gCtrlCharAttribTable[i].index,
			gCtrlCharAttribTable[i].abbreviation,
			gCtrlCharAttribTable[i].caretNotation,
			gCtrlCharAttribTable[i].escapeSequecesInC,
			gCtrlCharAttribTable[i].description);
		if (i==7 || i==9 || i==10 || i==12 || i==13) {
			printf("\n");
		}
	}
	printf("\n");
}

void DumpPrintableCharTable(void)
{
	printf("================================================================================\n");
	printf("[[[ PRINTABLE CHARACTERS ]]] ( 32 ~ 126 ):\n\n");
	printf("   [DEC] [HEX] [Glyph]       [DEC] [HEX] [Glyph]       [DEC] [HEX] [Glyph]\n\n");
	for (u8 i = 32; i < 64; i++) {
		if (i == 32) {
			//First row
			printf("--> %-3d  0x%02X  (space)    --> %-3d  0x%02X     %c       --> %-3d  0x%02X     %c   \n", i, i, i+32, i+32, i+32, i+64, i+64, i+64);
		} else if (i == 63) {
			//Last row
			printf("--> %-3d  0x%02X     %c       --> %-3d  0x%02X     %c\n", i, i, i, i+32, i+32, i+32);
		} else {
			//Other rows
			printf("--> %-3d  0x%02X     %c       --> %-3d  0x%02X     %c       --> %-3d  0x%02X     %c   \n", i, i, i, i+32, i+32, i+32, i+64, i+64, i+64);
		}
	}

	printf("\n");
}

void PrintUsage(void)
{
	printf("Usage:\n");
	printf("  atbl -[options]\n");
	printf("Options:\n");
	printf("  -a : Print all.\n");
	printf("  -c : Print control characters.\n");
	printf("  -p : Print printable characters.\n");
}

int AppAsciiTable(int argc, char *argv[])
{
	LibArgParser_ClearCharOption();
	LibArgParser_CharOptionParserEx(argc, argv, "acp");

	if (NOT(LibArgParser_DoesCharOptionExist())) {
		PrintUsage();
		return 1;
	}
	
	if (LibArgParser_CheckCharOption('a')) {
		DumpControlCharTable();
		DumpPrintableCharTable();
		return 0;
	}
	if (LibArgParser_CheckCharOption('c')) {
		DumpControlCharTable();
		//no return here
	}

	if (LibArgParser_CheckCharOption('p')) {
		DumpPrintableCharTable();
		//no return here
	}

	return 0;
}
