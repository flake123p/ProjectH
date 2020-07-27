
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

int TextCallDB::AddPair(const char *str, TextCall_CB_t cb, void *userHdl_0 /*= NULL*/, void *userHdl_1 /*= NULL*/)
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
    strAgent.Split();

    BASIC_ASSERT(strAgent.subStrVector.size() >= 1);

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

int TestTextCB_Var(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    //PRINT_FUNC;
    //u32 *x = (u32 *)userHdl_0;
    //DUMPND(*x);
    //splitedStrAgent->Dump();

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

    return 0;
}

int TestTextCB_Dump(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    TextVar *var = textCallDB->textVarDB->Find(splitedStrAgent->subStrVector[1]);
    if (var == NULL) {
        printf("Unknown var name : %s\n", splitedStrAgent->subStrVector[1].c_str());
    } else {
        var->TextVarDump();
    }

    return 0;
}

int TestTextCB_DumpAll(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    textCallDB->textVarDB->Dump();

    return 0;
}

int TestTextCB_Call(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    //splitedStrAgent->Dump();

    if (splitedStrAgent->subStrVector.size() < 2) {
        printf("%s : no function name\n", splitedStrAgent->subStrVector[0].c_str());
    }

    TextVar *var = textCallDB->textVarDB->Find(splitedStrAgent->subStrVector[1]);
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

int FuncHAHAHA(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    PRINT_FUNC_LINE;

    return 0;
}

void TextCall_Start(const char *line)
{
    // 1. get first word
    LibStringClass strAgent = LibStringClass(line);
    strAgent.Split();

    if (strAgent.subStrVector.size() >= 1) {
        printf("[[[%s]]]\n", strAgent.subStrVector[0].c_str());
    }
    strAgent.Dump();

    // 2. find callback in map
}

void TextCall_BasicInit(TextCallDB *db, TextVarDB *varDB)
{
    db->textCallDB = db;
    db->textVarDB = varDB;
    ASSERT_IF( db->AddPair("@var", (TextCall_CB_t)TestTextCB_Var) );
    ASSERT_IF( db->AddPair("@_var", (TextCall_CB_t)TestTextCB_Var) );
    ASSERT_IF( db->AddPair("@dump", (TextCall_CB_t)TestTextCB_Dump) );
    ASSERT_IF( db->AddPair("@dumpAll", (TextCall_CB_t)TestTextCB_DumpAll) );
    ASSERT_IF( db->AddPair("@call", (TextCall_CB_t)TestTextCB_Call) );

    TextVar *var = varDB->AddPair("FuncHAHAHA");
    BASIC_ASSERT(var != NULL);
    var->pUniVar->Import((void *)FuncHAHAHA);
}

void TextCall_BasicUninit(TextCallDB *db, TextVarDB *varDB)
{
    //varDB->Dump();
    SAFE_DELETE(varDB);
    SAFE_DELETE(db);
}

void TextCall_Demo(void)
{
    const char testStr00[] = " @var u8 i %x 01 03 04 34 12 00 0A";
    const char testStr01[] = " @_var s8 j";
    const char testStr02[] = "     @dump x";
    const char testStr03[] = "@dump j";
    const char testStr04[] = "@call FuncHAHAHA i j";

    MM_INIT();

    TextCallDB *db0 = new(TextCallDB);
    TextVarDB *varDB = new(TextVarDB);
    TextCall_BasicInit(db0, varDB);
    //db0->Dump();

    db0->Start(testStr00);
    db0->Start(testStr01);
    db0->Start(testStr02);
    db0->Start(testStr03);
    db0->Start(testStr04);

    TextCall_BasicUninit(db0, varDB);

    MM_UNINIT();

    /*
        next:
            TextVar -> global var's & var map
            Class TextVar -> global var's & enhanced variable from univar & scan state machine & TextVar are all u8 array!!!!!
            Class TextVarDB -> var map
    */
}

