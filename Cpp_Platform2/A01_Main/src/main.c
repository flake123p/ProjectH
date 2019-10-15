
#include "Everything_App.hpp"
//#include "unix_sys_queue.h"

#if 0
void UartRx(void)
{
	LibUart_SniffSetting(1);
	
	LibUartClass uart(NULL, 8000);
	uart.comPortName = "COM26";
	uart.baudRate = 115200;
	uart.InitComPort(3);
	//
	//uart.ReceiveWithLength(2, 2000);
	//
    u8 sendBuf[] = {0x01, 0x03, 0x0C, 0x00};
	uart.Send(sendBuf, sizeof(sendBuf));
	uart.ReceiveWithLength(2, 2000);
	uart.UninitComPort();
}
#endif

u8 hdl_map = 0xFF;
u32 hdl[8];
int GetNewHandle(void)
{
    int ret = LibUtil_GetTrueBitIndexOfU8(hdl_map);

    if(ret < 0)
        return ret;

    CLEAR_BIT(hdl_map, ret);
    hdl[ret] &= 0x7FFFFFFF;
    hdl[ret] += 0x1000;
    return ret;
}
void dump(void)
{
    for(int i=0; i<8; i++)
    {
        printf("0x%08X ", hdl[i]);
        if(i%4==3)
            printf("\n");
    }
}
int main(int argc, char *argv[])
{
    LibDesc_Demo();

    for(int i=0; i<8; i++)
    {
        hdl[i] = 0x80000000 + i;
    }
    dump();
    int newHdl = GetNewHandle();
    printf("newHdl = %d, hdl_map = 0x%02X\n", newHdl, hdl_map);dump();
    newHdl = GetNewHandle();
    printf("newHdl = %d, hdl_map = 0x%02X\n", newHdl, hdl_map);dump();
    newHdl = GetNewHandle();
    printf("newHdl = %d, hdl_map = 0x%02X\n", newHdl, hdl_map);dump();
    newHdl = GetNewHandle();
    printf("newHdl = %d, hdl_map = 0x%02X\n", newHdl, hdl_map);dump();
    newHdl = GetNewHandle();
    printf("newHdl = %d, hdl_map = 0x%02X\n", newHdl, hdl_map);dump();
    hdl_map += 1;
    newHdl = GetNewHandle();
    printf("newHdl = %d, hdl_map = 0x%02X\n", newHdl, hdl_map);dump();
    return 99;
}


