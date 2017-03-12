
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

int LibThread_ReleaseHandle(THREAD_HANDLE_t threadHdl)
{
	free(threadHdl);
	return 0;
}
