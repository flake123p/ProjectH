
#include "Everything_Lib_Mgr.hpp"


int Lib_Init(void)
{
    MEM_INIT(); //keep this at first
    return 0;
}

int Lib_Uninit(void)
{
    MEM_UNINIT(); //keep this at last
    return 0;
}

