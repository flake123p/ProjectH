

#ifndef _LIB_STRING_HPP_INCLUDED_

class ConstStr {
	private:
		
	public:
		const char *str;
		ConstStr(void);
		void SetStr(const char *inStr);
};

bool String_IsCharHex(char ch);
bool String_IsCharNumber(char ch);
bool String_IsCharLetter(char ch);
int String_HexStringToCharString(const char *srcString, char *dstString, int maxLength);

#define _LIB_STRING_HPP_INCLUDED_
#endif//_LIB_STRING_HPP_INCLUDED_

