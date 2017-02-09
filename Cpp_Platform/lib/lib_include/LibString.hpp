

#ifndef _LIB_STRING_HPP_INCLUDED_

#include "My_Types.h"

class ConstStr {
	private:
		
	public:
		const char *str;
		ConstStr(void);
		void SetStr(const char *inStr);
};

bool LibString_IsCharHex(char ch);
bool LibString_IsCharNumber(char ch);
bool LibString_IsCharLetter(char ch);
bool LibString_IsCharLegal(char ch, const char *legalCharAry, int aryLen);
bool LibString_IsStringAllLetter(const char *inStr);
bool LibString_CharToIndex(IN char ch, IN bool doAddingInLowercase, OUT int *index);
int LibString_HexStringToCharString(const char *srcString, char *dstString, int maxLength);
void LibString_2D_HexStringToCharString(char *srcString[], char *dstString, int maxLength);
int LibString_DecStringToInt(const char * str);
int LibString_HexStringToInt(const char * str);
char * LibString_Copy(char * destination, const char * source, int num = -1);

#define _LIB_STRING_HPP_INCLUDED_
#endif//_LIB_STRING_HPP_INCLUDED_

