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
*/
bool LibString_CharToIndex(IN char ch, IN bool doAddingInLowercase, OUT int *index)
{
	if (false == LibString_IsCharLetter(ch)) {
		return false;
	}

	if (ch >= 'A' && ch <= 'Z') {
		*index = ch - 'A';
	} else if (ch >= 'a' && ch <= 'z') {
		*index = ch - 'a';
		if (doAddingInLowercase)
			*index = *index + 26;
	} else {
		BASIC_ASSERT(0);
	}

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
