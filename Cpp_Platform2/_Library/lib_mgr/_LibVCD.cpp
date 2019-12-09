
#include "Everything_Lib_Mgr.hpp"

typedef struct {
    LIB_VCD_WIRE_INFO_t ori;
    char ch_symbol;
} _LIB_VCD_WIRE_INFO_INT_t;

int LibVCD_Init(u32 timescale, Time_Unit_t unit, LIB_VCD_WIRE_INFO_t *info, u32 num_of_info)
{
    printf("%s()\n", __func__);

    return 0;
}

int LibVCD_Uninit(void)
{
    printf("%s()\n", __func__);

    return 0;
}

void LibVCD_Demo(void)
{
    printf("%s()\n", __func__);
}