
// ====== Standard C/Cpp Library ======
#include <cstdio>
#include <iostream>
#include <string.h>
#include <stdint.h> // for uint32_t ...
using namespace std;

// ====== Basics ======
#include "My_Basics.hpp"

// ====== Platform Library ======
#include "LibSharedMemory.hpp"
#include "LibEvent.hpp"
#include "LibOs.hpp"

#define SHARED_MEMORY_SIZE (10)   //Size of Shared Memory

SM_STAT_t *gStat = NULL;

int LibShmMgr_Demo_Server(void)
{
	char *sm_buf = LibShmMgr_Init_WithStat(SHARED_MEMORY_SIZE, &gStat, SHM_SERVER_INIT);
	
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

		LibSharedMemory_Uninit(SHM_SERVER_INIT);
	} else {
		return 1;
	}

	return 0;
}

int LibShmMgr_Demo_Client(void)
{
	char *sm_buf = LibShmMgr_Init_WithStat(SHARED_MEMORY_SIZE, &gStat, SHM_CLIENT_INIT);
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

		LibSharedMemory_Uninit(SHM_CLIENT_INIT);
	} else {
		return 1;
	}

	return 0;
}

char *LibShmMgr_Init_WithStat(IN u32 buf_size, OUT SM_STAT_t **stat_ptr, SHM_INIT_TYPE_t init_type)
{
	u32 aligned_buf_size;
	u32 aligned_stat_size;
	
	BASIC_ASSERT(buf_size>0);

	aligned_buf_size = buf_size;
	if(aligned_buf_size%32 != 0) {
		aligned_buf_size = ((aligned_buf_size/32) + 1) * 32;
	}
	
	aligned_stat_size = sizeof(SM_STAT_t);
	if(aligned_stat_size%32 != 0) {
		aligned_stat_size = ((aligned_stat_size/32) + 1) * 32;
	}

	#if SM_LOG
	DUMPD(buf_size);
	DUMPD(aligned_buf_size);
	DUMPD(sizeof(SM_STAT_t));
	DUMPD(aligned_stat_size);
	#endif
	
	char *sm_buf = LibShmMgr_InitEx(aligned_buf_size + aligned_stat_size, init_type);
	
	if (sm_buf != NULL) {
		//Use last space to store stat
		*stat_ptr = (SM_STAT_t *)(&sm_buf[aligned_buf_size]);
		(*stat_ptr)->server_state = SERVER_IS_FREE;
		(*stat_ptr)->max_len = aligned_buf_size;
		(*stat_ptr)->cur_len = 0;
	}
	
	#if SM_LOG
	LibShmMgr_DumpStat(*stat_ptr);
	#endif
	
	return sm_buf;
}

void LibShmMgr_DumpStat(SM_STAT_t *stat)
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

char *LibShmMgr_InitEx(u32 buf_size, SHM_INIT_TYPE_t init_type)
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