#include "Everything_ThisMod.hpp"

/*
    NOT USE IN ARRAY!!
    @print "abc" %x i j[4] k[4-e] "123"

SINGAL ELEMENT MATH (@add @sub @mux @div @mod @and @or)
    @add i j
    @add i %d 12
    @add i[6] j[7]

WRITE
    @write i %d 3 6
    @write i j
    
    @compare [result] [var1] [var2]
    @write [var_name] [position] [-const] %x 09 A5
    @write [var_name] [position] [-var] [var_name] [position] [len]
    @if [-equ/-smaller/-bigger]
    @if_end
    @print
    @exit
    @for [var_name] [start val] [end val]
    @for_end
*/

TextVar *TextCall_GetVar(TextCallDB *textCallDB, std::string *in, u32 *outStartIdx, u32 *outEndIdx, int singleElementCheck /*= 0*/)
{
    std::string extractName;
    LibString_IsArrayPattern(in, &extractName, outStartIdx, outEndIdx);
    TextVar *var = textCallDB->textVarDB->FindVar(&extractName);
    u32 varNum;

    if (var == NULL) {
        printf("[Unknown Var:%s]", in->c_str());
        return NULL;
    }
    varNum = var->pUniVar->MaxNum();
    if (varNum == 0) {
        printf("[Uninit Var:%s]", in->c_str());
        return NULL;
    }
    if (*outEndIdx == 0xFFFFFFFF) {
        *outEndIdx = varNum - 1;
    }
    if ((*outStartIdx > *outEndIdx) || (*outStartIdx >= varNum) || (*outEndIdx >= varNum)) {
        printf("[Var Range Error:%s (Max:%d)]", in->c_str(), varNum-1);
        return NULL;
    }

    if (singleElementCheck) {
        if (*outStartIdx != *outEndIdx) {
            printf("[Not Single Element:%s]", in->c_str());
            return NULL;
        }
    }
    return var;
}

TextVar *TextCall_GetVarWithSingleU32(TextCallDB *textCallDB, std::string *in, u32 *outIdx, u32 *outVal)
{
    u32 startIdx, endIdx;

    TextVar *var = TextCall_GetVar(textCallDB, in, &startIdx, &endIdx, 1);
    if (var == NULL) {
        return NULL;
    }
    *outIdx = startIdx;
    var->pUniVar->GetSingleElement32(startIdx, outVal);

    return var;
}

int TextCall_CompareSingleElement(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, int *isTrue)
{
    TextVar *var;
    u32 dummyInx;
    u32 uCmp0, uCmp1;
    s32 sCmp0, sCmp1;
    int useSignedCompare = 0;

    *isTrue = 0;

    FORMAT_ERROR_CHECK(splitedStrAgent->subStrVector.size() < 4);

    size_t curr = 1;
    size_t formatStrIdx;
    int isSignedFormat;

    FORMAT_ERROR_CHECK(curr >= splitedStrAgent->subStrVector.size());
    //curr 1

    if(LibString_IsPrintPattern(SUB_STR_PTR(curr), &isSignedFormat)) {
        FORMAT_ERROR_CHECK(curr+1 >= splitedStrAgent->subStrVector.size());
        sscanf(SUB_C_STR(curr+1), SUB_C_STR(curr), &uCmp0);
        if (isSignedFormat) {
            useSignedCompare = 1;
        }
        curr += 2;
    } else {
        var = TextCall_GetVarWithSingleU32(textCallDB, SUB_STR_PTR(curr), &dummyInx, &uCmp0);
        if (var == NULL) {
            return TXT_CALL_RET_ERROR_LOG;
        }
        if (var->pUniVar->type & VAR_IS_SIGNED) {
            useSignedCompare = 1;
        }
        curr += 1;
    }
    FORMAT_ERROR_CHECK(curr >= splitedStrAgent->subStrVector.size());
    formatStrIdx = curr;
    curr += 1;
    FORMAT_ERROR_CHECK(curr >= splitedStrAgent->subStrVector.size());

    if(LibString_IsPrintPattern(SUB_STR_PTR(curr), &isSignedFormat)) {
        FORMAT_ERROR_CHECK(curr+1 >= splitedStrAgent->subStrVector.size());
        sscanf(SUB_C_STR(curr+1), SUB_C_STR(curr), &uCmp1);
        if (isSignedFormat) {
            useSignedCompare = 1;
        }
        curr += 2;
    } else {
        var = TextCall_GetVarWithSingleU32(textCallDB, SUB_STR_PTR(curr), &dummyInx, &uCmp1);
        if (var == NULL) {
            return TXT_CALL_RET_ERROR_LOG;
        }
        if (var->pUniVar->type & VAR_IS_SIGNED) {
            useSignedCompare = 1;
        }
        curr += 1;
    }
    FORMAT_ERROR_CHECK(curr != splitedStrAgent->subStrVector.size());

    sCmp0 = (s32)uCmp0;
    sCmp1 = (s32)uCmp1;
//DUMPND(useSignedCompare);
    do {
        if (SUB_STR(formatStrIdx) == ">") {
            if (useSignedCompare) {
                if (sCmp0 > sCmp1) {
                    *isTrue = 1;
                }
            } else {
                if (uCmp0 > uCmp1) {
                    *isTrue = 1;
                }
            }
            break;
        }

        if (SUB_STR(formatStrIdx) == "<") {
            if (useSignedCompare) {
                if (sCmp0 < sCmp1) {
                    *isTrue = 1;
                }
            } else {
                if (uCmp0 < uCmp1) {
                    *isTrue = 1;
                }
            }
            break;
        }

        if (SUB_STR(formatStrIdx) == "==") {
            if (useSignedCompare) {
                if (sCmp0 == sCmp1) {
                    *isTrue = 1;
                }
            } else {
                if (uCmp0 == uCmp1) {
                    *isTrue = 1;
                }
            }
            break;
        }

        if (SUB_STR(formatStrIdx) == ">=") {
            if (useSignedCompare) {
                if (sCmp0 >= sCmp1) {
                    *isTrue = 1;
                }
            } else {
                if (uCmp0 >= uCmp1) {
                    *isTrue = 1;
                }
            }
            break;
        }

        if (SUB_STR(formatStrIdx) == "<=") {
            if (useSignedCompare) {
                if (sCmp0 <= sCmp1) {
                    *isTrue = 1;
                }
            } else {
                if (uCmp0 <= uCmp1) {
                    *isTrue = 1;
                }
            }
            break;
        }

        else {
            FORMAT_ERROR_CHECK(1);
        }
    } while (0);

    return 0;
}

