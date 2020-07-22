
#include "Everything_ThisMod.hpp"

void TextCall::Dump(void)
{
    printf("=============== TextCall DUMP ============= \\\\\\\\\\\\\n");

    printf("Dump callback map:\n");
    std::map<std::string,TextCall_t>::iterator it;
    for (it=callbackMap.begin(); it!=callbackMap.end(); ++it) {
        printf("%s ... \n", it->first.c_str());
    }
}

int TextCall::AddPair(const char *str, TextCall_CB_t cb, void *userHdl_0 /*= NULL*/, void *userHdl_1 /*= NULL*/)
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

int TextCall::Start(const char *line, int *cbRet /*= NULL*/)
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
        //printf("Unknown keywoard %s\n", strAgent.subStrVector[0].c_str());
        return 1;
    }

    TextCall_t textCall = mapIt->second;
    ret_from_cb = (*(textCall.cb))(&strAgent, textCall.userHdl_0, textCall.userHdl_1);
    if (cbRet != NULL) {
        *cbRet = ret_from_cb;
    }

    return 0;
}

int TestTextCB_Var(LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    PRINT_FUNC;
    u32 *x = (u32 *)userHdl_0;
    DUMPND(*x);
    return 0;
}

int TestTextCB_Dump(LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1)
{
    PRINT_FUNC;
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

void TextCall_Demo(void)
{
    const char testStr00[] = " @var u8 ary %x 01 03 04 34 12 00 0A";
    const char testStr01[] = " @_var u8 ary %x 01 03 04 34 12 00 0A";
    const char testStr02[] = "     @dump";
    u32 i=6, j=7;

    MM_INIT();

    PRINT_FUNC;

    TextCall *db0 = new(TextCall);
    ASSERT_IF( db0->AddPair("@var", TestTextCB_Var, &i, db0) );
    ASSERT_IF( db0->AddPair("@_var", TestTextCB_Var, &j, db0) );
    ASSERT_IF( db0->AddPair("@dump", TestTextCB_Dump) );
    db0->Dump();

    db0->Start(testStr00);
    db0->Start(testStr01);
    db0->Start(testStr02);

    SAFE_DELETE(db0);



    u16 gg = 0;
    gg = gg;
    printf("type is %s\n", GetUniTypeString(GetUniType(&gg, 8)));

    void *ggg = NULL;
    ggg = ggg;
    printf("type is %s\n", GetUniTypeString(GetUniType(ggg)));

    {
        UniVariable x,y,r,t;
        x.Init((void *)GetUniTypeString);
        x.dump();
        DUMPNP(GetUniTypeString);
        y.Copy(&x);
        y.dump();
        //void *tempPtr = (void *)GetUniType;
        r.Init((void *)GetUniTypeString);
        r.dump();
        t.Copy(&r);
        t.dump();
    }

    MM_UNINIT();

    /*
        next:
            TextVar -> global var's & var map
            Class TextVar -> global var's & enhanced variable from univar & scan state machine & TextVar are all u8 array!!!!!
            Class TextVarDB -> var map
    */
}

