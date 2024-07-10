
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

	LibThreadMgr_Touch_WatchDog();
	LibUart_Send(lpBuffer, sizeof(lpBuffer));
	LibUart_Receive(gRxBuffer, &receivedLegnth);
	LibThreadMgr_Freeze_WatchDog();
	UART_LOG_MSG(">>> receivedLegnth = %d\n", receivedLegnth);

	LibThreadMgr_Touch_WatchDog();
	LibUart_Send(lpBuffer, sizeof(lpBuffer));
	LibUart_Receive(gRxBuffer, &receivedLegnth);
	LibThreadMgr_Freeze_WatchDog();
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

int LibUartMgr_GetComPortConfigFrom_INI_File(const char *comPortNameFile, OUT char *strComPortName, OUT uint32_t *baudRate /* = NULL */)
{
	int retVal;

	LibFile_INI file_ComPortName(comPortNameFile, "r+b");

	EXIT_CHK( retVal, file_ComPortName.StartParse() );
	//file_ComPortName.Dump();

	std::string valStr;
	retVal = file_ComPortName.GetValueString("[Connect]", "ComPortName", valStr);
	if (retVal) {
		printf("ComPortName format error!!\n");
		return retVal;
	} else {
		strcpy(strComPortName, valStr.c_str());
	}

	if (baudRate != NULL) {
		retVal = file_ComPortName.GetValueU32("[Connect]", "BaudRate", *baudRate);
		if (retVal) {
			printf("BaudRate format error!!\n");
			return retVal;
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

int LibUartClass::GetComPortConfigFrom_INI_File(const char *comPortNameFile)
{
	char str[30] = {0};
	int retVal;

	RETURN_CHK( retVal, LibUartMgr_GetComPortConfigFrom_INI_File(comPortNameFile, str, &baudRate) );

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

int LibUartClass::ReceiveWithLength(u32 maxRxLen, u32 singleReadlength /* = 80 */)
{
	int retVal;
	u32 totalReceivedLen = 0;

	while (totalReceivedLen < maxRxLen) {
		RETURN_CHK( retVal, this->ReceiveEx(singleReadlength, totalReceivedLen) );

		totalReceivedLen += this->receivedLen;
	}

	this->receivedLen = totalReceivedLen;

	return 0;
}

UART_HANDLE_t gLibUartMgrHdl;

int LibUart_SniffSetting(int val)
{
    return LibUartHdl_SniffSetting(gLibUartMgrHdl, val);
}

int LibUart_InitComPort(const char *comPortName, uint32_t baudRate, int quickReadLevel /* = 0 */, uint8_t byteSize /* = 8 */, STOP_BITS stopBits /* = STOP_BITS_1 */, PARITY parity /* = PARITY__NONE */)
{
    gLibUartMgrHdl = LibUartHdl_HandleCreate();
    return LibUartHdl_InitComPort(gLibUartMgrHdl, comPortName, baudRate, quickReadLevel, byteSize, stopBits, parity);
}

int LibUart_Send(uint8_t *buffer, uint32_t length)
{
    return LibUartHdl_Send(gLibUartMgrHdl, buffer, length);
}

int LibUart_Receive(uint8_t *buffer, uint32_t *receivedLength)
{
    return LibUartHdl_Receive(gLibUartMgrHdl, buffer, receivedLength);
}

int LibUart_UninitComPort(void)
{
    int ret = LibUartHdl_UninitComPort(gLibUartMgrHdl);
    LibUartHdl_HandleDestroy(gLibUartMgrHdl);
    return ret;
}

int LibUart_ReceiveEx(uint8_t *buffer, uint32_t *receivedLength, uint32_t singleReadlength)
{
    return LibUartHdl_ReceiveEx(gLibUartMgrHdl, buffer, receivedLength, singleReadlength);
}

void UartRx_Simple_Demo(void)
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
