

#ifndef _LIB_UART_HPP_INCLUDED_

#include <stdint.h>
#include <stddef.h> //for NULL

// ============================== Debug ==============================
#define UART_LOG  (0)
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
void LibUartMgr_GetComPortNameFromFile(const char *comPortNameFile, char *strComPortName);

// This class is in test phase!!
class LibUartMgr_BasicUartClass{
public:
	char com_port_name[20];
	uint32_t baud_rate;
	uint8_t  rx_buf[300];
	uint32_t rx_len;
	
    LibUartMgr_BasicUartClass(uint32_t input_baud_rate, const char *input_com_port_name = NULL, const char *com_port_name_file = NULL);
	void RunTxRx(uint8_t *tx_buf, uint32_t tx_len);
};

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
int LibUart_Receive_WaitData(uint8_t *buffer, uint32_t *receivedLength);

#define _LIB_UART_HPP_INCLUDED_
#endif//_LIB_UART_HPP_INCLUDED_
