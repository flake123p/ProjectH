
#include "Everything_Lib_Mgr.hpp"

ConstStr::ConstStr(void)
{
	str = (const char *)(0);
}

void ConstStr::SetStr(const char *inStr)
{
	str = inStr;
}

bool LibString_IsCharHexadecimal(char ch, OUT u32 *val /* = NULL */)
{
	if (ch >= '0' && ch <= '9') {
		if (val != NULL)
			*val = ch - '0';
		return true;
	}
	
	if (ch >= 'A' && ch <= 'F') {
		if (val != NULL)
			*val = ch - 'A' + 10;
		return true;
	}
	
	if (ch >= 'a' && ch <= 'f') {
		if (val != NULL)
			*val = ch - 'a' + 10;
		return true;
	}
	
	return false;
}

bool LibString_IsCharDecimal(char ch, OUT u32 *val /* = NULL */)
{
	if (ch >= '0' && ch <= '9') {
		if (val != NULL)
			*val = ch - '0';
		return true;
	}
	
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

bool LibString_IsCharPrintable(char ch)
{
	if (ch >= 32 && ch <= 126)
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

bool LibString_IsStringNumerical(const char *inStr)
{
    size_t i = 0;
    size_t len = strlen(inStr);

    if (len >= 3) {
        if (inStr[0] == '0') {
            if (inStr[1] == 'x' || inStr[1] == 'X') {
                //continue
                i = 2;
            } else {
                return false;
            }
        }
    }
    for (; i < len; i++) {
        if (LibString_IsCharHexadecimal(inStr[i])) {
            //continue
        } else {
            return false;
        }
    }

    return true;
}

bool LibString_IsCharNonspace(char ch)
{
	if (ch == '\t' || ch == '\n' ||ch == ' ') {
		return false;
	} else {
		return true;
	}
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
int LibString_HexStringToCharString(u8 *dstString, const char *srcString, int maxLength)
{
	int retVal;
	unsigned int offset = 0;
	int numberOfChar = 0;
	unsigned int tempVal;

	//To next word
	while(1) {
		if (!LibString_IsCharHexadecimal(srcString[offset])) {
			offset++;
		} else {
			break;
		}
	}

	while(1) {
		retVal = sscanf(srcString+offset, "%x", &tempVal);
		if (retVal == 0)
			break;
			
		dstString[numberOfChar] = (u8)tempVal;
		numberOfChar++;
		if (numberOfChar >= maxLength)
			break;
			
		//Skip proccessed word
		while(1) {
			if (srcString[offset] == 0)
				goto END;
			if (LibString_IsCharHexadecimal(srcString[offset]))
				offset++;
			else
				break;
		}
		//To next word
		while(1) {
			if (srcString[offset] == 0)
				goto END;
			if (!LibString_IsCharHexadecimal(srcString[offset]))
				offset++;
			else
				break;
		}
	}
END:
	return numberOfChar;
}

//Max return value = maxLength
int LibString_DecStringToCharString(u8 *dstString, const char *srcString, int maxLength)
{
	int retVal;
	unsigned int offset = 0;
	int numberOfChar = 0;
	unsigned int tempVal;

	//To next word
	while(1) {
		if (!LibString_IsCharDecimal(srcString[offset])) {
			offset++;
		} else {
			break;
		}
	}

	while(1) {
		retVal = sscanf(srcString+offset, "%d", &tempVal);
		if (retVal == 0)
			break;
			
		dstString[numberOfChar] = (u8)tempVal;
		numberOfChar++;
		if (numberOfChar >= maxLength)
			break;
			
		//Skip proccessed word
		while(1) {
			if (srcString[offset] == 0)
				goto END;
			if (LibString_IsCharDecimal(srcString[offset]))
				offset++;
			else
				break;
		}
		//To next word
		while(1) {
			if (srcString[offset] == 0)
				goto END;
			if (!LibString_IsCharDecimal(srcString[offset]))
				offset++;
			else
				break;
		}
	}
END:
	return numberOfChar;
}

void LibString_2D_HexStringToCharString(u8 *dstString, char *srcString[], int scanCount)
{
	unsigned int temp;
	
	for (int i = 0; i < scanCount; i++) {
		sscanf(srcString[i], "%x", &temp);
		dstString[i] = (u8)temp;
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
	if (strlen(str) >= 2) {
		if (str[0] == '0') {
			if (str[1] == 'x' || str[1] == 'X') {
				str += 2;
			}
		}
	}
	
	int retVal;

	sscanf(str, "%x", &retVal);

	return retVal;
}

u32 LibString_HexStringToU32(const char * str)
{
	if (strlen(str) >= 2) {
		if (str[0] == '0') {
			if (str[1] == 'x' || str[1] == 'X') {
				str += 2;
			}
		}
	}
	
	u32 retVal;

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
		/*
		for (int i = 0; i < num; i++) {
		    destination[i] = source[i];
		}
		return 0;
		*/
		return strncpy(destination, source, (size_t)num);
	}
}

u8 LibString_GetCheckSumU8(u8 *srcAry, u32 srcAryLen)
{
	u32 tempU32 = 0;
	for (u32 i = 0; i < srcAryLen; i++) {
		tempU32 += srcAry[i];
	}
	tempU32 = 0x100 - tempU32;

	return (u8)tempU32;
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

u32 LibString_GetDeciOrHex(std::string *in)
{
    u32 retVal;
    int isHex = 0;
    int isUpperCaseX = 0;

    if (in->size() >= 3) {
        if ((*in)[0] == '0') {
            if ((*in)[1] == 'x') {
                isHex = 1;
            } else if ((*in)[1] == 'X') {
                isHex = 1;
                isUpperCaseX = 1;
            }
        }
    }

    if (isHex) {
        if (isUpperCaseX) {
            sscanf(in->c_str(), "0X%x", &retVal);
        } else {
            sscanf(in->c_str(), "0x%x", &retVal);
        }
    } else {
        sscanf(in->c_str(), "%u", &retVal);
    }

    return retVal;
}

int LibString_GetDeciOrHex32(std::string *in, u32 *out)
{
    u32 retVal;
    int isHex = 0;
    int isUpperCaseX = 0;

    if (in->size() >= 3) {
        if ((*in)[0] == '0') {
            if ((*in)[1] == 'x') {
                isHex = 1;
            } else if ((*in)[1] == 'X') {
                isHex = 1;
                isUpperCaseX = 1;
            }
        }
    }

    for (size_t i  = 0; i < in->size(); i ++) {
        if (i == 1) {
            if ( ((*in)[i] == 'x') || ((*in)[i] == 'X') ) {
                if ((*in)[0] == '0') {
                    continue;
                } else {
                    return 0;
                }
            }
        }
        if ( LibString_IsCharHexadecimal((*in)[i])  == false ) {
            return 0;
        }
    }
    if (isHex) {
        if (isUpperCaseX) {
            sscanf(in->c_str(), "0X%x", &retVal);
        } else {
            sscanf(in->c_str(), "0x%x", &retVal);
        }
    } else {
        sscanf(in->c_str(), "%u", &retVal);
    }

    return 1;
}


//return true for array index parsed
int LibString_IsArrayPattern(std::string *in, std::string *outAryName, u32 *outStartIdx, u32 *outEndIdx)
{
    size_t inStrSize = in->size();
    *outAryName = *in;
    *outStartIdx = 0;
    *outEndIdx = 0;

    if(inStrSize <= 3) {
        return 0;
    }

    if ((*in)[0] == '[') {
        return 0;
    }

    if ((*in)[inStrSize-1] != ']') {
        return 0;
    } else {
        int state = 0; //0 for name, 1 for start, 2 for end
        std::string startIdxStr = "";
        std::string endIdxStr = "";
        *outAryName = "";
        for (size_t i=0; i<inStrSize-1; i++) {
            if ((*in)[i] == '[') {
                state = 1;
                continue;
            }
            if ((*in)[i] == '-') {
                state = 2;
                continue;
            }
            if (state == 0) { //0 for saving var string
                *outAryName += (*in)[i];
            } else if (state == 1) {
                startIdxStr += (*in)[i];
            } else if (state == 2) {
                endIdxStr += (*in)[i];
            }
        }
        *outStartIdx = LibString_GetDeciOrHex(&startIdxStr);
        *outEndIdx = *outStartIdx;
        if (state  == 2) {
            if (endIdxStr == "e" || endIdxStr == "E") {
                *outEndIdx = 0xFFFFFFFF;
            } else {
                *outEndIdx = LibString_GetDeciOrHex(&endIdxStr);
            }
        }
    }

    return 1;
}

int LibString_IsArrayPattern2(std::string *in, std::string *outAryName, std::string *outIndexStr)
{
    size_t inStrSize = in->size();
    *outAryName = *in;

    if ((*in)[inStrSize-1] != ']') {
        return 0;
    }

    if(inStrSize <= 3) {
        return 0;
    }

    if ((*in)[0] == '[') {
        return 0;
    }

    int state = 0; //0 for name, 1 for index
    *outAryName = "";
    *outIndexStr = "";
    for (size_t i=0; i<inStrSize-1; i++) {
        if ((*in)[i] == '[') {
            state = 1;
            continue;
        }
        if ((*in)[i] == ' ') {
            continue;
        }
        if (state == 0) {
            *outAryName += (*in)[i];
        } else if (state == 1) {
            *outIndexStr += (*in)[i];
        }
    }

    if (outIndexStr->size() == 0) {
        return 0;
    }

    return 1;
}


int LibString_IsPrintPattern(std::string *in, int *isSigned /*= NULL*/)
{
    int isFound = 0;
    std::size_t foundPosi = in->find('%');

    if (isSigned != NULL) {
        *isSigned = 0;
    }

    if (foundPosi == std::string::npos) {
        isFound = 0;
    } else {
        if (foundPosi == in->size()-1) {
            isFound = 0;
        } else {
            isFound = 1;
            if ((*in)[foundPosi+1] == 'd') {
                if (isSigned != NULL) {
                    *isSigned = 1;
                }
            }
        }
    }

    return isFound;
}

LibStringClass::LibStringClass(const char *cString /* = NULL */)
{
	if (cString != NULL)
		Init(cString);
}

void LibStringClass::Init(char *cString)
{
	str = cString;
	subStrVector.clear();
}

void LibStringClass::Init(const char *cString)
{
	str = cString;
	subStrVector.clear();
}

void LibStringClass::Init(std::string inStr)
{
	this->str = inStr;
	subStrVector.clear();
}

size_t LibStringClass::Length(void) //Same result as Size()
{
	return str.length();
}

size_t LibStringClass::Size(void) //Same result as Length()
{
	return str.size();
}

bool LibStringClass::FindChar(char ch, size_t pos /* = 0 */, OUT u32 *result_position /* = NULL */)
{
	size_t retPos = str.find(ch, pos);

	if (retPos != std::string::npos) {
		if (result_position != NULL) {
			*result_position = (u32)retPos;
		}
		return true;
	} else {
		return false;
	}
}

bool LibStringClass::FindString(const char *s, size_t pos /* = 0 */, OUT u32 *result_position /* = NULL */)
{
	size_t retPos = str.find(s, pos);

	if (retPos != std::string::npos) {
		if (result_position != NULL) {
			*result_position = (u32)retPos;
		}
		return true;
	} else {
		return false;
	}
}

int LibStringClass::Split(bool checkDoubleQuote /* = false */)
{
	u32 string_length = str.size();
	LibBufferBasic bufferObj;
	bufferObj.Init(string_length+1);
	char *buf = (char *)bufferObj.bufPtr;
	std::string bufString;
	const char *src = str.c_str();

	subStrVector.clear();
	subStrFlagVector.clear();
	
	for (u32 i = 0; i < string_length; ) {
		//DUMPD(i);
		if (LibString_IsCharNonspace(src[i])) {
			if (checkDoubleQuote && src[i] == '\"') {
				u32 j;
				u32 x = 0;
				buf[0] = 0;
				for (j=i+1; j < string_length; j++) {
					if (src[j] != '\"') {
						buf[x] = src[j];
						x++;
					} else {
						break;
					}
				}
				buf[x] = 0;
				bufString = buf;
				subStrVector.push_back(bufString);
				subStrFlagVector.push_back(LIB_STR_SUB_DOUBLE_QUOTE);
				i = j+1;
			} else {
				sscanf(&(src[i]), "%s", buf);
				bufString = buf;
				subStrVector.push_back(bufString);
				subStrFlagVector.push_back(0);
				i+=strlen(buf);
			}
		} else {
			i++;
		}
	}

	return 0;
}

/*
	Turn:
		"01 7d fc [??] [00 20 02 00] 80"
	Into:
		"01 7d fc [05] [00 20 02 00] 80"
	By:
		ReplaceWithRestLength("??");
*/
int LibStringClass::ReplaceWithRestLength(const char *s)
{
	if (FindString(s)) {
		//PRINT_LINE(1);
		Split();

		std::string tempString;
		for (u32 i = 0; i < subStrVector.size(); i++) {
			if (subStrVector[i].find(s) != std::string::npos) {
				char temp[10];
				sprintf(temp, "%02X  ", (u32)(subStrVector.size()-i-1));
				tempString += temp;
			} else {
				tempString += subStrVector[i];
				tempString += "  ";
			}
		}
		//DUMPS(tempString.c_str());
		str = tempString;
	} else {
		//PRINT_LINE(1);
	}

	//Dump();
	
	return 0;
}

/*
	Turn:
		"01 7d fc [??-1] [00 20 02 00] 80"
	Into:
		"01 7d fc [04]   [00 20 02 00] 80"
	By:
		ReplaceWithRestLength("??-1");
*/
int LibStringClass::ReplaceWithRestLengthEx(const char *s)
{
	if (FindString(s)) {
		//PRINT_LINE(1);
		Split();

		std::string tempString;
		for (u32 i = 0; i < subStrVector.size(); i++) {
			if (subStrVector[i].find(s) != std::string::npos) {
				s32 offset = 0;
				LibStringClass inStr = LibStringClass(subStrVector[i].c_str());
				inStr.InsertBefore("+", " ");
				inStr.InsertAfter("+", " ");
				inStr.InsertBefore("-", " ");
				inStr.InsertAfter("-", " ");
				inStr.Split();
				if (inStr.subStrVector.size() == 3) {
					sscanf(inStr.subStrVector[2].c_str(), "%d", &offset);
					if (0 == inStr.subStrVector[1].compare("+")) {
						//do nothing
					} else if (0 == inStr.subStrVector[1].compare("-")) {
						offset = 0 - offset;
					} else {
						BASIC_ASSERT(0);
					}
				}

				{
					char temp[10];
					sprintf(temp, "%02X  ", (u32)(subStrVector.size()-i-1+offset));
					tempString += temp;
				}
			} else {
				tempString += subStrVector[i];
				tempString += "  ";
			}
		}
		//DUMPS(tempString.c_str());
		str = tempString;
	} else {
		//PRINT_LINE(1);
	}

	//Dump();
	
	return 0;
}

int LibStringClass::RemoveRestStringByChar(char ch)
{
    return RemoveExtension(ch);
}

int LibStringClass::RemoveExtension(char ch)
{
	std::size_t pos = str.find(ch);

	if (pos == std::string::npos)
		return 0;

	str = str.substr(0, pos);

	return 0;
}

int LibStringClass::ReplaceExtension(char ch, const char *s)
{
	RemoveExtension(ch);

	str = str + s;
	
	return 0;
}

int LibStringClass::ReplaceSubString(const char *toStr, const char *fromStr)
{
	u32 loop = 0;
	u32 findStartPos = 0;
	for (size_t findPos = str.find(toStr); findPos != std::string::npos; findPos = str.find(toStr, findStartPos)) {
		str.replace(findPos, strlen(fromStr), fromStr);

		findStartPos += findPos + strlen(fromStr);

		loop++;
		if (loop >= 100) {
			BASIC_ASSERT(0);
		}
	}
	
	return 0;
}

int LibStringClass::ToUpperCase(void)
{
	std::string newStr;

	std::locale settings;
	std::string converted;

	for(u32 i = 0; i < str.size(); ++i)
		newStr += (std::toupper(str[i], settings));

	str = newStr;

	return 0;
}

/*
	Turn:
		"123 456 //789"
	Into:
		"123 456 "
	By:
		RemoveRestString("//");
*/
int LibStringClass::RemoveRestString(const char *s)
{
	std::size_t pos = str.find(s);

	if (pos == std::string::npos)
		return 0;

	str = str.substr(0, pos);

	return 0;
}

int LibStringClass::RemoveEmptyPrefixChar(void)
{
	while (1) {
		if (0 == str.size())
			break;

		if (str[0] == ' ') {
			str.erase(0, 1);
		} else if (str[0] == '\t') {
			str.erase(0, 1);
		} else {
			break;
		}
	}

	return 0;
}

int LibStringClass::RemoveEmptyPostfixChar(void)
{
	size_t len;
	
	do {
		len = this->Size();
		if (0 == len)
			break;

		if (str[len-1] == ' ') {
			str.erase(len-1);
			continue;
		}

		if (str[len-1] == '\t') {
			str.erase(len-1);
			continue;
		}

	} while (false);

	return 0;
}

/*
	Turn:
		"a=b"
	Into:
		"axx=b"
	By:
		InsertBefore("=", "xx");
*/
int LibStringClass::InsertBefore(const char *pattern, const char *s)
{
	std::size_t pos = str.find(pattern);

	if (pos == std::string::npos)
		return 0;

	str = str.insert(pos, s);

	return 0;
}

/*
	Turn:
		"a=b"
	Into:
		"a=xxb"
	By:
		InsertAfter("=", "xx");
*/
int LibStringClass::InsertAfter(const char *pattern, const char *s)
{
	std::size_t pos = str.find(pattern);

	if (pos == std::string::npos)
		return 0;

	str = str.insert(pos + strlen(pattern), s);

	return 0;
}

const char* LibStringClass::BitsStringOfOneByte_LittleEndian(u8 byte, u32 bits_size)
{
    str = "";
    BASIC_ASSERT(bits_size>=1&&bits_size<=8);
    for (u32 i=0; i<bits_size; i++) {
        if (byte & 0x01)
            str = "1" + str;
        else
            str = "0" + str;
        byte=byte>>1;
    }
    return str.c_str();
}

const char * LibStringClass::CStr(void)
{
	return str.c_str();
}

void LibStringClass::Dump(void)
{
	printf("str = %s\n", str.c_str());
	printf("length = %d\n", (u32)Length());
	printf("Size of sub-string = %d\n", (u32)subStrVector.size());
	for (u32 i = 0; i < subStrVector.size(); i++) {
		printf("(0x%08X) subStr[%d] = %s\n", subStrFlagVector[i], i, subStrVector[i].c_str());
	}
}

void LibString_Demo(void)
{
	char testString1[] = "01 7d fc [??] xx [00 20 02 00] 80";
	LibStringClass stringObj;
	stringObj.Init(testString1);
	stringObj.ReplaceWithRestLength("??");
	printf("ReplaceWithRestLength() ==> %s\n", stringObj.str.c_str());
	stringObj.ReplaceWithRestLength("xx");
	printf("ReplaceWithRestLength() ==> %s\n", stringObj.str.c_str());

	u8 hexArray[10];
	int resultLen;
	resultLen = LibString_HexStringToCharString(hexArray, stringObj.str.c_str(), 10);
	ARRAYDUMPX3(hexArray, resultLen);

	char testString2[] = "11 22 33";
	char testString3[] = "AA";
	char *argv[4];
	argv[0] = &testString2[0];
	argv[1] = &testString2[3];
	argv[2] = &testString3[0];
	argv[3] = &testString2[6];
	LibString_2D_HexStringToCharString(hexArray, argv, 4);
	ARRAYDUMPX3(hexArray, 4);
}

