#include "Everything_ThisMod.hpp"

/*
    NOT USE IN ARRAY!!
    @print "abc" %x [var_name] [-posi] num "123"
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

int TextCall_Print(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    std::string extractName;
    u32 arrayIndex;
    int isArray;

    for (size_t i = 1; i <splitedStrAgent->subStrVector.size(); i++) {
        if (splitedStrAgent->subStrFlagVector[i] & LIB_STR_SUB_DOUBLE_QUOTE) {
            printf("%s", splitedStrAgent->subStrVector[i].c_str());
            continue;
        }
        isArray = LibString_ParseArrayPattern(&(splitedStrAgent->subStrVector[i]), &extractName, &arrayIndex);
    }
    printf("\n");

    PRINT_FUNC;
    splitedStrAgent->Dump();
    //u32 *x = (u32 *)userHdl_0;
    //DUMPND(*x);
    //splitedStrAgent->Dump();

#if 0
    TextVar *var = textCallDB->textVarDB->AddPair(splitedStrAgent->subStrVector[2]);
    if (var == NULL) {
        printf("Duplicate var name:%s\n", splitedStrAgent->subStrVector[2].c_str());
        BASIC_ASSERT(0);
    }
    var->name = splitedStrAgent->subStrVector[2];

    u32 temp;
    for (size_t i = 4; i < splitedStrAgent->subStrVector.size(); i++) {
        sscanf(splitedStrAgent->subStrVector[i].c_str(), splitedStrAgent->subStrVector[3].c_str(), &temp);
        //DUMPNX(temp);
        if (splitedStrAgent->subStrVector[1] == "u8") {
            if (temp > 0xFF) {
                printf("val is big than u8 (0x%X)\n", temp);
                BASIC_ASSERT(0);
            }
            var->pUniVar->Import((u8)temp);
        }
        else if (splitedStrAgent->subStrVector[1] == "s8") {
            if (temp > 0xFF) {
                printf("val is big than s8 (0x%X)\n", temp);
                BASIC_ASSERT(0);
            }
            var->pUniVar->Import((s8)temp);
        }
        else if (splitedStrAgent->subStrVector[1] == "u16") {
            if (temp > 0xFFFF) {
                printf("val is big than u16 (0x%X)\n", temp);
                BASIC_ASSERT(0);
            }
            var->pUniVar->Import((u16)temp);
        }
        else if (splitedStrAgent->subStrVector[1] == "s16") {
            if (temp > 0xFFFF) {
                printf("val is big than s16 (0x%X)\n", temp);
                BASIC_ASSERT(0);
            }
            var->pUniVar->Import((s16)temp);
        }
        else if (splitedStrAgent->subStrVector[1] == "u32") {
            var->pUniVar->Import(temp);
        }
        else if (splitedStrAgent->subStrVector[1] == "s32") {
            var->pUniVar->Import((s32)temp);
        }
        else {
            printf("scan type not support (%s)\n", splitedStrAgent->subStrVector[1].c_str());
            BASIC_ASSERT(0);
        }
    }
#endif
    return 0;
}

void TextCall_BasicInit(TextCallDB *db)
{
    ASSERT_IF( db->AddPair("@print", (TextCall_CB_t)TextCall_Print) );
}

