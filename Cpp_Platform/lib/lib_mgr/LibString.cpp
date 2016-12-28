//#include "stdafx.h" //For porting to "Visual Studio"
#include <stdio.h>
#include "LibString.hpp"

ConstStr::ConstStr(void)
{
	str = (const char *)(0);
}

void ConstStr::SetStr(const char *inStr)
{
	str = inStr;
}

bool String_IsCharHex(char ch)
{
	if (ch >= '0' && ch <= '9')
		return true;

	if (ch >= 'A' && ch <= 'F')
		return true;

	if (ch >= 'a' && ch <= 'f')
		return true;

	return false;
}

bool String_IsCharNumber(char ch)
{
	if (ch >= '0' && ch <= '9')
		return true;

	return false;
}

bool String_IsCharLetter(char ch)
{
	if (ch >= 'A' && ch <= 'Z')
		return true;

	if (ch >= 'a' && ch <= 'z')
		return true;

	return false;
}

//Max return value = maxLength
int String_HexStringToCharString(const char *srcString, char *dstString, int maxLength)
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
			if (String_IsCharHex(srcString[offset]))
				offset++;
			else
				break;
		}
		//To next word
		while(1) {
			if (srcString[offset] == 0)
				goto END;
			if (!String_IsCharHex(srcString[offset]))
				offset++;
			else
				break;
		}
	}
END:
	return numberOfChar;
}
