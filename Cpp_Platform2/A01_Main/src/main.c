/*
    TODO:
        1.task message send/receive                [LibHiTask.cpp]
            create task & trigger event
            task go with message
            wait task done with comeback message
        2.thread-safe LibDesc                      [LibHiTask.cpp]
        3.task pool (enable true multi-threading)  [LibHiTask.cpp]

        libDesc LibUtil and ... add init mutex for hTask
        LibVCD
        Mod:SimClk SimChnl SimBT SimUSB SimWIFI Sim4G Sim5G Sim6G SimZB
        SimClk -> SimParalell -> SimRTOS
        OS sim? task sim?

Lib_TotalInit();
Lib_TotalUninit();
        Enable MT:
        int main()
        {
            LibMT_Init();
            LibVCD_Init();
            LibVCD_Unit();
            LibMT_Uninit();
        }
*/

#include "Everything_App.hpp"

static int gIsMultiThreadingEnable = 0;

int LibMT_InitMultiThreading(void)
{
    return 0;
}

int LibMT_UninitMultiThreading(void)
{
    return 0;
}

int LibMT_IsMultiThreadingEnable(void)
{
    return gIsMultiThreadingEnable;
}

typedef enum {
    TIME_UNIT_PS,
    TIME_UNIT_NS,
    TIME_UNIT_US,
    TIME_UNIT_S,
}Time_Unit_t;

typedef struct {
    u32 num_of_bits;
    const char wire_name;
} LIB_VCD_WIRE_INFO_t;

typedef struct {
    char ch_symbol;
    u32 num_of_bits;
    const char wire_name;
} LIB_VCD_WIRE_INFO_INT_t;

static int gIs_LibVCD_Using_MT = 0;

int LibVCD_Init(u32 timescale, Time_Unit_t unit, LIB_VCD_WIRE_INFO_t *info, u32 num_of_info)
{
    printf("%s()\n", __func__);
    gIs_LibVCD_Using_MT = LibMT_IsMultiThreadingEnable();

    
    return 0;
}


int main(int argc, char *argv[])
{
    Lib_Init(ENABLE_NONE);

    LibMem_Demo();
#define XBA
    LibDesc_Demo();
    //DUMPND(LibUtil_GetUniqueU32());
    //DUMPND(LibUtil_GetUniqueU32());
    //DUMPND(LibUtil_GetUniqueU32());
    //DUMPND(LibUtil_GetUniqueU32());

    Lib_Uninit();
    return 99;
}


