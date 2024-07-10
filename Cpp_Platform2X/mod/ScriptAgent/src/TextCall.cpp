
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
    int ret = 0;
    int ret_from_cb;

    // 1. get first word
    LibStringClass *strAgent = new(LibStringClass);
    strAgent->Init(line);
    strAgent->RemoveRestStringByChar('#');
    strAgent->Split(true);
    //strAgent.Dump();

    do {
        if (strAgent->subStrVector.size() < 1) {
            ret = 1;
            break;
        }

        if (strAgent->subStrVector[0][0] == '#') {
            if (cbRet != NULL) {
                *cbRet = TXT_CALL_RET_COMMENT_LINE;
            }
            ret = 2;
            break;
        }

        if (skipEnable) {
            if (strAgent->subStrVector[0] == skipOverString) {
                skipEnable = 0;
                skipOverString = "";
            }
            ret = 3;
            break;
        }

        std::map<std::string,TextCall_t>::iterator mapIt;

        mapIt = callbackMap.find(strAgent->subStrVector[0]);
        if (mapIt == callbackMap.end()) {
            //LibError_SetExtErrorMessage("Can't find section: %s\n", secName.c_str());
            printf("Unknown TextCall keywoard : %s\n", strAgent->subStrVector[0].c_str());
            ret = 4;
            break;
        }

        TextCall_t textCall = mapIt->second;
        ret_from_cb = (*(textCall.cb))(textCallDB, strAgent, textCall.userHdl_0, textCall.userHdl_1);
        if (cbRet != NULL) {
            *cbRet = ret_from_cb;
        }

        if (TXT_CALL_RET_ERROR_LOG == ret_from_cb) {
            printf("\n[ERROR in file : %s, Line : %d]\n", currFile.c_str(), currFileLine);
        }
    } while (0) ;
    delete(strAgent);
    return ret;
}

/*
Variable declare: @var <name> <(0)u8/u32...> %d 7 8 9
    @var i u32 %d 9
    @var x str "ffddaa"
    @var x "ffddaa"
*/
int TextCall_Var(TextCallDB *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    //PRINT_FUNC;
    //u32 *x = (u32 *)userHdl_0;
    //DUMPND(*x);
    //splitedStrAgent->Dump();
    //return 0;

    u32 type = VAR_IS_UNINITED;
    std::string scanFormat = "";

    TextVar *var = textCallDB->textVarDB->AddVarPair(&(splitedStrAgent->subStrVector[1]));
    if (var == NULL) {
        printf("Duplicate var name:%s\n", splitedStrAgent->subStrVector[1].c_str());
        BASIC_ASSERT(0);
    }
    var->name = splitedStrAgent->subStrVector[1];

    size_t i = 0xFFFFFFFF;
    if (splitedStrAgent->subStrFlagVector[2] & LIB_STR_SUB_DOUBLE_QUOTE) {
        i= 2;
        type = VAR_C_STRING;
    } else {
        int isSigned;
        // simple case without type (default:u32/s32)
        // @var i %d 9
        if (LibString_IsPrintPattern(&(splitedStrAgent->subStrVector[2]), &isSigned)) {
            if (isSigned) {
                type = VAR_S32;
            } else {
                type = VAR_U32;
            }
            i = 3;
            scanFormat = splitedStrAgent->subStrVector[2];
        } else if (TextVar_GetTypeFromString(&(splitedStrAgent->subStrVector[2]), &type)) {
            //@var x str "ffddaa"
            if (type == VAR_C_STRING) {
                i = 3;
            } else {
                if (LibString_IsPrintPattern(&(splitedStrAgent->subStrVector[3]), &isSigned)) {
                    i = 4;
                    scanFormat = splitedStrAgent->subStrVector[3];
                } else {
                    printf("Wrong var format (%d)\n", __LINE__);
                    return 1;
                }
            }
        } else {
            printf("Wrong var format (%d)\n", __LINE__);
            return 1;
        }
    }
    BASIC_ASSERT(type != VAR_IS_UNINITED);
    BASIC_ASSERT(i != 0xFFFFFFFF);

    if (type & VAR_C_STRING) {
        var->pUniVar->Import((const char *)splitedStrAgent->subStrVector[i].c_str());
    } else {
        BASIC_ASSERT(scanFormat.size() != 0);
        u32 temp;
        for (; i < splitedStrAgent->subStrVector.size(); i++) {
            sscanf(splitedStrAgent->subStrVector[i].c_str(), scanFormat.c_str(), &temp);
            switch (type) {
                case VAR_U8:
                    var->pUniVar->Import((u8)temp);
                    break;
                case VAR_U16:
                    var->pUniVar->Import((u16)temp);
                    break;
                case VAR_U32:
                    var->pUniVar->Import((u32)temp);
                    break;
                case VAR_S8:
                    var->pUniVar->Import((s8)temp);
                    break;
                case VAR_S16:
                    var->pUniVar->Import((s16)temp);
                    break;
                case VAR_S32:
                    var->pUniVar->Import((s32)temp);
                    break;
                default:
                    printf("Wrong var format (%d)\n", __LINE__);
                    return 1;
            }
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

