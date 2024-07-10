

#ifndef _LIB_SYNTEX_HPP_INCLUDED_
#include "My_Types.h"
#include <string>
#include <vector>
// ============================== Debug ==============================

// ============================== Define ==============================
typedef struct {
	const char *s;
	u32 val;
	u32 operandNum;
}OPCODE_TABLE_t;
int OpcodeTableSearch(std::string &s, OUT u32 &val, OUT u32 &operandNum);

typedef enum {
	AT_IMMEDIATE,
	AT_SIMPLE,
	AT_INDIRECT,
	AT_NONE,
	
	AT_INVALID,
}ADDRESSING_TYPE_t;

typedef enum {
	WORD_SIMPLE,
	WORD_LABEL,
	WORD_OPCODE,
	WORD_OPERAND,

	WORD_INVALID,
}ASM_WORD_TYPE_t;
typedef struct {
	std::string str;
	ASM_WORD_TYPE_t type;
	ADDRESSING_TYPE_t addrType;
	u32 val;
	u32 operandNum;
}ASM_WORD_t; //Superset for all kinds of words
int AsmWordInit(ASM_WORD_t &asmWord);
int AsmWordParsing(std::string &s, OUT ASM_WORD_t &asmWord);

typedef enum {
	LINE_NONE,
	//LINE_CODE,
	LINE_LABEL_ONLY,
	LINE_CODE_ABS_SEG_START,
	LINE_CODE_REL_SEG_START,
	LINE_CODE_ABS_DATA_START,
	LINE_CODE_REL_DATA_START,

	LINE_INVALID,
}ASM_LINE_TYPE_t;
typedef struct {
	u32 line;
	u32 loc;
	u32 sizeInByte;
	ASM_LINE_TYPE_t type;
	bool pcAddressing;
	bool baseAddressing;
	bool indexAddressing;
	ADDRESSING_TYPE_t addrType;
	u32 machineCode;
	std::vector<ASM_WORD_t> words;
}ASM_LINE_t;
int AsmLineInit(ASM_LINE_t &asmLine);
int AsmLineDump(ASM_LINE_t &asmLine);

class AsmLists {
public:
	std::vector<ASM_LINE_t> lineVec;
	u32 curLine;
	u32 curLoc;
	
	AsmLists(void){curLine=0;curLoc=0;};
	~AsmLists(void){};
	int AddLine(const char *s);
	int Dump(void);
private:
};
#define _LIB_SYNTEX_HPP_INCLUDED_
#endif//_LIB_SYNTEX_HPP_INCLUDED_



