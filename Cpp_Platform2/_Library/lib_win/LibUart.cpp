
#include "Everything_Lib_Win.hpp"

HANDLE g_hComm;                     // Handle to the Serial port
char   g_ComPortName[30] = {0};     // Name of the Serial port(May Change) to be opened,
BOOL   g_Status;
int g_SniffEnable = 0;

int LibUart_SniffSetting(int initVal)
{
	g_SniffEnable = initVal;
	return g_SniffEnable;
}

int LibUart_InitComPort(const char *comPortName, uint32_t baudRate, int quickReadLevel /* = 0 */, uint8_t byteSize /* = 8 */, STOP_BITS stopBits /* = STOP_BITS_1 */, PARITY parity /* = PARITY__NONE */)
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

#ifndef CBR_460800
#define CBR_460800 460800
#endif
#ifndef CBR_512000
#define CBR_512000 512000
#endif
#ifndef CBR_921600
#define CBR_921600 921600
#endif
//dcbSerialParams.fOutxCtsFlow = 1;
//dcbSerialParams.fRtsControl = 1; //Seems like this line can disable flow controll
//printf("aaaa======0x%x\n",dcbSerialParams.fOutxCtsFlow);
//printf("bbbb======0x%x\n",dcbSerialParams.fRtsControl);
	switch (baudRate) {
		case 921600: dcbSerialParams.BaudRate = CBR_921600; break;
		case 512000: dcbSerialParams.BaudRate = CBR_512000; break;
		case 460800: dcbSerialParams.BaudRate = CBR_460800; break;
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
	if (quickReadLevel == 0) {
		timeouts.ReadIntervalTimeout         = 50;
		timeouts.ReadTotalTimeoutConstant    = 50;
		timeouts.ReadTotalTimeoutMultiplier  = 10;
		timeouts.WriteTotalTimeoutConstant   = 50;
		timeouts.WriteTotalTimeoutMultiplier = 10;
	} else if (quickReadLevel == 1) {
		timeouts.ReadIntervalTimeout         = 10;
		timeouts.ReadTotalTimeoutConstant    = 50;
		timeouts.ReadTotalTimeoutMultiplier  = 10;
		timeouts.WriteTotalTimeoutConstant   = 50;
		timeouts.WriteTotalTimeoutMultiplier = 10;
	} else if (quickReadLevel == 2) {
		timeouts.ReadIntervalTimeout         = 5;
		timeouts.ReadTotalTimeoutConstant    = 50;
		timeouts.ReadTotalTimeoutMultiplier  = 10;
		timeouts.WriteTotalTimeoutConstant   = 50;
		timeouts.WriteTotalTimeoutMultiplier = 10;
	} else {
		timeouts.ReadIntervalTimeout         = 1;
		timeouts.ReadTotalTimeoutConstant    = 1;
		timeouts.ReadTotalTimeoutMultiplier  = 0;
		timeouts.WriteTotalTimeoutConstant   = 5;
		timeouts.WriteTotalTimeoutMultiplier = 1;
	}
	
	if (SetCommTimeouts(g_hComm, &timeouts) == FALSE)
	{
		UART_ERR_MSG("\n   Error! in Setting Time Outs\n");
		return -1;
	}
	else
	{
		UART_LOG_MSG("\n\n   Setting Serial Port Timeouts Successfull");
	}

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

	
	UART_LOG_MSG("\n%s() Success.\n", __func__);
	return 0;
}

int LibUart_Send(uint8_t *buffer, uint32_t length)
{
	if (g_SniffEnable || UART_LOG) {
		printf("\n\n    [ %s() - SNIFF ] ", __func__);
		for (uint32_t i = 0; i < length; i++) {
			printf("%02X ", buffer[i]);
		}
		printf("- Written to COM Port\n");
	}

	/*----------------------------- Writing a Character to Serial Port----------------------------------------*/
	DWORD  dNoOfBytesWritten = 0;          // No of bytes written to the port

	g_Status = WriteFile(g_hComm,               // Handle to the Serialport
					   buffer,              // Data to be written to the port 
					   (DWORD)length,   // No of bytes to write into the port
					   &dNoOfBytesWritten,  // No of bytes written to the port
					   NULL);
	
	if (g_Status == TRUE)
	{
		;
	}
	else
	{
		UART_ERR_MSG("\n\n   Error %ld in Writing to Serial Port\n", GetLastError());
		return -1;
	}
	
	UART_LOG_MSG("\n%s() Success.\n", __func__);
	return 0;
}

int LibUart_Receive(uint8_t *buffer, uint32_t *receivedLength)
{
	DWORD dwEventMask;                     // Event mask to trigger

	*receivedLength = 0; // Clear output variable
	
	/*-------------------------- Program will Wait here till a Character is received ------------------------*/				
	//printf("\n\n    Waiting for Data Reception");
	g_Status = WaitCommEvent(g_hComm, &dwEventMask, NULL); //Wait for the character to be received
	
	if (g_Status == FALSE)
	{
		UART_ERR_MSG("\n    Error! in Setting WaitCommEvent()\n");
	}
	else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
	{
#if 1
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
#else
		g_Status = ReadFile(g_hComm, buffer, 40, (DWORD *)receivedLength, NULL);
#endif
		/*------------Printing the RXed String to Console----------------------*/

		if (g_SniffEnable || UART_LOG) {
			printf("\n\n    [ %s() - SNIFF ] ", __func__);
			int j =0;
			for (j = 0; j < i-1; j++)		// j < i-1 to remove the dupliated last character
			{
				printf("%02X ", buffer[j]);
			}
			printf("- Read from COM Port\n");
		}
	}	

	UART_LOG_MSG("\n%s() Success.\n", __func__);
	return 0;
}

int LibUart_UninitComPort(void)
{
	CloseHandle(g_hComm);//Closing the Serial Port
	return 0;
}

int LibUart_ReceiveEx(uint8_t *buffer, uint32_t *receivedLength, uint32_t singleReadlength)
{
	DWORD dwEventMask;                     // Event mask to trigger

	*receivedLength = 0; // Clear output variable
	
	/*------------------------------------ Setting Receive Mask ----------------------------------------------*/
	/*-------------------------- Program will Wait here till a Character is received ------------------------*/				
	UART_LOG_MSG("\n\n    Waiting for Data Reception");
	g_Status = WaitCommEvent(g_hComm, &dwEventMask, NULL); //Wait for the character to be received
	
	if (g_Status == FALSE)
	{
		UART_ERR_MSG("\n    Error! in Setting WaitCommEvent()\n");
	}
	else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
	{
#if 0
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
#else
		#if UART_LOG
		UART_LOG_MSG("ReadFile START. singleReadlength=%d\n", singleReadlength);
		#endif

		g_Status = ReadFile(g_hComm, buffer, singleReadlength, (DWORD *)receivedLength, NULL);
#endif
		/*------------Printing the RXed String to Console----------------------*/

		if (g_SniffEnable || UART_LOG) {
			printf("\n\n    [ %s() - SNIFF ] ", __func__);
			uint32_t j =0;
			for (j = 0; j < *receivedLength; j++)		// j < i-1 to remove the dupliated last character
			{
				printf("%02X ", buffer[j]);
			}
			printf("- Read from COM Port. receivedLength=%d\n", *receivedLength);
		}
	}	
	
	UART_LOG_MSG("\n%s() Success.\n", __func__);
	return 0;
}