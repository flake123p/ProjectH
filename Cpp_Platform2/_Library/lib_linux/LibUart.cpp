
// ====== Standard C/Cpp Library ======
#include <stdio.h>
#include <string.h>
#include <stdint.h> // for uint32_t ...

// ====== Standard Windows Library ======
//#include <Windows.h>

// ====== Basics ======
#include "My_Basics.hpp"

// ====== Platform Library ======
#include "LibUart.hpp"

UART_HANDLE_t LibUartHdl_HandleCreate(void)
{
    return NULL;
}

int LibUartHdl_HandleDestroy(UART_HANDLE_t hdl)
{
    return 0;
}

int LibUartHdl_SniffSetting(UART_HANDLE_t hdl, int val)
{
	return 0;
}

int LibUartHdl_InitComPort(UART_HANDLE_t hdl, const char *comPortName, uint32_t baudRate, int quickReadLevel /* = 0 */, uint8_t byteSize /* = 8 */, STOP_BITS stopBits /* = STOP_BITS_1 */, PARITY parity /* = PARITY__NONE */)
{
	return 0;
}

int LibUartHdl_Send(UART_HANDLE_t hdl, uint8_t *buffer, uint32_t length)
{
	return 0;
}

int LibUartHdl_Receive(UART_HANDLE_t hdl, uint8_t *buffer, uint32_t *receivedLength)
{
	return 0;
}

int LibUartHdl_UninitComPort(UART_HANDLE_t hdl)
{
	return 0;
}

int LibUartHdl_ReceiveEx(UART_HANDLE_t hdl, uint8_t *buffer, uint32_t *receivedLength, uint32_t singleReadlength)
{
	return 0;
}

