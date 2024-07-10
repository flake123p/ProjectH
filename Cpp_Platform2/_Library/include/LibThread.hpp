


#ifndef _LIB_THREAD_HPP_INCLUDED_
#include <stdarg.h>
#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================
typedef void * THREAD_HANDLE_t;
typedef enum {
	TPRI_HH,
	TPRI_H,
	TPRI_M,
	TPRI_L,
	TPRI_LL,

	TPRI_DEFAULT,
} THREAD_PRIORITY_t;
// ============================== Library: Cross-Platform (Manager) ==============================
typedef void *(*ThreadEntryFunc)(void *);
typedef void (*WatchDogTimeOutFunc)(void);

int LibThreadMgr_Init_WatchDog(u32 sleepMiliSec, WatchDogTimeOutFunc cb = NULL);
int LibThreadMgr_Uninit_WatchDog(void);
int LibThreadMgr_Touch_WatchDog(void);
int LibThreadMgr_Freeze_WatchDog(void);
int LibThreadMgr_Unfreeze_WatchDog(void);

int LibThreadMgr_BatchCreate(THREAD_HANDLE_t *hdlAry, ThreadEntryFunc *funcAry, u32 len);
int LibThreadMgr_BatchWait(THREAD_HANDLE_t *hdlAry, u32 len);
int LibThreadMgr_BatchDestroy(THREAD_HANDLE_t *hdlAry, u32 len);



void LibThreadMgr_Demo(void);
void LibThreadMgr_DemoLite(void);

void LibThreadMgr_DemoEvent(void);
void LibThreadMgr_DemoPriority(void);

void LibThreadMgr_DemoMutex(void);
void LibThreadMgr_Demo_WatchDog(void);
// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================
int LibThread_NewHandle(OUT THREAD_HANDLE_t *threadHdlPtr, THREAD_PRIORITY_t priority = TPRI_DEFAULT);
int LibThread_Create(THREAD_HANDLE_t threadHdl, ThreadEntryFunc entry, void *arg = NULL);
int LibThread_WaitThread(THREAD_HANDLE_t threadHdl);
int LibThread_WaitThreads(THREAD_HANDLE_t *threadHdlArray, u32 count);
int LibThread_DestroyHandle(THREAD_HANDLE_t threadHdl);

typedef void * EVENT_HANDLE_t;
int LibIPC_Event_Create(OUT EVENT_HANDLE_t *eventHdlPtr); // AUTO RESET EVENT !!
int LibIPC_Event_Destroy(EVENT_HANDLE_t eventHdl);
int LibIPC_Event_Set(EVENT_HANDLE_t eventHdl);
int LibIPC_Event_Wait(EVENT_HANDLE_t eventHdl);
int LibIPC_Event_BatchCreate(EVENT_HANDLE_t *eventHdlAry, u32 len);
int LibIPC_Event_BatchDestroy(EVENT_HANDLE_t *eventHdlAry, u32 len);

typedef void * MUTEX_HANDLE_t;
int LibIPC_Mutex_Create(OUT MUTEX_HANDLE_t *mutexHdlPtr);
int LibIPC_Mutex_Destroy(MUTEX_HANDLE_t mutexHdl);
int LibIPC_Mutex_Lock(MUTEX_HANDLE_t mutexHdl);
int LibIPC_Mutex_Unlock(MUTEX_HANDLE_t mutexHdl);

// Old name
#define LibThread_ReleaseHandle LibThread_DestroyHandle
#define LibIPC_Event_Release    LibIPC_Event_Destroy

#define _LIB_THREAD_HPP_INCLUDED_
#endif//_LIB_THREAD_HPP_INCLUDED_


