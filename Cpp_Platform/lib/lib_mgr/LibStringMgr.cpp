//#include "stdafx.h" //For porting to "Visual Studio"
#include <stdio.h>
#include <stdlib.h>     /* atoi */
#include <string.h>

#include "My_Basics.hpp"

#include "LibString.hpp"

ConstStr::ConstStr(void)
{
	str = (const char *)(0);
}

void ConstStr::SetStr(const char *inStr)
{
	str = inStr;
}

bool LibString_IsCharHex(char ch)
{
	if (ch >= '0' && ch <= '9')
		return true;

	if (ch >= 'A' && ch <= 'F')
		return true;

	if (ch >= 'a' && ch <= 'f')
		return true;

	return false;
}

bool LibString_IsCharNumber(char ch)
{
	if (ch >= '0' && ch <= '9')
		return true;

	return false;
}

bool LibString_IsCharLetter(char ch)
{
	if (ch >= 'A' && ch <= 'Z')
		return true;

	if (ch >= 'a' && ch <= 'z')
		return true;

	return false;
}

bool LibString_IsCharLegal(char ch, const char *legalCharAry, int aryLen)
{
	BASIC_ASSERT(legalCharAry != NULL);
	
	for (int i = 0; i < aryLen; i++) {
		if (ch == legalCharAry[i]) {
			return true;
		}
	}

	return false;
}

bool LibString_IsStringAllLetter(const char *inStr)
{
	if (0 == *inStr) {
		return false;
	}
		
	do {
		if (false == LibString_IsCharLetter(*inStr)) {
			return false;
		}

		inStr++;
	} while (0 != *inStr);

	return true;
}

/*
	A to 0
	B to 1
	...
	Z to 25
	a to 26
	b to 27
	...
	z to 51
	0 to 52
	...
	9 to 61
*/
bool LibString_CharToIndex(IN char ch, IN bool doAddingInLowercase, OUT int *index)
{
	if (ch >= 'A' && ch <= 'Z') {
		*index = ch - 'A';
	} else if (ch >= 'a' && ch <= 'z') {
		*index = ch - 'a';
		if (doAddingInLowercase) {
			*index = *index + 26;
		}
	} else if (ch >= '0' && ch <= '9') {
		*index = ch - '0' + 52;
	} else {
		return false;
	}
	BASIC_ASSERT(*index < 62);
	return true;
}

//Max return value = maxLength
int LibString_HexStringToCharString(const char *srcString, char *dstString, int maxLength)
{
	int retVal;
	unsigned int offset = 0;
	int numberOfChar = 0;
	unsigned int tempVal;

	//To next word
	while(1) {
		if (srcString[offset] == '\t' || srcString[offset] == ' ') {
			offset++;
		} else {
			break;
		}
	}

	while(1) {
		retVal = sscanf(srcString+offset, "%x", &tempVal);
		if (retVal == 0)
			break;
			
		dstString[numberOfChar] = (char)tempVal;
		numberOfChar++;
		if (numberOfChar >= maxLength)
			break;
			
		//Skip proccessed word
		while(1) {
			if (srcString[offset] == 0)
				goto END;
			if (LibString_IsCharHex(srcString[offset]))
				offset++;
			else
				break;
		}
		//To next word
		while(1) {
			if (srcString[offset] == 0)
				goto END;
			if (!LibString_IsCharHex(srcString[offset]))
				offset++;
			else
				break;
		}
	}
END:
	return numberOfChar;
}

void LibString_2D_HexStringToCharString(char *srcString[], char *dstString, int maxLength)
{
	unsigned int temp;
	
	for (int i = 0; i < maxLength; i++) {
		sscanf(srcString[i], "%x", &temp);
		dstString[i] = (char)temp;
	}
}

int LibString_DecStringToInt(const char * str)
{
	// double atof (const char* str);            //Convert string to double
	// int atoi (const char * str);              //Convert string to integer
	// long int atol ( const char * str );       //Convert string to long integer
	// long long int atoll ( const char * str ); //Convert string to long long integer
	return atoi(str);
}

int LibString_HexStringToInt(const char * str)
{
	int retVal;

	sscanf(str, "%x", &retVal);

	return retVal;
}

/*
	Return Value
		destination is returned.

	num default is -1
*/
char * LibString_Copy(char * destination, const char * source, int num /* = -1 */)
{
	if (num < 0) {
		return strcpy(destination, source);
	} else {
		/*
		Copies the first num characters of source to destination. 
		If the end of the source C string (which is signaled by a null-character) is found before num characters have been copied.
		*/
		return strncpy(destination, source, (size_t)num);
	}
}

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

CTRL_CHAR_ATTRIB_t *LibString_ControlChar_TableGet(OUT u32 *length)
{
	*length = gCtrlCharAttribTable_Len;
	
	return gCtrlCharAttribTable;
}

void LibString_DumpControlChar(void)
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

void LibString_DumpPrintableChar(void)
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
