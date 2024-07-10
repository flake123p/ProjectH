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

    20200202
        2+ process simulator is required
        self/predetermined execution time is required
        use single thread in core scheme
        porting BlueZ to my simulator
        It should be able to interact with human (interact mode - HOW?, target is 2 blueZ commuticate to each other)
            What would VCD dump work like in interact mode??
        How to simulate dozens peers scheme (MESH)
        get my note!!!!!!!!!!!!!!!!!!!!!!!

    VCD dump: LA like, trigger condition & trigger position

    USB
        L1 - SYNC / PID - "End point"
        L2 - pipe (4 transfers)
*/

#include "Everything_App.hpp"




int main(int argc, char *argv[])
{
#if ( 0 )
    Lib_Init();
#else
    Lib_Init(LIB_MT_ENABLE);
#endif

    //LibDesc_DemoMT();

    //LibU64_Demo();
    //LibVCD_LA_Demo();
    LibUtil_SimpleDynamicMemDemo();

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


