
// ====== Standard C/Cpp Library ======
#include <cstdio>
#include <iostream>
#include <string.h>
#include <stdint.h> // for uint32_t ...
using namespace std;

// ====== Standard Windows Library ======
//#include <windows.h>
//#include <tchar.h>

// ====== Basics ======
#include "My_Basics.hpp"

// ====== Platform Library ======
#include "LibSharedMemory.hpp"

bool LibSharedMemory_Init(u32 buf_size)
{
	return true;
}

void LibSharedMemory_Uninit()
{

}

char *LibSharedMemory_InitEx(u32 buf_size)
{
	return NULL;
}

char *LibSharedMemory_GetBuffer()
{
	return NULL;
}

