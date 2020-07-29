#include "Everything_ThisMod.hpp"

static u32 gPrint;
static u32 gPrintN;

/*
    NOT USE IN ARRAY!!
    @print "abc" %x i j[4] k[4-e] "123"

SINGAL ELEMENT MATH (@add @sub @mux @div @mod @and @or)
    @add i j
    @add i %d 12
    @add i[6] j[7]

    @add [var_name] [-const] %x 09
    @add [var_name] [-var] [var_name]
    
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

TextVar *TextCall_GetVar(TextCallDB *textCallDB, std::string *in, u32 *outStartIdx, u32 *outEndIdx)
{
    std::string extractName;
    LibString_IsArrayPattern(in, &extractName, outStartIdx, outEndIdx);
    TextVar *var = textCallDB->textVarDB->FindVar(&extractName);
    u32 varNum;

    if (var == NULL) {
        printf("[Unknown Var:%s]", in->c_str());
        return NULL;
    }
    varNum = var->pUniVar->Num();
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
    return var;
}

int TextCall_Print(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    std::string extractName;
    std::string printFormat = "%d";
    u32 startIdx, endIdx;
    u32 *printMode = (u32 *)userHdl_0;

    for (size_t i = 1; i <splitedStrAgent->subStrVector.size(); i++) {
        if (splitedStrAgent->subStrFlagVector[i] & LIB_STR_SUB_DOUBLE_QUOTE) {
            printf("%s", splitedStrAgent->subStrVector[i].c_str());
            continue;
        } else {
            if (LibString_IsPrintPattern(&(splitedStrAgent->subStrVector[i]))) {
                printFormat = (splitedStrAgent->subStrVector[i]);
                continue;
            }
            LibString_IsArrayPattern(&(splitedStrAgent->subStrVector[i]), &extractName, &startIdx, &endIdx);
            TextVar *var = textCallDB->textVarDB->FindVar(&extractName);
            u32 varNum;
            u32 value, j;

            if (var == NULL) {
                printf("[Unknown Var:%s]", splitedStrAgent->subStrVector[i].c_str());
                continue;
            }
            varNum = var->pUniVar->Num();
            if (varNum == 0) {
                printf("[Uninit Var:%s]", splitedStrAgent->subStrVector[i].c_str());
                continue;
            }
            if (endIdx == 0xFFFFFFFF) {
                endIdx = varNum - 1;
            }
            if ((startIdx > endIdx) || (startIdx >= varNum) || (endIdx >= varNum)) {
                printf("[Var Range Error:%s (Max:%d)]", splitedStrAgent->subStrVector[i].c_str(), varNum-1);
                continue;
            }

            j = startIdx;
            while (1) {
                if (var->pUniVar->type & VAR_IS_8BITS) {
                    value = (u32)(((u8 *)var->pUniVar->ptr)[j]);
                } else if (var->pUniVar->type & VAR_IS_16BITS) {
                    value = (u32)(((u16 *)var->pUniVar->ptr)[j]);
                } else if (var->pUniVar->type & VAR_IS_32BITS) {
                    value = (u32)(((u32 *)var->pUniVar->ptr)[j]);
                } else {
                    //void *temp = ((void **)var->pUniVar->ptr)[j];
                    value = (u32)((int **)var->pUniVar->ptr)[j];
                    //DUMPA(temp);
                }
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

    if (printMode == &gPrintN) {
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
SINGAL ELEMENT MATH (@add @sub @mux @div @mod @and @or) to <- from
@add i j
@add i %d 12
@add i[6] j[7]
*/
enum {
    TXT_CALL_MATH_ADD = 1100,
};
int TextCall_Math(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    int i = (int)userHdl_0;
    printf("i = %d\n", i);

    TextVar *varTo;
    TextVar *varFrom;

    if(splitedStrAgent->subStrVector.size() < 3) {
        printf("[Format error : %s]\n", splitedStrAgent->str.c_str());
        return 0;
    }

    return 0;
}

void TextCall_BasicInit(TextCallDB *db)
{
    ASSERT_IF( db->AddCallPair("@print", (TextCall_CB_t)TextCall_Print, &gPrint) );
    ASSERT_IF( db->AddCallPair("@printn", (TextCall_CB_t)TextCall_Print, &gPrintN) );
    ASSERT_IF( db->AddCallPair("@nl", (TextCall_CB_t)TextCall_PrintNewLine) );
    ASSERT_IF( db->AddCallPair("@add", (TextCall_CB_t)TextCall_Math, (void *)TXT_CALL_MATH_ADD) );
}

