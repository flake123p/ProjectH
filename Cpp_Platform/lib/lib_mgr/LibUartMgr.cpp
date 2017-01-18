
// ====== Standard C/Cpp Library ======
#include <stdio.h>
#include <string.h>
#include <stdint.h> // for uint32_t ...

// ====== Platform Library ======
#include "LibUart.hpp"
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

