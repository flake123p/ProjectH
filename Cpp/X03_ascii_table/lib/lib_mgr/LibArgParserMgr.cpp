//#include "stdafx.h" //For porting to "Visual Studio"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "My_Basics.hpp"

#include "LibString.hpp"
#include "LibArgParser.hpp"

static bool gDoesCharOptionExist = false;
/*
	'A' = 0x41 (65)
	'a' = 0x61 (97)
*/
static int  gCharOption[52 + 10] = {0};

/*
	return true if option exists.
*/
static bool _CharOptionParser(char *str, const char *legalCharAry, int aryLen)
{
	// Skip 1st char, which is '-'
	str++;

	/*
	if (false == LibString_IsStringAllLetter(str)) {
		return false;
	}
	*/

	int index;
	do {
		if (legalCharAry != NULL) {
			if (false == LibString_IsCharLegal(*str, legalCharAry, aryLen)) {
				gDoesCharOptionExist = false;
				return false;
			}
		}
		
		LibString_CharToIndex(*str, true, &index);

		gCharOption[index] = 1;
		gDoesCharOptionExist = true;
		str++;
	} while (0 != *str);

	return true;
}

void LibArgParser_ClearCharOption(void)
{
	gDoesCharOptionExist = false;
	memset(gCharOption, 0, 52 * sizeof(int));
}

/*
	return true if option exists.
*/
bool LibArgParser_CharOptionParser(int argc, char *argv[], const char *legalCharAry /* = NULL */, int aryLen /* = 0 */)
{
	bool doesOptionExist = false;
	
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			doesOptionExist = _CharOptionParser(argv[i], legalCharAry, aryLen);
			if (false == doesOptionExist) {
				return false;
			}
		}
	}

	return true;
}

bool LibArgParser_CharOptionParserEx(int argc, char *argv[], const char *legalCharAry)
{
	int aryLen = strlen(legalCharAry);

	return LibArgParser_CharOptionParser(argc, argv, legalCharAry, aryLen);
}

bool LibArgParser_CheckCharOption(char ch)
{
	if (false == gDoesCharOptionExist) {
		return false;
	}

	int index;
	if (false == LibString_CharToIndex(ch, true, &index)) {
		return false;
	}

	if (gCharOption[index])
		return true;
	else
		return false;
}

bool LibArgParser_DoesCharOptionExist(void)
{
	return gDoesCharOptionExist;
}

bool LibArgParser_ArgvsToVariables(ARG_TYPE_DB_t *db_buf, u32 count, char *argv[], u32 argv_start_index, OUT u32 *error_index)
{
	int retVal;
	char *strPtr;
	u32  *u32Ptr;
	u32 temp; //for sscanf
	u32 cur_index;
	bool doSscanf;
	const char *sscanfType;
	
	for (u32 i = 0; i < count; i++) {
		cur_index = argv_start_index + i;
		strPtr = (char *)(db_buf[i].ptr);
		u32Ptr = (u32 *)(db_buf[i].ptr);
		doSscanf = false;
		
		switch (db_buf[i].type) {
			case ARG_TYPE_STRING:
				LibString_Copy(strPtr, argv[cur_index]);
			break;
			
			case ARG_TYPE_DEC:
				sscanfType = "%d";
				doSscanf = true;
			break;

			case ARG_TYPE_HEX_WITH_0x:
				sscanfType = "0x%x";
				doSscanf = true;
			break;

			case ARG_TYPE_HEX_WITHOUT_0x:
				sscanfType = "%x";
				doSscanf = true;
			break;

			default:
				BASIC_ASSERT(0);
			break;
		}

		if (doSscanf) {
			retVal = sscanf(argv[cur_index], sscanfType, &temp);
			if (retVal != 1) {
				*error_index = cur_index;
				return false;
			} else {
				*u32Ptr = temp;
			}
		}
	}
	
	return true;
}
