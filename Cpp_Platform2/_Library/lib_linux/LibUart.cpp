
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

int LibUart_InitComPort(const char *comPortName, uint32_t baudRate, uint8_t byteSize, STOP_BITS stopBits, PARITY parity)
{
	return 0;
}

int LibUart_UninitComPort(void)
{
	return 0;
}

int LibUart_Send(uint8_t *buffer, uint32_t length)
{
	return 0;
}

int LibUart_Receive(uint8_t *buffer, uint32_t *receivedLength)
{
	return 0;
}

int LibUart_Receive_WaitData(uint8_t *buffer, uint32_t *receivedLength)
{
	return 0;
}
