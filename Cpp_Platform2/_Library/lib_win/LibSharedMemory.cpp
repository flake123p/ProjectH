
// ====== Standard C/Cpp Library ======
#include <cstdio>
#include <iostream>
#include <string.h>
#include <stdint.h> // for uint32_t ...
using namespace std;

// ====== Standard Windows Library ======
#include <windows.h>
#include <tchar.h>

// ====== Basics ======
#include "My_Basics.hpp"

// ====== Platform Library ======
#include "_LibError.hpp"
#include "LibSharedMemory.hpp"

//global variables
HANDLE  g_hSharedMemory = NULL; //global handle to shared memory
LPTSTR  g_pBuffer = NULL;       //shared memory pointer
TCHAR   g_szShareMemoryName[] = _T("My Shared Memory");  //Name of the shared memory, required for a Named Kernal Object

bool LibSharedMemory_Init(u32 buf_size, SHM_SC_TYPE_t sc_type)
{
	switch (sc_type) {
		case SHM_SERVER:
		case SHM_CLIENT:
			break;

		default:
			BASIC_ASSERT(0);
			break;
	}
	
	/*
	HANDLE WINAPI CreateFileMapping(
	  _In_     HANDLE                hFile,
	  _In_opt_ LPSECURITY_ATTRIBUTES lpAttributes,
	  _In_     DWORD                 flProtect,
	  _In_     DWORD                 dwMaximumSizeHigh,
	  _In_     DWORD                 dwMaximumSizeLow,
	  _In_opt_ LPCTSTR               lpName
	);

	LPVOID WINAPI MapViewOfFile(
	  _In_ HANDLE hFileMappingObject,
	  _In_ DWORD  dwDesiredAccess,
	  _In_ DWORD  dwFileOffsetHigh,
	  _In_ DWORD  dwFileOffsetLow,
	  _In_ SIZE_T dwNumberOfBytesToMap
	);
	*/
	
	//Shared Memory Stuff
	//Creates or opens shared memory, depending on whether already exists or not
	g_hSharedMemory = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // dwMaximumSizeHigh (High order in DWORD)
		(DWORD)buf_size,         // dwMaximumSizeLow  (Low order in DWORD)
		g_szShareMemoryName);    // name of mapping object

	if (NULL == g_hSharedMemory || INVALID_HANDLE_VALUE == g_hSharedMemory) 
	{
		#if SM_ERR
		cout << "Error occured while creating file mapping object :" << GetLastError() << "\n";
		#endif
		return false;
	}

	g_pBuffer = (LPTSTR) MapViewOfFile(g_hSharedMemory,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,                   
		0,                   
		(SIZE_T)buf_size);           

	if (NULL == g_pBuffer) 
	{ 
		#if SM_ERR
		cout << "Error occured while mapping view of the file :" << GetLastError() << "\n";
		#endif
		return false;
	}

	return true;
}

void LibSharedMemory_Uninit(SHM_SC_TYPE_t sc_type)
{
	switch (sc_type) {
		case SHM_SERVER:
		case SHM_CLIENT:
			break;

		default:
			BASIC_ASSERT(0);
			break;
	}
	
	UnmapViewOfFile(g_pBuffer);
	CloseHandle(g_hSharedMemory);
}

char *LibSharedMemory_GetBuffer()
{
	return static_cast<char*>(g_pBuffer);
}

