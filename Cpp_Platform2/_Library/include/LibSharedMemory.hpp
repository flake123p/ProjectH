


#ifndef _LIB_SHARED_MEMORY_HPP_INCLUDED_
#include "My_Types.h"

// ============================== Debug ==============================
#define SM_LOG  (1)
#define SM_WARN (1)
#define SM_ERR  (1)

#if SM_LOG
#define SM_LOG_MSG printf
#else
#define SM_LOG_MSG(...)
#endif

// ============================== Library: Cross-Platform (Manager) ==============================
typedef enum {
	SHM_SERVER_INIT,
	SHM_CLIENT_INIT,
} SHM_INIT_TYPE_t;

typedef enum {
	SERVER_IS_FREE,
	SERVER_IS_PRINTING,
} SERVER_STATE_t;
typedef enum {
	REQUEST_NONE,
	REQUEST_PRINT,

	REQUEST_CLOSE_SERVER,
} CLIENT_REQUEST_t;
typedef struct {
	SERVER_STATE_t   server_state;
	CLIENT_REQUEST_t client_request;
	u32              max_len;
	u32              cur_len;
} SM_STAT_t;
int LibShmMgr_Demo_Server(void);
int LibShmMgr_Demo_Client(void);
char *LibShmMgr_Init_WithStat(IN u32 buf_size, OUT SM_STAT_t **stat, SHM_INIT_TYPE_t init_type);
void LibShmMgr_DumpStat(SM_STAT_t *stat);
char *LibShmMgr_InitEx(u32 buf_size, SHM_INIT_TYPE_t init_type);
// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================
bool LibSharedMemory_Init(u32 buf_size, SHM_INIT_TYPE_t init_type);
void LibSharedMemory_Uninit(SHM_INIT_TYPE_t init_type);
char *LibSharedMemory_GetBuffer();

#define _LIB_SHARED_MEMORY_HPP_INCLUDED_
#endif//_LIB_SHARED_MEMORY_HPP_INCLUDED_

