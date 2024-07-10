

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

int UniVar::GetSingleElement32(u32 index, u32 *out)
{
    u32 value;

    if (index >= MaxNum()) {
        return 1;
    }

    if (type & VAR_IS_8BITS) {
        value = (u32)(((u8 *)ptr)[index]);
    } else if (type & VAR_IS_16BITS) {
        value = (u32)(((u16 *)ptr)[index]);
    } else if (type & VAR_IS_32BITS) {
        value = (u32)(((u32 *)ptr)[index]);
    } else {
        value = (u32)((int **)ptr)[index];
    }

    *out = value;
    return 0;
}

int UniVar::SetSingleElement32(u32 index, u32 in)
{
    if (index >= MaxNum()) {
        return 1;
    }

    if (type & VAR_IS_8BITS) {
        ((u8 *)ptr)[index] = (u8)in;
    } else if (type & VAR_IS_16BITS) {
        ((u16 *)ptr)[index] = (u16)in;
    } else if (type & VAR_IS_32BITS) {
        ((u32 *)ptr)[index] = (u32)in;
    } else {
        //TBD
    }
    return 0;
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

