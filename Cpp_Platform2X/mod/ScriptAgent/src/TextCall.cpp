
#include "Everything_ThisMod.hpp"

void TextCallDB::Dump(void)
{
    printf("=============== TextCallDB DUMP ============= \\\\\\\\\\\\\n");

    printf("Dump callback map:\n");
    std::map<std::string,TextCall_t>::iterator it;
    for (it=callbackMap.begin(); it!=callbackMap.end(); ++it) {
        printf("%s ... \n", it->first.c_str());
    }
}

int TextCallDB::AddCallPair(const char *str, TextCall_CB_t cb, void *userHdl_0 /*= NULL*/, void *userHdl_1 /*= NULL*/)
{
    std::string stdStr = str;
    TextCall_t textCall = {cb, userHdl_0, userHdl_1};

    std::pair<std::map<std::string,TextCall_t>::iterator,bool> ret;
    ret = callbackMap.insert ( std::pair<std::string,TextCall_t>(stdStr, textCall) );
    if (ret.second==false) {
        LibError_SetExtErrorMessage("str %s already existed\n", str);
        return 1;
    }

    return 0;
}

int TextCallDB::Start(const char *line, int *cbRet /*= NULL*/)
{
    int ret_from_cb;

    // 1. get first word
    LibStringClass strAgent = LibStringClass(line);
    strAgent.Split(true);
    //strAgent.Dump();

    if (strAgent.subStrVector.size() < 1) {
        return 1;
    }

    if (strAgent.subStrVector[0][0] == '#') {
        if (cbRet != NULL) {
            *cbRet = TXT_CALL_RET_COMMENT_LINE;
        }
        return 2;
    }

    std::map<std::string,TextCall_t>::iterator mapIt;

    mapIt = callbackMap.find(strAgent.subStrVector[0]);
    if (mapIt == callbackMap.end()) {
        //LibError_SetExtErrorMessage("Can't find section: %s\n", secName.c_str());
        printf("Unknown TextCall keywoard : %s\n", strAgent.subStrVector[0].c_str());
        return 1;
    }

    TextCall_t textCall = mapIt->second;
    ret_from_cb = (*(textCall.cb))(textCallDB, &strAgent, textCall.userHdl_0, textCall.userHdl_1);
    if (cbRet != NULL) {
        *cbRet = ret_from_cb;
    }

    return 0;
}

int TextCall_Var(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    //PRINT_FUNC;
    //u32 *x = (u32 *)userHdl_0;
    //DUMPND(*x);
    //splitedStrAgent->Dump();
    //return 0;

    TextVar *var = textCallDB->textVarDB->AddVarPair(&(splitedStrAgent->subStrVector[2]));
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

    return 0;
}

int TextCall_Dump(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    TextVar *var = textCallDB->textVarDB->FindVar(&(splitedStrAgent->subStrVector[1]));
    if (var == NULL) {
        printf("Unknown var name : %s\n", splitedStrAgent->subStrVector[1].c_str());
    } else {
        var->TextVarDump();
    }

    return 0;
}

int TextCall_DumpAll(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    textCallDB->textVarDB->Dump();

    return 0;
}

int TextCall_Call(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    //splitedStrAgent->Dump();

    if (splitedStrAgent->subStrVector.size() < 2) {
        printf("%s : no function name\n", splitedStrAgent->subStrVector[0].c_str());
    }

    TextVar *var = textCallDB->textVarDB->FindVar(&(splitedStrAgent->subStrVector[1]));
    if (var == NULL) {
        printf("Unknown func name : %s\n", splitedStrAgent->subStrVector[1].c_str());
    } else {
        BASIC_ASSERT(var->pUniVar->type & VAR_IS_VOID_POINTER);
        //var->TextVarDump();
        TextCall_CALL_CB_t cb = *((TextCall_CALL_CB_t *)(var->pUniVar->ptr));
        (*cb)(textCallDB, splitedStrAgent, userHdl_0, userHdl_1);
    }

    return 0;
}

int TextCall_Exit(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    return TXT_CALL_RET_EXIT;
}

int FuncHAHAHA(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    PRINT_FUNC_LINE;

    return 0;
}

void TextCall_DefaultInit(TextCallDB *db)
{
    db->textCallDB = db;
    ASSERT_IF( db->AddCallPair("@var", (TextCall_CB_t)TextCall_Var) );
    ASSERT_IF( db->AddCallPair("@_var", (TextCall_CB_t)TextCall_Var) );
    ASSERT_IF( db->AddCallPair("@dump", (TextCall_CB_t)TextCall_Dump) );
    ASSERT_IF( db->AddCallPair("@dumpAll", (TextCall_CB_t)TextCall_DumpAll) );
    ASSERT_IF( db->AddCallPair("@call", (TextCall_CB_t)TextCall_Call) );
    ASSERT_IF( db->AddCallPair("@exit", (TextCall_CB_t)TextCall_Exit) );

    TextVar *var = db->textVarDB->AddVarPair("FuncHAHAHA");
    BASIC_ASSERT(var != NULL);
    var->pUniVar->Import((void *)FuncHAHAHA);
}

void TextCall_DefaultUninit(TextCallDB *db)
{
    SAFE_DELETE(db);
}

const char *gTestText[] = {
    "@var s8 i %x 0F 03 04 34 12 00 0A",
    " @_var u32 j 0x%x 0xFF123456 0xAABBCCDD",
    "     @dump j",
    "@call FuncHAHAHA i j",
    "@print \"abc haha = \" i[0-0xFF] \" next = \" 0x%08X FuncHAHAHA \" \" \"END\n\"",
};
void TextCall_Demo(void)
{
    MM_INIT();
    DUMPND(sizeof(gTestText)/sizeof(const char*));

    TextCallDB *db0 = new(TextCallDB);

    TextCall_DefaultInit(db0);
    //db0->Dump();
    TextCall_BasicInit(db0);

    FOR_I(sizeof(gTestText)/sizeof(const char*)) {
        db0->Start(gTestText[i]);
    }

    DUMPNA(FuncHAHAHA);
    //db0->textVarDB->Dump();
    TextCall_DefaultUninit(db0);

    MM_UNINIT();

    

    /*
        next:
            TextVar -> global var's & var map
            Class TextVar -> global var's & enhanced variable from univar & scan state machine & TextVar are all u8 array!!!!!
            Class TextVarDB -> var map
    */
}

