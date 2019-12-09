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





int main(int argc, char *argv[])
{
    Lib_Init();

    //LibMem_Demo();
#define XBA
    LibVCD_Demo();
    //DUMPND(LibUtil_GetUniqueU32());
    //DUMPND(LibUtil_GetUniqueU32());
    //DUMPND(LibUtil_GetUniqueU32());
    //DUMPND(LibUtil_GetUniqueU32());

    Lib_Uninit();
    return 99;
}


