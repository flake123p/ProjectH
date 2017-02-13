
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

int LibSharedMemory_Demo_Server(void)
{
	char *sm_buf = LibSharedMemory_Init_WithStat(SHARED_MEMORY_SIZE, &gStat);
	
	if (sm_buf != NULL) {
		printf("Dump SharedMemory: (SERVER)\n");
		while (1) {
			LibOs_SleepMiliSeconds(500);
			if (SERVER_HAD_A_JOB == gStat->server_msg) {
				gStat->server_msg = SERVER_IS_FREE;
				printf("--> %s\n", sm_buf);
			}
		}

		LibSharedMemory_Uninit();
	} else {
		return 1;
	}

	return 0;
}

int LibSharedMemory_Demo_Client(void)
{
	char *sm_buf = LibSharedMemory_Init_WithStat(SHARED_MEMORY_SIZE, &gStat);
	int kb;
	int i = 0;
	
	if (sm_buf != NULL) {
		printf("Press Any Key to Write SharedMemory: (CLIENT)\n");
		while (1) {
			kb = LibEvent_GetOneKeyBoard();

			if (kb == KEYBOARD_ESC) {
				break;
			}

			if (i%2 == 0) {
				printf("Write \"ABC\"\n");
				strcpy(sm_buf, "ABC");
			} else {
				printf("Write \"XYZ\"\n");
				strcpy(sm_buf, "XYZ");
			}
			gStat->server_msg = SERVER_HAD_A_JOB;
			
			i++;
		}

		LibSharedMemory_Uninit();
	} else {
		return 1;
	}

	return 0;
}

char *LibSharedMemory_Init_WithStat(IN u32 buf_size, OUT SM_STAT_t **stat_ptr)
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
	
	char *sm_buf = LibSharedMemory_InitEx(aligned_buf_size + aligned_stat_size);
	
	if (sm_buf != NULL) {
		//Use last space to store stat
		*stat_ptr = (SM_STAT_t *)(&sm_buf[aligned_buf_size]);
		(*stat_ptr)->server_msg = SERVER_IS_FREE;
		(*stat_ptr)->max_len = aligned_buf_size;
		(*stat_ptr)->cur_len = 0;
	}
	
	#if SM_LOG
	LibSharedMemory_DumpStat(*stat_ptr);
	#endif
	
	return sm_buf;
}

void LibSharedMemory_DumpStat(SM_STAT_t *stat)
{
	if (stat == NULL) {
		printf("Stat is NULL\n");
	} else {
		DUMPD(stat->server_msg);
		DUMPD(stat->max_len);
		DUMPD(stat->cur_len);
	}
}