int TextCall_Print(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    std::string printFormat = "%d";
    u32 startIdx, endIdx;
    int printNewLine = (int)userHdl_0;

    for (size_t i = 1; i <splitedStrAgent->subStrVector.size(); i++) {
        if (splitedStrAgent->subStrFlagVector[i] & LIB_STR_SUB_DOUBLE_QUOTE) {
            printf("%s", splitedStrAgent->subStrVector[i].c_str());
            continue;
        } else {
            if (LibString_IsPrintPattern(&(splitedStrAgent->subStrVector[i]))) {
                printFormat = (splitedStrAgent->subStrVector[i]);
                continue;
            }

            TextVar *var = TextCall_GetVar(textCallDB, &(splitedStrAgent->subStrVector[i]), &startIdx, &endIdx);
            if (var == NULL) {
                continue;
            }

            u32 value, j;
            j = startIdx;
            while (1) {
                var->pUniVar->GetSingleElement32(j, &value);
                printf(printFormat.c_str(), value);
                if (j == endIdx) {
                    break;
                } else {
                    j++;
                    printf(" ");
                }
            }
        }
    }

    if (printNewLine) {
        printf("\n");
    }

    //PRINT_FUNC;
    //splitedStrAgent->Dump();
    //u32 *x = (u32 *)userHdl_0;
    //DUMPND(*x);
    //splitedStrAgent->Dump();
    return 0;
}

int TextCall_PrintNewLine(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    printf("\n");
    return 0;
}

/*
SINGAL EXISTED ELEMENT MATH (@add @sub @mux @div @mod @and @or @xor @not) to <- from
@add i j
@add i %d 12
@add i[6] j[7]

@not i
@not i[7]

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
    TXT_CALL_MATH_NOT,
    TXT_CALL_MATH_CMPL,
};
*/
int TextCall_Math(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    int useScan = 0;
    std::string scanFormat;

    int mode = (int)userHdl_0;
    //printf("mode = %d\n", mode);

    TextVar *varTo;
    TextVar *varFrom;

    FORMAT_ERROR_CHECK(splitedStrAgent->subStrVector.size() < 3);

    if (LibString_IsPrintPattern(&(splitedStrAgent->subStrVector[2]))) {
        scanFormat = (splitedStrAgent->subStrVector[2]);
        useScan = 1;
        FORMAT_ERROR_CHECK(splitedStrAgent->subStrVector.size() != 4);
    } else {
        FORMAT_ERROR_CHECK(splitedStrAgent->subStrVector.size() != 3);
    }

    u32 toIdx, fromIdx, valueTo, valueFrom;

    varTo = TextCall_GetVarWithSingleU32(textCallDB, &(splitedStrAgent->subStrVector[1]), &toIdx, &valueTo);
    if (varTo == NULL) {
        return TXT_CALL_RET_ERROR_LOG;
    }

    if (useScan) {
        sscanf(splitedStrAgent->subStrVector[3].c_str(), scanFormat.c_str(), &valueFrom);
    } else {
        varFrom = TextCall_GetVarWithSingleU32(textCallDB, &(splitedStrAgent->subStrVector[2]), &fromIdx, &valueFrom);
        if (varFrom == NULL) {
            return TXT_CALL_RET_ERROR_LOG;
        }
    }

    //printf("mode = 0x%04X, to:%d, from:%d", mode, valueTo, valueFrom);
    switch (mode) {
        case TXT_CALL_MATH_SET: valueTo = valueFrom; break;
        case TXT_CALL_MATH_ADD: valueTo += valueFrom; break;
        case TXT_CALL_MATH_SUB: valueTo -= valueFrom; break;
        case TXT_CALL_MATH_MUX: valueTo *= valueFrom; break;
        case TXT_CALL_MATH_DIV: valueTo /= valueFrom; break;
        case TXT_CALL_MATH_MOD: valueTo %= valueFrom; break;
        case TXT_CALL_MATH_AND: valueTo &= valueFrom; break;
        case TXT_CALL_MATH_OR : valueTo |= valueFrom; break;
        case TXT_CALL_MATH_XOR: valueTo ^= valueFrom; break;

        case TXT_CALL_MATH_LSHIFT: valueTo = valueTo << valueFrom; break;
        case TXT_CALL_MATH_RSHIFT: valueTo = valueTo >> valueFrom; break;
        default: BASIC_ASSERT(0); break;
    }
    //printf(" result:%d\n", valueTo);
    varTo->pUniVar->SetSingleElement32(toIdx, valueTo);

    return 0;
}

