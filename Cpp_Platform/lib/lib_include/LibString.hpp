

#ifndef _LIB_STRING_HPP_INCLUDED_

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
int LibString_HexStringToCharString(const char *srcString, char *dstString, int maxLength);
int LibString_DecStringToInt(const char * str);
int LibString_HexStringToInt(const char * str);

#define _LIB_STRING_HPP_INCLUDED_
#endif//_LIB_STRING_HPP_INCLUDED_

