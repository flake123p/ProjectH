
#include "Everything_Lib_Win.hpp"

/*
	Reference: https://www.codeproject.com/Articles/13557/Creating-Threads-using-the-CreateThread-API
*/

typedef struct {
	HANDLE winHdl;
	THREAD_PRIORITY_t priority;
}THREAD_HANDLE_WIN_t;
int LibThread_NewHandle(OUT THREAD_HANDLE_t *threadHdlPtr, THREAD_PRIORITY_t priority /* = TPRI_DEFAULT */)
{
	THREAD_HANDLE_WIN_t *tHdl = (THREAD_HANDLE_WIN_t *)malloc(sizeof(THREAD_HANDLE_WIN_t));
	if (tHdl == NULL)
		return -1;

	tHdl->priority = priority;
	*threadHdlPtr = tHdl;
	return 0;
}

int LibThread_Create(THREAD_HANDLE_t threadHdl, ThreadEntryFunc entry, void *arg /* = NULL */)
{
	THREAD_HANDLE_WIN_t *tHdl = (THREAD_HANDLE_WIN_t *)threadHdl;
	
	tHdl->winHdl = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)entry, arg, 0, NULL);

	bool isSetThreadSuccess = false;
	switch (tHdl->priority) {
		case TPRI_HH: {
			isSetThreadSuccess = SetThreadPriority(tHdl->winHdl, THREAD_PRIORITY_HIGHEST);
		} break;

		case TPRI_H: {
			isSetThreadSuccess = SetThreadPriority(tHdl->winHdl, THREAD_PRIORITY_ABOVE_NORMAL);
		} break;

		case TPRI_DEFAULT:
		case TPRI_M: {
			isSetThreadSuccess = SetThreadPriority(tHdl->winHdl, THREAD_PRIORITY_NORMAL);
		} break;

		case TPRI_L: {
			isSetThreadSuccess = SetThreadPriority(tHdl->winHdl, THREAD_PRIORITY_BELOW_NORMAL);
		} break;

		case TPRI_LL: {
			isSetThreadSuccess = SetThreadPriority(tHdl->winHdl, THREAD_PRIORITY_LOWEST);
		} break;

		default:
			return 3;
	}

	if (isSetThreadSuccess == false)
		return 2;
	
	if ( tHdl->winHdl == NULL)
		return 1;
	
	return 0;
}

int LibThread_WaitThread(THREAD_HANDLE_t threadHdl)
{
	THREAD_HANDLE_WIN_t *tHdl = (THREAD_HANDLE_WIN_t *)threadHdl;

	WaitForSingleObject(tHdl->winHdl, INFINITE);
	
	return 0;
}

int LibThread_WaitThreads(THREAD_HANDLE_t *threadHdlArray, u32 count)
{
	THREAD_HANDLE_WIN_t *tHdl;
#if 1
	HANDLE *winHdlAry = (HANDLE *)malloc(sizeof(HANDLE)*count);

	for (u32 i = 0; i < count; i++) {
		tHdl = (THREAD_HANDLE_WIN_t *)threadHdlArray[i];
		winHdlAry[i] = tHdl->winHdl;
	}
	WaitForMultipleObjects(count, winHdlAry, TRUE, INFINITE);
	
	free(winHdlAry);
#else
	for (u32 i = 0; i < count; i++) {
		tHdl = (THREAD_HANDLE_WIN_t *)threadHdlArray[i];
		WaitForSingleObject(tHdl->winHdl, INFINITE);
		tHdl++;
	}
#endif
	return 0;
}

int LibThread_DestroyHandle(THREAD_HANDLE_t threadHdl)
{
	free(threadHdl);
	return 0;
}



typedef struct {
	HANDLE winHdl;
}EVENT_HANDLE_WIN_t;
int LibIPC_Event_Create(OUT EVENT_HANDLE_t *eventHdlPtr) // AUTO RESET EVENT !!
{
	EVENT_HANDLE_WIN_t *eHdl = (EVENT_HANDLE_WIN_t *)malloc(sizeof(EVENT_HANDLE_WIN_t));
	if (eHdl == NULL)
		return -1;

	eHdl->winHdl = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	*eventHdlPtr = eHdl;
	return 0;
}

int LibIPC_Event_Destroy(EVENT_HANDLE_t eventHdl)
{
	EVENT_HANDLE_WIN_t *eHdl = (EVENT_HANDLE_WIN_t *)eventHdl;

	CloseHandle(eHdl->winHdl);

	free(eventHdl);
	return 0;
}

int LibIPC_Event_Set(EVENT_HANDLE_t eventHdl)
{
	EVENT_HANDLE_WIN_t *eHdl = (EVENT_HANDLE_WIN_t *)eventHdl;

	SetEvent(eHdl->winHdl);
	
	return 0;
}

int LibIPC_Event_Wait(EVENT_HANDLE_t eventHdl)
{
	EVENT_HANDLE_WIN_t *eHdl = (EVENT_HANDLE_WIN_t *)eventHdl;

	WaitForSingleObject(eHdl->winHdl, INFINITE);
	
	return 0;
}



typedef struct {
	HANDLE winHdl;
}MUTEX_HANDLE_WIN_t;
int LibIPC_Mutex_Create(OUT MUTEX_HANDLE_t *mutexHdlPtr)
{
	MUTEX_HANDLE_WIN_t *winMutexHdl = (MUTEX_HANDLE_WIN_t *)malloc(sizeof(MUTEX_HANDLE_WIN_t));
	if (winMutexHdl == NULL)
		return -1;

	winMutexHdl->winHdl = CreateMutex( 
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	*mutexHdlPtr = winMutexHdl;

	return 0;
}

int LibIPC_Mutex_Destroy(MUTEX_HANDLE_t mutexHdl)
{
	MUTEX_HANDLE_WIN_t *winMutexHdl = (MUTEX_HANDLE_WIN_t *)mutexHdl;

	CloseHandle(winMutexHdl->winHdl);

	free(winMutexHdl);
	return 0;
}

int LibIPC_Mutex_Lock(MUTEX_HANDLE_t mutexHdl)
{
	MUTEX_HANDLE_WIN_t *winMutexHdl = (MUTEX_HANDLE_WIN_t *)mutexHdl;

	WaitForSingleObject(winMutexHdl->winHdl, INFINITE);

	return 0;
}

int LibIPC_Mutex_Unlock(MUTEX_HANDLE_t mutexHdl)
{
	MUTEX_HANDLE_WIN_t *winMutexHdl = (MUTEX_HANDLE_WIN_t *)mutexHdl;

	if ( NOT( ReleaseMutex(winMutexHdl->winHdl) ) ) {
		return 1;
	}

	return 0;
}
