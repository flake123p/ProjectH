
#include "Everything_Lib_Linux.hpp"

/*
	Reference: 
		http://timmurphy.org/2010/05/04/pthreads-in-c-a-minimal-working-example/
		https://linux.die.net/man/3/pthread_create
*/

typedef struct {
	pthread_t thread;
	THREAD_PRIORITY_t priority;
}THREAD_HANDLE_LINUX_t;
int LibThread_NewHandle(OUT THREAD_HANDLE_t *threadHdlPtr, THREAD_PRIORITY_t priority /* = TPRI_DEFAULT */)
{
	THREAD_HANDLE_LINUX_t *linuxThreadHdl = (THREAD_HANDLE_LINUX_t *)malloc(sizeof(THREAD_HANDLE_LINUX_t));
	if (linuxThreadHdl == NULL)
		return -1;

	linuxThreadHdl->priority = priority;
	*threadHdlPtr = linuxThreadHdl;
	return 0;
}

int LibThread_Create(THREAD_HANDLE_t threadHdl, ThreadEntryFunc entry, void *arg /* = NULL */)
{
	THREAD_HANDLE_LINUX_t *linuxThreadHdl = (THREAD_HANDLE_LINUX_t *)threadHdl;

	int retVal = pthread_create(&linuxThreadHdl->thread, NULL, entry, arg);

	return retVal;
}

int LibThread_WaitThread(THREAD_HANDLE_t threadHdl)
{
	THREAD_HANDLE_LINUX_t *linuxThreadHdl = (THREAD_HANDLE_LINUX_t *)threadHdl;

	/* wait for the thread to finish */
	int retVal = pthread_join(linuxThreadHdl->thread, NULL);
	
	return retVal;
}

int LibThread_WaitThreads(THREAD_HANDLE_t *threadHdlArray, u32 count)
{
	return 0;
}

int LibThread_ReleaseHandle(THREAD_HANDLE_t threadHdl)
{
	free(threadHdl);
	return 0;
}