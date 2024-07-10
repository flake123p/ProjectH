
// ====== Standard C/Cpp Library ======
#include <stdio.h>
#include <string.h>
#include <stdint.h> // for uint32_t ...

// ====== Standard Windows Library ======
#include <Windows.h>

// ====== Platform Library ======
#include "LibUart.hpp"
#include "My_Basics.hpp"


HANDLE g_hComm;                     // Handle to the Serial port
char   g_ComPortName[30] = {0};     // Name of the Serial port(May Change) to be opened,
BOOL   g_Status;

int LibUart_InitComPort(const char *comPortName, uint32_t baudRate, uint8_t byteSize /* = 8 */, STOP_BITS stopBits /* = STOP_BITS_1 */, PARITY parity /* = PARITY__NONE */)
{
	strcpy(g_ComPortName, "\\\\.\\");
	strcat(g_ComPortName, comPortName);

	/*----------------------------------- Opening the Serial Port --------------------------------------------*/
	g_hComm = CreateFile( g_ComPortName,                   // Name of the Port to be Opened
						GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
						0,                                 // No Sharing, ports cant be shared
						NULL,                              // No Security
						OPEN_EXISTING,                     // Open existing port only
						0,                                 // Non Overlapped I/O
						NULL);                             // Null for Comm Devices

	if (g_hComm == INVALID_HANDLE_VALUE)
	{
		UART_ERR_MSG("\n   Error! - Port %s can't be opened\n", g_ComPortName);
		return -1;
	}
	else 
	{
		UART_LOG_MSG("\n   Port %s Opened\n ", g_ComPortName);
	}

	/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/
	DCB dcbSerialParams = { 0 };                        // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	
	g_Status = GetCommState(g_hComm, &dcbSerialParams);     //retreives  the current settings

	if (g_Status == FALSE)
	{
		UART_ERR_MSG("\n   Error! in GetCommState()\n");
		return -1;
	}

	switch (baudRate) {
		case 256000: dcbSerialParams.BaudRate = CBR_256000; break;
		case 128000: dcbSerialParams.BaudRate = CBR_128000; break;
		case 115200: dcbSerialParams.BaudRate = CBR_115200; break;
		case 57600:  dcbSerialParams.BaudRate = CBR_57600;  break;
		case 38400:  dcbSerialParams.BaudRate = CBR_38400;  break;
		case 19200:  dcbSerialParams.BaudRate = CBR_19200;  break;
		case 14400:  dcbSerialParams.BaudRate = CBR_14400;  break;
		case 9600:   dcbSerialParams.BaudRate = CBR_9600;   break;
		case 4800:   dcbSerialParams.BaudRate = CBR_4800;   break;
		case 2400:   dcbSerialParams.BaudRate = CBR_2400;   break;
		case 1200:   dcbSerialParams.BaudRate = CBR_1200;   break;
		case 600:    dcbSerialParams.BaudRate = CBR_600;    break;
		case 300:    dcbSerialParams.BaudRate = CBR_300;    break;
		case 110:    dcbSerialParams.BaudRate = CBR_110;    break;

		default:
			BASIC_ASSERT(0);
	}

	switch (stopBits) {
		case STOP_BITS_1:   dcbSerialParams.StopBits = ONESTOPBIT; break;
		case STOP_BITS_1_5: dcbSerialParams.StopBits = ONE5STOPBITS; break;
		case STOP_BITS_2:   dcbSerialParams.StopBits = TWOSTOPBITS; break;

		default:
			BASIC_ASSERT(0);
	}

	switch (parity) {
		case PARITY__NONE:  dcbSerialParams.Parity = NOPARITY; break;
		case PARITY__ODD:   dcbSerialParams.Parity = ODDPARITY; break;
		case PARITY__EVEN:  dcbSerialParams.Parity = EVENPARITY; break;
		case PARITY__MARK:  dcbSerialParams.Parity = MARKPARITY; break;
		case PARITY__SPACE: dcbSerialParams.Parity = SPACEPARITY; break;

		default:
			BASIC_ASSERT(0);
	}

	dcbSerialParams.ByteSize = byteSize;      // Setting ByteSize = 8

	g_Status = SetCommState(g_hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB 

	if (g_Status == FALSE)
	{
		UART_ERR_MSG("\n   Error! in Setting DCB Structure\n");
		return -1;
	}
	else
	{
		UART_LOG_MSG("\n   Setting DCB Structure Successfull\n");
		UART_LOG_MSG("\n       Baudrate = %ld", dcbSerialParams.BaudRate);
		UART_LOG_MSG("\n       ByteSize = %d", dcbSerialParams.ByteSize);
		UART_LOG_MSG("\n       StopBits = %d", dcbSerialParams.StopBits);
		UART_LOG_MSG("\n       Parity   = %d", dcbSerialParams.Parity);
	}

	/*------------------------------------ Setting Timeouts --------------------------------------------------*/	
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout         = 50;
	timeouts.ReadTotalTimeoutConstant    = 50;
	timeouts.ReadTotalTimeoutMultiplier  = 10;
	timeouts.WriteTotalTimeoutConstant   = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(g_hComm, &timeouts) == FALSE)
	{
		UART_ERR_MSG("\n   Error! in Setting Time Outs\n");
		return -1;
	}
	else
	{
		UART_LOG_MSG("\n\n   Setting Serial Port Timeouts Successfull");
	}

	UART_LOG_MSG("\n%s() Success.\n", __func__);
	return 0;
}

