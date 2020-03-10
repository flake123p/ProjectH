
#include "ABT02_UartTester.hpp"


#define VERBOSE ( 1 )

LibInit gLibInit = LibInit(LIB_MT_ENABLE, VERBOSE);
HciUart gHciUart_IUT = HciUart(VERBOSE);
HciUart gHciUart_Tstr1 = HciUart(VERBOSE);
HciUart gHciUart_Tstr2 = HciUart(VERBOSE);



int main(int argc, char *argv[])
{
    gHciUart_IUT.Init("Uart_IUT.ini");
    gHciUart_Tstr1.Init("Uart_Tester1.ini");
    gHciUart_Tstr2.Init("Uart_Tester2.ini");

    
    //gHciUart_IUT.SniffSetting(1);

    while(0) {
        gHciUart_IUT.RawReceive();
        NEWLINE;
    }

    return 0;
}



