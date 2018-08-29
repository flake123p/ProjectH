
#include "Everything_App.hpp"

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

int main(int argc, char *argv[])
{
	//LibArgParser_OptionSet_Demo();
	//LibPacket_Demo();
	LibEncrypt_Demo();
	//LibSysCmd_SimpleCmd("start /B entry.bat 1233");
	//LibOs_SleepSeconds(2);
	//LibSysCmd_SimpleCmd("start /B entry.bat 1235");
	//LibSysCmd_Simple("python test.py");

	//UartRx();

	return 0;
}

