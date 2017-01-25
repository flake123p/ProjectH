
// ====== Standard C/Cpp Library ======
#include <stdio.h>
#include <string.h>
#include <stdint.h> // for uint32_t ...

// ====== Platform Library ======
#include "LibUart.hpp"
#include "LibFileIO.hpp"
#include "My_Basics.hpp"

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

	LibUart_Send(sizeof(lpBuffer), lpBuffer);
	LibUart_Receive(&receivedLegnth, gRxBuffer);
	UART_LOG_MSG(">>> receivedLegnth = %d\n", receivedLegnth);
	
	LibUart_Send(sizeof(lpBuffer), lpBuffer);
	LibUart_Receive(&receivedLegnth, gRxBuffer);
	UART_LOG_MSG(">>> receivedLegnth = %d\n", receivedLegnth);
	
	LibUart_UninitComPort();
	return 0;
}

void LibUartMgr_GetComPortNameFromFile(const char *comPortNameFile, char *strComPortName)
{
	int retVal;
	
	LibFileIoClass file_ComPortName(comPortNameFile, "r");
	BASIC_ASSERT(file_ComPortName.IsFileExist() == true);
	file_ComPortName.FileOpen();

	retVal = fscanf(file_ComPortName.fp, "%s", strComPortName);
	BASIC_ASSERT(retVal > 0);

	file_ComPortName.FileClose();
}

LibUartMgr_BasicUartClass::LibUartMgr_BasicUartClass(uint32_t input_baud_rate, const char *input_com_port_name, const char *com_port_name_file)
{
	com_port_name[0] = 0;
	baud_rate = input_baud_rate;
	rx_len = 0;

	if (input_com_port_name != NULL)
		strcpy(com_port_name, input_com_port_name);

	if (com_port_name_file != NULL) {
		LibUartMgr_GetComPortNameFromFile(com_port_name_file, com_port_name);
	}
}

void LibUartMgr_BasicUartClass::RunTxRx(uint32_t tx_len, uint8_t *tx_buf)
{
	BASIC_ASSERT(this->com_port_name[0] != 0);
	LibUart_InitComPort(this->com_port_name, this->baud_rate);

	LibUart_Send(tx_len, tx_buf);
	LibUart_Receive(&(this->rx_len), this->rx_buf);

	LibUart_UninitComPort();
	return;
}
