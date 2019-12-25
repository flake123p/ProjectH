

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

// ============================== Define ==============================

// ============================== Manager ==============================
int LibUartMgr_DemoTxRx(void);
int LibUartMgr_GetComPortConfigFromFile(const char *comPortNameFile, OUT char *strComPortName, OUT uint32_t *baudRate = NULL);
int LibUartMgr_GetComPortConfigFrom_INI_File(const char *comPortNameFile, OUT char *strComPortName, OUT uint32_t *baudRate = NULL);
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

int LibUart_SniffSetting(int val);
int LibUart_InitComPort(const char *comPortName, uint32_t baudRate, int quickReadLevel = 0, uint8_t byteSize = 8, STOP_BITS stopBits = STOP_BITS_1, PARITY parity = PARITY__NONE);
int LibUart_UninitComPort(void);
int LibUart_Send(uint8_t *buffer, uint32_t length);
int LibUart_Receive(uint8_t *buffer, uint32_t *receivedLength);
int LibUart_ReceiveEx(uint8_t *buffer, uint32_t *receivedLength, uint32_t singleReadlength);

typedef void * UART_HANDLE_t;

// Windows / Linux adaptor
UART_HANDLE_t LibUartHdl_HandleCreate(void);
int LibUartHdl_HandleDestroy(UART_HANDLE_t hdl);

int LibUartHdl_SniffSetting(UART_HANDLE_t hdl, int val);
int LibUartHdl_InitComPort(UART_HANDLE_t hdl, const char *comPortName, uint32_t baudRate, int quickReadLevel = 0, uint8_t byteSize = 8, STOP_BITS stopBits = STOP_BITS_1, PARITY parity = PARITY__NONE);
int LibUartHdl_UninitComPort(UART_HANDLE_t hdl);
int LibUartHdl_Send(UART_HANDLE_t hdl, uint8_t *buffer, uint32_t length);
int LibUartHdl_Receive(UART_HANDLE_t hdl, uint8_t *buffer, uint32_t *receivedLength);
int LibUartHdl_ReceiveEx(UART_HANDLE_t hdl, uint8_t *buffer, uint32_t *receivedLength, uint32_t singleReadlength);


#include <string>
class LibUartClass {
public:
	std::string comPortName;
	u32 baudRate;
	u32 rxBufLen;
	u8 *rxBuf;
	u32 receivedLen;

	LibUartClass(const char *comPortNameFile = NULL, u32 rx_buf_len = 0);
	~LibUartClass(void);
	int GetComPortConfigFromFile(const char *comPortNameFile);
	int GetComPortConfigFrom_INI_File(const char *comPortNameFile);
	int InitRxBuffer(u32 rx_buf_len);
	int InitComPort(int quickReadLevel = 0);
	int UninitComPort(void);
	int Send(u8 *buffer, u32 length);
	int SendAndReceive(u8 *buffer, u32 length);
	int Receive(void);
	int ReceiveEx(u32 singleReadlength, u32 bufOffset = 0);
	int ReceiveWithLength(u32 maxRxLen, u32 singleReadlength = 80);
};

void UartRx_Simple_Demo(void);

#define _LIB_UART_HPP_INCLUDED_
#endif//_LIB_UART_HPP_INCLUDED_
