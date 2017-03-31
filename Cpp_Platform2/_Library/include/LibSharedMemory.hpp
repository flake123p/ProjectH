


#ifndef _LIB_SHARED_MEMORY_HPP_INCLUDED_
#include <stdarg.h>
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

// ============================== Define ==============================

// ============================== Library: Cross-Platform (Manager) ==============================
typedef enum {
	SHM_SERVER,
	SHM_CLIENT,
} SHM_SC_TYPE_t;

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
} SHM_STATE_t;
int LibShmMgr_Demo_Server(void);
int LibShmMgr_Demo_Client(void);
char *LibShmMgr_Init_WithStat(IN u32 buf_size, OUT SHM_STATE_t **stat, SHM_SC_TYPE_t init_type);
void LibShmMgr_DumpStat(SHM_STATE_t *stat);
char *LibShmMgr_InitEx(u32 buf_size, SHM_SC_TYPE_t init_type);


typedef struct {
	char *sm_buf;
	SHM_STATE_t *state;
} SHM_GENERIC_CMD_t;
typedef enum {
	SHM_SERVER_INIT,   /* SHM_GENERIC_CMD_t */
	SHM_CLIENT_INIT,   /* SHM_GENERIC_CMD_t */
	SHM_SERVER_UNINIT,
	SHM_CLIENT_UNINIT,
} SHM_COMMAND;
int LibShmMgr_Command(SHM_COMMAND cmd, void *cmdHdl = NULL);
int LibShmMgr_ClientPrint(SHM_GENERIC_CMD_t *genericCmd, const char *format, ...);
// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================
bool LibSharedMemory_Init(u32 buf_size, SHM_SC_TYPE_t init_type);
void LibSharedMemory_Uninit(SHM_SC_TYPE_t init_type);
char *LibSharedMemory_GetBuffer();

#define _LIB_SHARED_MEMORY_HPP_INCLUDED_
#endif//_LIB_SHARED_MEMORY_HPP_INCLUDED_

