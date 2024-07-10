
// ====== Standard C/Cpp Library ======
#include <cstdio>
#include <iostream>
#include <string.h>
#include <stdint.h> // for uint32_t ...
using namespace std;

// ====== Basics ======
#include "My_Basics.hpp"

// ====== Platform Library ======
#include "_LibError.hpp"
#include "LibSharedMemory.hpp"
#include "LibEvent.hpp"
#include "LibOs.hpp"

#define SHARED_MEMORY_SIZE (1000)   //Size of Shared Memory

SHM_STATE_t *gStat = NULL;

int LibShmMgr_Demo_Server(void)
{
	char *sm_buf = LibShmMgr_Init_WithStat(SHARED_MEMORY_SIZE, &gStat, SHM_SERVER);
	
	if (sm_buf != NULL) {
		printf("Dump SharedMemory: (SERVER)\n");
		while (1) {
			LibOs_SleepMiliSeconds(500);
			if (REQUEST_PRINT == gStat->client_request) {
				gStat->server_state = SERVER_IS_PRINTING;
				printf("--> %s\n", sm_buf);
				gStat->client_request = REQUEST_NONE;
				gStat->server_state = SERVER_IS_FREE;
			} else if (REQUEST_CLOSE_SERVER == gStat->client_request) {
				gStat->client_request = REQUEST_NONE;
				break;
			}
		}

		LibSharedMemory_Uninit(SHM_SERVER);
	} else {
		return 1;
	}

	return 0;
}

int LibShmMgr_Demo_Client(void)
{
	char *sm_buf = LibShmMgr_Init_WithStat(SHARED_MEMORY_SIZE, &gStat, SHM_CLIENT);
	int kb;
	int i = 0;
	
	if (sm_buf != NULL) {
		printf("Press Any Key to Write SharedMemory: (CLIENT)\n");
		while (1) {
			kb = LibEvent_GetOneKeyBoard();

			if (kb == KEYBOARD_ESC) {
				gStat->client_request = REQUEST_CLOSE_SERVER;
				break;
			}

			if (i%2 == 0) {
				printf("Write \"ABC\"\n");
				strcpy(sm_buf, "ABC");
			} else {
				printf("Write \"XYZ\"\n");
				strcpy(sm_buf, "XYZ");
			}
			gStat->client_request = REQUEST_PRINT;
			
			i++;
		}

		LibSharedMemory_Uninit(SHM_CLIENT);
	} else {
		return 1;
	}

	return 0;
}

char *LibShmMgr_Init_WithStat(IN u32 buf_size, OUT SHM_STATE_t **stat_ptr, SHM_SC_TYPE_t init_type)
{
	u32 aligned_buf_size;
	u32 aligned_stat_size;
	
	BASIC_ASSERT(buf_size>0);

	aligned_buf_size = buf_size;
	if(aligned_buf_size%32 != 0) {
		aligned_buf_size = ((aligned_buf_size/32) + 1) * 32;
	}
	
	aligned_stat_size = sizeof(SHM_STATE_t);
	if(aligned_stat_size%32 != 0) {
		aligned_stat_size = ((aligned_stat_size/32) + 1) * 32;
	}

	#if SM_LOG
	DUMPD(buf_size);
	DUMPD(aligned_buf_size);
	DUMPD(sizeof(SHM_STATE_t));
	DUMPD(aligned_stat_size);
	#endif
	
	char *sm_buf = LibShmMgr_InitEx(aligned_buf_size + aligned_stat_size, init_type);
	
	if (sm_buf != NULL) {
		//Use last space to store stat
		*stat_ptr = (SHM_STATE_t *)(&sm_buf[aligned_buf_size]);
		(*stat_ptr)->server_state = SERVER_IS_FREE;
		(*stat_ptr)->max_len = aligned_buf_size;
		(*stat_ptr)->cur_len = 0;
	}
	
	#if SM_LOG
	LibShmMgr_DumpStat(*stat_ptr);
	#endif
	
	return sm_buf;
}

void LibShmMgr_DumpStat(SHM_STATE_t *stat)
{
	if (stat == NULL) {
		printf("Stat is NULL\n");
	} else {
		DUMPD(stat->server_state);
		DUMPD(stat->client_request);
		DUMPD(stat->max_len);
		DUMPD(stat->cur_len);
	}
}

char *LibShmMgr_InitEx(u32 buf_size, SHM_SC_TYPE_t init_type)
{
	if (LibSharedMemory_Init(buf_size, init_type))
	{
		#if SM_LOG
		cout << "Initialization of the process was successful\n";
		#endif
		
		return LibSharedMemory_GetBuffer();
	}
	else
	{
		LibSharedMemory_Uninit(init_type);
		#if SM_LOG
		cout << "Initialization of the process was not successful\n";
		#endif
		
		return NULL; //error
	}
}

int LibShmMgr_Command(SHM_COMMAND cmd, void *cmdHdl /* = NULL */)
{
	switch (cmd) {
	case SHM_SERVER_INIT: {
		SHM_GENERIC_CMD_t *genericCmd = (SHM_GENERIC_CMD_t *)cmdHdl;
		BASIC_ASSERT(cmdHdl != NULL);
		
		char *sm_buf = LibShmMgr_Init_WithStat(SHARED_MEMORY_SIZE, &(genericCmd->state), SHM_SERVER);

		genericCmd->sm_buf = sm_buf;
	} break;

	case SHM_CLIENT_INIT: {
		SHM_GENERIC_CMD_t *genericCmd = (SHM_GENERIC_CMD_t *)cmdHdl;
		BASIC_ASSERT(cmdHdl != NULL);
		
		char *sm_buf = LibShmMgr_Init_WithStat(SHARED_MEMORY_SIZE, &(genericCmd->state), SHM_CLIENT);

		genericCmd->sm_buf = sm_buf;
	} break;
	
	case SHM_SERVER_UNINIT: {
		LibSharedMemory_Uninit(SHM_SERVER);
	} break;

	case SHM_CLIENT_UNINIT: {
		LibSharedMemory_Uninit(SHM_CLIENT);
	} break;

	default:
		BASIC_ASSERT(0);
		break;
	}

	return 0;
}

int LibShmMgr_ClientPrint(SHM_GENERIC_CMD_t *genericCmd, const char *format, ...)
{
	int totalPrintNum;
	
	va_list vl;
	va_start(vl, format);
	totalPrintNum = vsprintf(genericCmd->sm_buf, format, vl);
	va_end(vl);

	genericCmd->state->client_request = REQUEST_PRINT;
	LibOs_SleepMiliSeconds(10);
	while (1) {
		if (genericCmd->state->server_state == SERVER_IS_FREE && genericCmd->state->client_request == REQUEST_NONE)
			break;
	}

	return totalPrintNum;
}