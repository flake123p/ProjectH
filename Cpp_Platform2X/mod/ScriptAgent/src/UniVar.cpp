

#include "Everything_ThisMod.hpp"

#if 0
void UniVar::Clear(int retainBuf /* = 0 */) {
    if (type == VAR_CPP_STRING) {
        SAFE_DELETE((std::string *)hdl);
    } else {
        if (retainBuf) {
            LibAry *libAry = (LibAry *)hdl;
            libAry->len = 0;
        } else {
            SAFE_DELETE((LibAry *)hdl);
        }
    }
};
#endif
void UniVar::UniVarDump(void)
{
    DUMPX(type);
    printf("[%s]\n", GetUniTypeString(type));
    Dump();
    if (type & VAR_IS_C_STRING) {
        printf("var is: %s\n", (char *)ptr);
    } else if (type & VAR_IS_CPP_STRING) {
        printf("var is: %s\n", (*((std::string **)ptr))->c_str());
    }
}

void UniVar_Demo(void)
{
    MM_INIT();
    {
        UniVar x;
        std::string cppStr = "abc123";
        const char *cStr = "hello123";
        x.Import(&cppStr);
        x.UniVarDump();
        x.Reset();
        x.Import(cStr);
        x.UniVarDump();
    }
    MM_UNINIT();
}

