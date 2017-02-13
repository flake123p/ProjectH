

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

typedef struct {
	u32 index;
	const char *abbreviation;
	const char *caretNotation;
	const char *escapeSequecesInC;
	const char *description;
} CTRL_CHAR_ATTRIB_t;
CTRL_CHAR_ATTRIB_t *LibString_ControlChar_TableGet(OUT u32 *length);
void LibString_DumpControlChar(void);
void LibString_DumpPrintableChar(void);

/*
	string.h     http://www.cplusplus.com/reference/cstring/

	Copying:
		void * memcpy ( void * destination, const void * source, size_t num );
			// destination is returned. 
			// To avoid overflows, the size of the arrays pointed to by both the destination and source parameters, 
			// shall be at least num bytes, and should not overlap (for overlapping memory blocks, memmove is a safer approach).

		void * memmove ( void * destination, const void * source, size_t num );
			// destination is returned.
			// allowing the destination and source to overlap.

		char * strcpy ( char * destination, const char * source );
			// destination is returned.

		char * strncpy ( char * destination, const char * source, size_t num );
			// destination is returned.

	Other:
		void * memset ( void * ptr, int value, size_t num );
			// ptr is returned.
		
		size_t strlen ( const char * str );

	
*/
#define _LIB_STRING_HPP_INCLUDED_
#endif//_LIB_STRING_HPP_INCLUDED_

