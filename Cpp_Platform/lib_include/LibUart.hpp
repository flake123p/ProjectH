

#ifndef _LIB_UART_HPP_INCLUDED_

#include <stdint.h>

// ============================== Debug ==============================
#define UART_LOG  (1)
#define UART_WARN (1)
#define UART_ERR  (1)

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
int LibUart_Send(uint32_t length, uint8_t *buffer);
int LibUart_Receive(uint32_t *receivedLength, uint8_t *buffer);

#define _LIB_UART_HPP_INCLUDED_
#endif//_LIB_UART_HPP_INCLUDED_
