/*

[v] 1. LibThread or LibIPC ?? => Mutex utility
[ ] 2. use mutex utility to update LibMem libUtil ...


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
*/

#include "Everything_App.hpp"

int main(int argc, char *argv[])
{
#if 1
    Lib_Init();
#else
    Lib_Init(LIB_MT_ENABLE);
#endif

    LibMem_Demo();

    //LibMT_UtilMutex_Demo();
    DUMPND(LibUtil_GetUniqueU32());
    DUMPND(LibUtil_GetUniqueU32());
    DUMPND(LibUtil_GetUniqueU32());
    DUMPND(LibUtil_GetUniqueU32());

    Lib_Uninit();
    return 99;
}


