

#ifndef _LIB_UART_HPP_INCLUDED_

#include <stdint.h>
#include <stddef.h> //for NULL
#include "My_Types.h"

// ============================== Debug ==============================
#define UART_LOG  (0)
#define UART_WARN (1)
#define UART_ERR  (0)

#if UART_LOG
#define UART_LOG_MSG printf
#else
#define UART_LOG_MSG(...)
#endif

#if UART_WARN
#define UART_WARN_MSG printf
#else
#define UART_WARN_MSG(...)
#endif

#if UART_ERR
#define UART_ERR_MSG printf
#else
#define UART_ERR_MSG(...)
#endif

// ============================== Manager ==============================
int LibUartMgr_DemoTxRx(void);
void LibUartMgr_GetComPortConfigFromFile(const char *comPortNameFile, OUT char *strComPortName, OUT uint32_t *baudRate = NULL);
int LibUartMgr_Receive_WaitData(uint8_t *buffer, uint32_t *receivedLength, uint32_t miliSeconds = 10000);

// ============================== Library ==============================
typedef enum {
	STOP_BITS_1,
	STOP_BITS_1_5,
	STOP_BITS_2,
} STOP_BITS;

typedef enum {
	PARITY__NONE,
	PARITY__ODD,
	PARITY__EVEN,
	PARITY__MARK,
	PARITY__SPACE,
} PARITY;

int LibUart_InitComPort(const char *comPortName, uint32_t baudRate, uint8_t byteSize = 8, STOP_BITS stopBits = STOP_BITS_1, PARITY parity = PARITY__NONE);
int LibUart_UninitComPort(void);
int LibUart_Send(uint8_t *buffer, uint32_t length);
int LibUart_Receive(uint8_t *buffer, uint32_t *receivedLength);


#define _LIB_UART_HPP_INCLUDED_
#endif//_LIB_UART_HPP_INCLUDED_
