
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

int LibUartMgr_GetComPortConfigFromFile(const char *comPortNameFile, OUT char *strComPortName, OUT uint32_t *baudRate /* = NULL */)
{
	int retVal;
	
	LibFileIoClass file_ComPortName(comPortNameFile, "r");

	retVal = file_ComPortName.FileOpen();
	RETURN_IF(retVal);

	retVal = fscanf(file_ComPortName.fp, "ComPortName=%s", strComPortName);
	if (retVal == 0) {
		printf("ComPortName format error!!\n");
		return RC_FILE_SCAN_ERROR;
	}

	if (baudRate != NULL) {
		retVal = fscanf(file_ComPortName.fp, "\nBaudRate=%u", baudRate);
		if (retVal == 0) {
			printf("BaudRate format error!!\n");
			return RC_FILE_SCAN_ERROR;
		}
	}

	file_ComPortName.FileClose();

	return 0;
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

LibUartClass::LibUartClass(const char *comPortNameFile /* = NULL */, u32 rx_buf_len /* = 0 */)
{
	baudRate = 0;
	rxBuf = NULL;

	if (comPortNameFile != NULL)
		GetComPortConfigFromFile(comPortNameFile);
	else
		comPortName = "";

	InitRxBuffer(rx_buf_len);
}

LibUartClass::~LibUartClass(void)
{
	if (rxBuf != NULL) {
		free(rxBuf);
		rxBuf = NULL;
	}
}

int LibUartClass::GetComPortConfigFromFile(const char *comPortNameFile)
{
	char str[30] = {0};
	int retVal;

	retVal = LibUartMgr_GetComPortConfigFromFile(comPortNameFile, str, &baudRate);
	RETURN_IF(retVal);
	
	comPortName = str;
	
	return 0;
}

int LibUartClass::InitRxBuffer(u32 rx_buf_len)
{
	if (rxBuf != NULL) {
		BASIC_ASSERT(0);
	}
	
	rxBufLen = rx_buf_len;

	if (rxBufLen != 0) {
		rxBuf = (u8 *)malloc(rxBufLen);
	}
	
	return 0;
}

int LibUartClass::InitComPort(int quickReadLevel /* = 0 */)
{
	return LibUart_InitComPort(comPortName.c_str(), baudRate, quickReadLevel);
}

int LibUartClass::UninitComPort(void)
{
	return LibUart_UninitComPort();
}

int LibUartClass::Send(u8 *buffer, u32 length)
{
	return LibUart_Send(buffer, length);
}

int LibUartClass::SendAndReceive(u8 *buffer, u32 length)
{
	int retVal = LibUart_Send(buffer, length);
	RETURN_IF(retVal);

	return Receive();
}

int LibUartClass::Receive(void)
{
	return LibUart_Receive(rxBuf, &receivedLen);
}

int LibUartClass::ReceiveEx(u32 singleReadlength, u32 bufOffset /* = 0 */)
{
	return LibUart_ReceiveEx(ADDRX(rxBuf, bufOffset), &receivedLen, singleReadlength);
}

int LibUartClass::ReceiveWithLength(u32 maxRxLen)
{
	int retVal;
	u32 totalReceivedLen = 0;

	while (totalReceivedLen < maxRxLen) {
		RETURN_CHK( retVal, this->ReceiveEx(80, totalReceivedLen) );
		
		totalReceivedLen += this->receivedLen;
	}

	this->receivedLen = totalReceivedLen;
	
	return 0;
}
