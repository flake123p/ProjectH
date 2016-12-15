
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
int LibUart_InitComPort(const char *comPortName, unsigned int baudRate);
int LibUart_UninitComPort(void);
int LibUart_Send(uint32_t length, uint8_t *buffer);
int LibUart_Receive(uint32_t *receivedLength, uint8_t *buffer);
