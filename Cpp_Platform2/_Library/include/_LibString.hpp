

#ifndef __LIB_STRING_HPP_INCLUDED_
#include <vector>
#include <string>
#include <locale> //std::locale settings;
#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================

class ConstStr {
	private:
		
	public:
		const char *str;
		ConstStr(void);
		void SetStr(const char *inStr);
};

bool LibString_IsCharHexadecimal(char ch, OUT u32 *val = NULL);
bool LibString_IsCharDecimal(char ch, OUT u32 *val = NULL);
bool LibString_IsCharLetter(char ch);
bool LibString_IsCharPrintable(char ch);
bool LibString_IsCharLegal(char ch, const char *legalCharAry, int aryLen);
bool LibString_IsStringAllLetter(const char *inStr);
bool LibString_IsStringNumerical(const char *inStr);
bool LibString_IsCharNonspace(char ch);

bool LibString_CharToIndex(IN char ch, IN bool doAddingInLowercase, OUT int *index);
int LibString_HexStringToCharString(u8 *dstString, const char *srcString, int maxLength);
int LibString_DecStringToCharString(u8 *dstString, const char *srcString, int maxLength);
void LibString_2D_HexStringToCharString(u8 *dstString, char *srcString[], int scanCount);
int LibString_DecStringToInt(const char * str);
int LibString_HexStringToInt(const char * str);
u32 LibString_HexStringToU32(const char * str);
char * LibString_Copy(char * destination, const char * source, int num = -1);
u8 LibString_GetCheckSumU8(u8 *srcAry, u32 srcAryLen);

u32 LibString_GetDeciOrHex(std::string *in);
int LibString_GetDeciOrHex32(std::string *in, u32 *out);
int LibString_IsArrayPattern(std::string *in, std::string *outAryName, u32 *outStartIdx, u32 *outEndIdx);
int LibString_IsArrayPattern2(std::string *in, std::string *outAryName, std::string *outIndexStr);
int LibString_IsPrintPattern(std::string *in, int *isSigned = NULL);


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

======================================================================= sprintf()
int sprintf ( char * str, const char * format, ... );
--
char buffer [50];
int n, a=5, b=3;
n=sprintf (buffer, "%d plus %d is %d", a, b, a+b);
printf ("[%s] is a string %d chars long\n",buffer,n);
--
Output:
[5 plus 3 is 8] is a string 13 chars long
======================================================================= sscanf()
int sscanf ( const char * s, const char * format, ...);
--
char sentence []="Rudolph is 12 years old";
char str [20];
int i;
sscanf (sentence,"%s %*s %d",str,&i);
printf ("%s -> %d\n",str,i);
--
Output:
Rudolph -> 12


*/

typedef enum {
    LIB_STR_SUB_DOUBLE_QUOTE    = BIT_0,

} LIB_STR_SUB_FLAG_t;

class LibStringClass{
public:
	std::string str;
	std::vector<std::string> subStrVector;
	std::vector<u32> subStrFlagVector; //LIB_STR_SUB_FLAG_t
	
	LibStringClass(const char *cString = NULL);
	~LibStringClass(void){}; // Do nothing.
	void Init(char *cString);
	void Init(const char *cString);
	void Init(std::string inStr);
	size_t Length(void); //Same result as Size()
	size_t Size(void); //Same result as Length()
	bool FindChar(const char ch, size_t pos = 0, OUT u32 *result_position = NULL);
	bool FindString(const char *s, size_t pos = 0, OUT u32 *result_position = NULL);
	int Split(bool checkDoubleQuote = false);
	int ReplaceWithRestLength(const char *s);
	int ReplaceWithRestLengthEx(const char *s);
	int RemoveRestStringByChar(char ch);
	int RemoveExtension(char ch);
	int ReplaceExtension(char ch, const char *s);
	int ReplaceSubString(const char *toStr, const char *fromStr);
	int ToUpperCase(void);

	// For INI file parsing
	int RemoveRestString(const char *s);
	int RemoveEmptyPrefixChar(void);
	int RemoveEmptyPostfixChar(void);
	int InsertBefore(const char *pattern, const char *s);
	int InsertAfter(const char *pattern, const char *s);

	const char* BitsStringOfOneByte_LittleEndian(u8 byte, u32 bits_size);

	const char* CStr(void);
	void Dump(void);
};

void LibString_Demo(void);


#define __LIB_STRING_HPP_INCLUDED_
#endif//__LIB_STRING_HPP_INCLUDED_

