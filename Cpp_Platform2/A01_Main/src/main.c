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
        
*/

#include "Everything_App.hpp"
#define XBAA

int main(int argc, char *argv[])
{
    //LibUtile_Demo2();
#define XBA
    LibDesc_Demo();
    //DUMPND(LibUtil_GetUniqueU32());
    //DUMPND(LibUtil_GetUniqueU32());
    //DUMPND(LibUtil_GetUniqueU32());
    //DUMPND(LibUtil_GetUniqueU32());
    return 99;
}


