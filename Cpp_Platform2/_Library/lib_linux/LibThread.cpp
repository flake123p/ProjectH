

/*
	Reference: 
		http://timmurphy.org/2010/05/04/pthreads-in-c-a-minimal-working-example/
		https://linux.die.net/man/3/pthread_create
*/

int LibThread_NewHandle(OUT THREAD_HANDLE_t *threadHdlPtr, THREAD_PRIORITY_t priority /* = TPRI_DEFAULT */)
{
	return 0;
}

int LibThread_Create(THREAD_HANDLE_t threadHdl, ThreadEntryFunc entry, void *arg = /* NULL */)
{
	return 0;
}

int LibThread_WaitThread(THREAD_HANDLE_t threadHdl)
{
	return 0;
}

int LibThread_WaitThreads(THREAD_HANDLE_t *threadHdlArray, u32 count)
{
	return 0;
}

int LibThread_ReleaseHandle(THREAD_HANDLE_t threadHdl)
{
	return 0;
}