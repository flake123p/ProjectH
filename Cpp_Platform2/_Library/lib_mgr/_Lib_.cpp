
#include "Everything_Lib_Mgr.hpp"

static LIB_FLAG_t gLibFlags = LIB_NONE_ENABLE;

int Lib_Init(LIB_FLAG_t flags /* = LIB_NONE_ENABLE */)
{
    gLibFlags = flags; //keep this before first

    MM_INIT(); //keep this at first

    if (FLG_CHK(gLibFlags, LIB_MT_ENABLE)) {
        LibMT_Init();
    }

    LibUtil_Init();

    return 0;
}

int Lib_Uninit(void)
{
    LibUtil_Uninit();

    if (FLG_CHK(gLibFlags, LIB_MT_ENABLE)) {
        LibMT_Uninit();
    }

    MM_UNINIT(); //keep this at last
    return 0;
}

int Lib_IsMT(void)
{
    return FLG_CHK(gLibFlags, LIB_MT_ENABLE) ? 1 : 0;
}
