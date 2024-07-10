


#ifndef _LIB_SHARED_MEMORY_HPP_INCLUDED_
#include "My_Types.h"

// ============================== Debug ==============================
#define SM_LOG  (0)
#define SM_WARN (1)
#define SM_ERR  (1)

#if SM_LOG
#define SM_LOG_MSG printf
#else
#define SM_LOG_MSG(...)
#endif

// ============================== Library: Cross-Platform (Manager) ==============================
typedef enum {
	SERVER_IS_FREE,
	SERVER_HAD_A_JOB,
} SERVER_MSG_t;
typedef struct {
	SERVER_MSG_t server_msg;
	u32          max_len;
	u32          cur_len;
} SM_STAT_t;
int LibSharedMemory_Demo_Server(void);
int LibSharedMemory_Demo_Client(void);
char *LibSharedMemory_Init_WithStat(IN u32 buf_size, OUT SM_STAT_t **stat);
void LibSharedMemory_DumpStat(SM_STAT_t *stat);
// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================
bool LibSharedMemory_Init(u32 buf_size);
void LibSharedMemory_Uninit();
char *LibSharedMemory_InitEx(u32 buf_size);
char *LibSharedMemory_GetBuffer();

#define _LIB_SHARED_MEMORY_HPP_INCLUDED_
#endif//_LIB_SHARED_MEMORY_HPP_INCLUDED_

