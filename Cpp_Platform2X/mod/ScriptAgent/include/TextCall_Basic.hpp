

#ifndef _TEXT_CALL_BASIC_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"
#include "TextCall.hpp"

#define SUB_STR(x)       (splitedStrAgent->subStrVector[x])
#define SUB_STR_PTR(x)   (&(splitedStrAgent->subStrVector[x]))
#define SUB_C_STR(x) (splitedStrAgent->subStrVector[x].c_str())
#define FORMAT_ERROR_CHECK(condition) if(condition){printf("[Format error : %s] (%d)", splitedStrAgent->str.c_str(), __LINE__);return TXT_CALL_RET_ERROR_LOG;}

enum {
    TXT_CALL_MATH_SET = 0x1100,
    TXT_CALL_MATH_ADD,
    TXT_CALL_MATH_SUB,
    TXT_CALL_MATH_MUX,
    TXT_CALL_MATH_DIV,
    TXT_CALL_MATH_MOD,
    TXT_CALL_MATH_AND,
    TXT_CALL_MATH_OR,
    TXT_CALL_MATH_XOR,
    TXT_CALL_MATH_LSHIFT,
    TXT_CALL_MATH_RSHIFT,
    TXT_CALL_MATH_NOT,
    TXT_CALL_MATH_CMPL,
};

TextVar *TextCall_GetVar(TextCallDB *textCallDB, std::string *in, u32 *outStartIdx, u32 *outEndIdx, int singleElementCheck = 0);
void TextCall_BasicInit(TextCallDB *db);


#define _TEXT_CALL_BASIC_HPP_INCLUDED_
#endif//_TEXT_CALL_BASIC_HPP_INCLUDED_

