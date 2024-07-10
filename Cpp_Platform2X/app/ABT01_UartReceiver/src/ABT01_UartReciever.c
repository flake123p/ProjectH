
#include "ABT01_UartReciever.hpp"

#define VERBOSE ( 1 )

LibInit gLibInit = LibInit(LIB_MT_ENABLE, VERBOSE);
HciUart gHciUart = HciUart(VERBOSE);

int main(int argc, char *argv[])
{
    gHciUart.Init("Uart.ini");
    //gHciUart.SniffSetting(1);

    while(1) {
        gHciUart.RawReceive();
        NEWLINE;
    }

    return 0;
}



