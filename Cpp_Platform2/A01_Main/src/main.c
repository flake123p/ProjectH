
#include "Everything_App.hpp"

void UartRx(void)
{
	LibUart_SniffSetting(1);
	
	LibUartClass uart(NULL, 8000);
	uart.comPortName = "COM30";
	uart.baudRate = 115200;
	uart.InitComPort(3);
	uart.ReceiveWithLength(2000, 2000);
	uart.UninitComPort();
}

int main(int argc, char *argv[])
{
	//LibArgParser_OptionSet_Demo();
	//LibPacket_Demo();
	//LibEncrypt_Demo();
	//LibSysCmd_SimpleCmd("start /B entry.bat 1233");
	//LibOs_SleepSeconds(2);
	//LibSysCmd_SimpleCmd("start /B entry.bat 1235");
	//LibSysCmd_Simple("python test.py");

	UartRx();

	return 0;
}