int LibUart_Send(uint32_t length, uint8_t *buffer)
{
	/*----------------------------- Writing a Character to Serial Port----------------------------------------*/
	DWORD  dNoOfBytesWritten = 0;          // No of bytes written to the port

	g_Status = WriteFile(g_hComm,               // Handle to the Serialport
					   buffer,              // Data to be written to the port 
					   (DWORD)length,   // No of bytes to write into the port
					   &dNoOfBytesWritten,  // No of bytes written to the port
					   NULL);
	
	if (g_Status == TRUE)
	{
		#if UART_LOG
		UART_LOG_MSG("\n\n    [HEX] ");
		for (uint32_t i = 0; i < length; i++) {
			UART_LOG_MSG("%02X ", buffer[i]);
		}
		UART_LOG_MSG("- Written to COM Port");
		#endif
	}
	else
	{
		UART_ERR_MSG("\n\n   Error %ld in Writing to Serial Port\n", GetLastError());
		return -1;
	}
	
	UART_LOG_MSG("\n%s() Success.\n", __func__);
	return 0;
}

int LibUart_Receive(uint32_t *receivedLength, uint8_t *buffer)
{
	DWORD dwEventMask;                     // Event mask to trigger

	*receivedLength = 0; // Clear output variable
	
	/*------------------------------------ Setting Receive Mask ----------------------------------------------*/
	
	g_Status = SetCommMask(g_hComm, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

	if (g_Status == FALSE)
	{
		UART_ERR_MSG("\n\n    Error! in Setting CommMask\n");
		return -1;
	}
	else
	{
		//UART_LOG_MSG("\n\n    Setting CommMask successfull");
	}	

	/*-------------------------- Program will Wait here till a Character is received ------------------------*/				
	//printf("\n\n    Waiting for Data Reception");
	g_Status = WaitCommEvent(g_hComm, &dwEventMask, NULL); //Wait for the character to be received
	
	if (g_Status == FALSE)
	{
		UART_ERR_MSG("\n    Error! in Setting WaitCommEvent()\n");
	}
	else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
	{
		char  TempChar;                        // Temperory Character
		DWORD NoBytesRead;                     // Bytes read by ReadFile()
		int i = 0;
		UART_LOG_MSG("\n\n    Characters Received");
		do
		{
			g_Status = ReadFile(g_hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
			buffer[i] = TempChar;
			i++;
		}
		while (NoBytesRead > 0);
		*receivedLength = i - 1; // Set output variable

		/*------------Printing the RXed String to Console----------------------*/

		#if UART_LOG
		UART_LOG_MSG("\n\n    [HEX] ");
		int j =0;
		for (j = 0; j < i-1; j++)		// j < i-1 to remove the dupliated last character
		{
			UART_LOG_MSG("%02X ", buffer[j]);
		}
		#endif
	}	
	
	UART_LOG_MSG("\n%s() Success.\n", __func__);
	return 0;
}

int LibUart_Receive_WaitData(uint32_t *receivedLength, uint8_t *buffer)
{
	int retVal;
	
	retVal = LibUart_Receive(receivedLength, buffer);

	while (*receivedLength == 0) {
		UART_LOG_MSG("\n%s() Keep waiting the receiving data.\n", __func__);
		retVal = LibUart_Receive(receivedLength, buffer);
	}

	return retVal;
}

int LibUart_UninitComPort(void)
{
	CloseHandle(g_hComm);//Closing the Serial Port
	return 0;
}
