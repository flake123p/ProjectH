
#include "Everything_Lib_Mgr.hpp"

#define COM_PORT_NAME  "COM17"
#define BAUD_RATE      (115200)
#define TX_BUFFER_SIZE (100)
#define RX_BUFFER_SIZE (100)

uint8_t gTxBuffer[TX_BUFFER_SIZE];
uint8_t gRxBuffer[RX_BUFFER_SIZE];

unsigned char   lpBuffer[] = {0x01, 0x09, 0xFC, 0x03, 0x2B, 0x1B, 0x03};

int LibUartMgr_DemoTxRx(void)
{
	uint32_t receivedLegnth = 0;
	LibUart_InitComPort(COM_PORT_NAME, BAUD_RATE);

	LibUart_Send(lpBuffer, sizeof(lpBuffer));
	LibUart_Receive(gRxBuffer, &receivedLegnth);
	UART_LOG_MSG(">>> receivedLegnth = %d\n", receivedLegnth);
	
	LibUart_Send(lpBuffer, sizeof(lpBuffer));
	LibUart_Receive(gRxBuffer, &receivedLegnth);
	UART_LOG_MSG(">>> receivedLegnth = %d\n", receivedLegnth);
	
	LibUart_UninitComPort();
	return 0;
}

void LibUartMgr_GetComPortConfigFromFile(const char *comPortNameFile, OUT char *strComPortName, OUT uint32_t *baudRate /* = NULL */)
{
	int retVal;
	
	LibFileIoClass file_ComPortName(comPortNameFile, "r");
	BASIC_ASSERT(file_ComPortName.IsFileExist() == true);
	file_ComPortName.FileOpen();

	retVal = fscanf(file_ComPortName.fp, "ComPortName=%s", strComPortName);
	BASIC_ASSERT(retVal > 0); // file format error

	if (baudRate != NULL) {
		retVal = fscanf(file_ComPortName.fp, "\nBaudRate=%u", baudRate);
		BASIC_ASSERT(retVal > 0); // file format error
	}

	file_ComPortName.FileClose();
}

int LibUartMgr_Receive_WaitData(uint8_t *buffer, uint32_t *receivedLength, uint32_t miliSeconds /* = 10000 */)
{
	int retVal;
	uint32_t retryLoop;

	if (miliSeconds) {
		retryLoop = (miliSeconds / 50) + 1;
	}
	
	retVal = LibUart_Receive(buffer, receivedLength);
	if (retVal) 
		return retVal;

	while (*receivedLength == 0) {
		UART_LOG_MSG("\n%s() Keep waiting the receiving data.\n", __func__);
		retVal = LibUart_Receive(buffer, receivedLength);
		if (retVal) 
			return retVal;
		
		if (retryLoop == 0) {
			break;
		} else {
			retryLoop--;
			LibOs_SleepMiliSeconds(50);
		}
	}

	return retVal;
}
