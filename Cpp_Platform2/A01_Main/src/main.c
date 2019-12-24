/*
    Ideas:
        1. Multi/Single Task switcher
        2. libDesc enhance to 2 category
            a. used between threads
            b. used in single thread to store job

    TODO:
        1. LibComm:
            a. compare array
            b. clock sync
        2. Mod:SimClk SimChnl SimBT SimUSB SimWIFI Sim4G Sim5G Sim6G SimZB
        SimClk -> SimParalell -> SimRTOS
        OS sim? task sim?
*/

#include "Everything_App.hpp"




int main(int argc, char *argv[])
{
#if ( 0 )
    Lib_Init();
#else
    Lib_Init(LIB_MT_ENABLE);
#endif

    LibComm_Demo();
//    LibMT_Demo();
//    LibTimer_DemoEx();
//    LibMT_UtilMutex_Demo();

    //LibTDD_Demo();
    //LibTime_DemoClockTimer2();
    
#if 0
    DUMPND(LibUtil_GetUniqueU32());
    DUMPND(LibUtil_GetUniqueU32());
    DUMPND(LibUtil_GetUniqueU32());
    DUMPND(LibUtil_GetUniqueU32());
#endif

    Lib_Uninit();
    return 0;
}


