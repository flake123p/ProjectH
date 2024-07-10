
#include "Everything_Lib_Mgr.hpp"

/*
	23,22,21,20,  19,18,17,16,  15,14,13,12,  11,10,09,08,  07,06,05,04,  03,02,01,00
	[    OPCODE(6)    ] [                 ]   [             Operand(12)             ]
*/
static OPCODE_TABLE_t gOpcodetable[] = {
	{"LDA", 0x01, 1},
	{"LDB", 0x02, 1},
};
int OpcodeTableSearch(std::string &s, OUT u32 &val, OUT u32 &operandNum)
{
	for (u32 i=0; i<LENGTH_OF_ARRAY(gOpcodetable); i++) {
		if (s == gOpcodetable[i].s) {
			val = gOpcodetable[i].val;
			operandNum = gOpcodetable[i].operandNum;
			return 0;
		}
	}
	return 1;
}


typedef enum {
	LS_FIND_FIRST_WORD,
	// Symbol define
	LS_FIND_EQU,
	LS_FIND_SYMBOL_VAL,
	// Normal
	LS_FIND_OPCODE,
	LS_FIND_OPERAND0,
	LS_FIND_OPERAND1,
	LS_FIND_OPERAND2,

	LS_FIND_NOTHING,
}LINE_STATE_t;
int AsmLists::AddLine(const char *s)
{
	LibStringClass str(s);

	str.Split();
	//str.Dump();

	ASM_LINE_t newLine;
	AsmLineInit(newLine);
	newLine.line = curLine;
	newLine.loc  = curLoc;

	ASM_WORD_t newWord;

	// State Machine start.
	LINE_STATE_t ls = LS_FIND_FIRST_WORD;
	for (u32 i = 0; i < str.subStrVector.size(); i++) {
		AsmWordInit(newWord);
		AsmWordParsing(str.subStrVector[i], newWord);
		newLine.words.push_back(newWord);
		#if 1
		switch (ls) {
			case LS_FIND_FIRST_WORD: {
				switch (newWord.type) {
					case WORD_LABEL: {
						//ls = LS_FIND_OPCODE;
						BASIC_ASSERT(0);//TODO
					} break;

					case WORD_OPCODE: {
						if (newWord.operandNum > 0) {
							ls = LS_FIND_OPERAND0;
							BASIC_ASSERT(newLine.type == LINE_NONE);
							newLine.type = LINE_CODE_ABS_SEG_START;
							newLine.sizeInByte = 3;
							SET_BIT_FIELD(newLine.machineCode, newWord.val, SIZE_6, OFFSET_18);
						} else {
							BASIC_ASSERT(0);//TODO
						}
						BASIC_ASSERT(i<2);
					} break;
					
					default:
						BASIC_ASSERT(0);
						break;
				}
			} break;

			case LS_FIND_OPERAND0: {
				switch (newWord.type) {
					case WORD_OPERAND: {
						ls = LS_FIND_NOTHING;
						SET_BIT_FIELD(newLine.machineCode, newWord.val, SIZE_12, OFFSET_0);
					} break;

					default:
						BASIC_ASSERT(0);
						break;
				}
			} break;
			
			default:
				BASIC_ASSERT(0);
				break;
		}
		#endif
	}
	
	lineVec.push_back(newLine);
	
	curLine++;
	curLoc+=newLine.sizeInByte;

	return 0;
}

int AsmLists::Dump(void)
{
	printf("[LINE] [LOC] [SIZE] [TYPE] [PC] [BASE] [X] [AT] [M-Code]\n");
	for (u32 i = 0; i < lineVec.size(); i++) {
		AsmLineDump(lineVec[i]);
	}
	return 0;
}

int AsmWordInit(ASM_WORD_t &asmWord)
{
	asmWord.str  = "NULL";
	asmWord.type = WORD_INVALID;
	asmWord.addrType = AT_INVALID;
	asmWord.val = 0;
	asmWord.operandNum = 0;

	return 0;
}

int AsmWordParsing(std::string &s, OUT ASM_WORD_t &asmWord)
{
	int retVal;

	asmWord.str = s;
	asmWord.type = WORD_SIMPLE;
	asmWord.addrType = AT_NONE;
	
	u32 len = s.length();
	if (s[len-1] == ':') {
		asmWord.type = WORD_LABEL;
		return 0;
	}

	if (s[0] == '#') {
		asmWord.type = WORD_OPERAND;
		asmWord.addrType = AT_IMMEDIATE;
		s32 temp = LibString_DecStringToInt(ADDRX(s.c_str(), 1));
		asmWord.val = (u32)temp;
		return 0;
	}

	retVal = OpcodeTableSearch(s, asmWord.val, asmWord.operandNum);
	if (0 == retVal) {
		
		asmWord.type = WORD_OPCODE;
		return 0;
	}

	return 0;
}

int AsmLineInit(ASM_LINE_t &asmLine)
{
	asmLine.line = 0;
	asmLine.loc = 0;
	asmLine.sizeInByte  = 0;
	asmLine.type = LINE_NONE;
	asmLine.pcAddressing = false;
	asmLine.baseAddressing = false;
	asmLine.indexAddressing = false;
	asmLine.addrType = AT_NONE;
	asmLine.machineCode = 0;
	asmLine.words.clear();

	return 0;
}

int AsmLineDump(ASM_LINE_t &asmLine)
{
	printf(
		"%-4d   %-5d %04X   %-5d  %d    %d      %d   %d    0x%08X >> ",
		asmLine.line,
		asmLine.loc,
		asmLine.sizeInByte,
		asmLine.type,
		asmLine.pcAddressing,
		asmLine.baseAddressing,
		asmLine.indexAddressing,
		asmLine.addrType,
		asmLine.machineCode);
	for (u32 j = 0; j < asmLine.words.size(); j++) {
		printf("%s  ", asmLine.words[j].str.c_str());
	}
	printf("\n");

	return 0;
}
