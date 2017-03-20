


#ifndef _LIB_THREAD_HPP_INCLUDED_
#include <stdarg.h>
#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================
typedef void * THREAD_HANDLE_t;
typedef enum {
	TPRI_L,
	TPRI_M,
	TPRI_H,

	TPRI_DEFAULT,
} THREAD_PRIORITY_t;
// ============================== Library: Cross-Platform (Manager) ==============================
typedef void *(*ThreadEntryFunc)(void *);


void LibThreadMgr_Demo(void);
void LibThreadMgr_DemoLite(void);
// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================
int LibThread_NewHandle(OUT THREAD_HANDLE_t *threadHdlPtr, THREAD_PRIORITY_t priority = TPRI_DEFAULT);
int LibThread_Create(THREAD_HANDLE_t threadHdl, ThreadEntryFunc entry, void *arg = NULL);
int LibThread_WaitThread(THREAD_HANDLE_t threadHdl);
int LibThread_WaitThreads(THREAD_HANDLE_t *threadHdlArray, u32 count);
int LibThread_ReleaseHandle(THREAD_HANDLE_t threadHdl);

#define _LIB_THREAD_HPP_INCLUDED_
#endif//_LIB_THREAD_HPP_INCLUDED_