int TextCall_Math_Not(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    int mode = (int)userHdl_0;
    TextVar *varTo;

    FORMAT_ERROR_CHECK(splitedStrAgent->subStrVector.size() != 2);

    u32 startIdx, endIdx, toIdx, valueTo;

    varTo = TextCall_GetVar(textCallDB, &(splitedStrAgent->subStrVector[1]), &startIdx, &endIdx, 1);
    if (varTo == NULL) {
        return TXT_CALL_RET_ERROR_LOG;
    }
    varTo->pUniVar->GetSingleElement32(startIdx, &valueTo);
    toIdx = startIdx;

    switch (mode) {
        case TXT_CALL_MATH_NOT: valueTo = !valueTo; break;
        case TXT_CALL_MATH_CMPL: valueTo = ~valueTo; break;
        default: BASIC_ASSERT(0); break;
    }

    varTo->pUniVar->SetSingleElement32(toIdx, valueTo);
    return 0;
}

int TextCall_If(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    int isTrue = 0;

    int ret = TextCall_CompareSingleElement(textCallDB, splitedStrAgent, &isTrue);

    if (ret == 0) {
        if (isTrue) {
        } else {
            textCallDB->skipEnable = 1;
            textCallDB->skipOverString = "@else";
        }
    }

    return ret;
}

int TextCall_Else(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    textCallDB->skipEnable = 1;
    textCallDB->skipOverString = "@endif";
    return 0;
}

int TextCall_Endif(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    return 0;
}

void TextCall_BasicInit(TextCallDB *db)
{
    ASSERT_IF( db->AddCallPair("@print", (TextCall_CB_t)TextCall_Print, (void *)0) );
    ASSERT_IF( db->AddCallPair("@printn", (TextCall_CB_t)TextCall_Print, (void *)1 /*1 for print next line*/) );
    ASSERT_IF( db->AddCallPair("@nl", (TextCall_CB_t)TextCall_PrintNewLine) );
    ASSERT_IF( db->AddCallPair("@set", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_SET) );
    ASSERT_IF( db->AddCallPair("@add", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_ADD) );
    ASSERT_IF( db->AddCallPair("@sub", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_SUB) );
    ASSERT_IF( db->AddCallPair("@mux", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_MUX) );
    ASSERT_IF( db->AddCallPair("@div", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_DIV) );
    ASSERT_IF( db->AddCallPair("@mod", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_MOD) );
    ASSERT_IF( db->AddCallPair("@and", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_AND) );
    ASSERT_IF( db->AddCallPair("@or",  (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_OR) );
    ASSERT_IF( db->AddCallPair("@xor", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_XOR) );
    ASSERT_IF( db->AddCallPair("@<<", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_LSHIFT) );
    ASSERT_IF( db->AddCallPair("@lshift", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_LSHIFT) );
    ASSERT_IF( db->AddCallPair("@>>", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_RSHIFT) );
    ASSERT_IF( db->AddCallPair("@rshift", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_RSHIFT) );

    ASSERT_IF( db->AddCallPair("@not", (TextCall_CB_t)TextCall_Math_Not, (void *)TXT_CALL_MATH_NOT) );
    ASSERT_IF( db->AddCallPair("@cmpl", (TextCall_CB_t)TextCall_Math_Not, (void *)TXT_CALL_MATH_CMPL) );
    ASSERT_IF( db->AddCallPair("@if", (TextCall_CB_t)TextCall_If) );
    ASSERT_IF( db->AddCallPair("@else", (TextCall_CB_t)TextCall_Else) );
    ASSERT_IF( db->AddCallPair("@endif", (TextCall_CB_t)TextCall_Endif) );
}

