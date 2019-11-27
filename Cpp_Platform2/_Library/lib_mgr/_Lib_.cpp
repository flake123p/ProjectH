
#include "Everything_Lib_Mgr.hpp"

LIB_FLAG_t gLibFlags = ENABLE_NONE;

int Lib_Init(LIB_FLAG_t initFlags)
{
    gLibFlags = initFlags;
    //LibMem_Init(void)
    return 0;
}

int Lib_Uninit(void)
{
    return 0;
}

