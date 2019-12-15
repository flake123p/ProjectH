
#include "Everything_Lib_Mgr.hpp"

int LibMT_UtilMutex_Init(LibMT_UtilMutex_t *mutex)
{
    if (Lib_IsMT()) {
        int retVal;
        ASSERT_CHK( retVal, LibIPC_Mutex_Create(&(mutex->handle)) );
    }
    return 0;
}

int LibMT_UtilMutex_Uninit(LibMT_UtilMutex_t *mutex)
{
    if (Lib_IsMT()) {
        int retVal;
        ASSERT_CHK( retVal, LibIPC_Mutex_Destroy(mutex->handle) );
    }
    return 0;
}

int LibMT_UtilMutex_Lock(LibMT_UtilMutex_t *mutex)
{
    if (Lib_IsMT()) {
        int retVal;
        ASSERT_CHK( retVal, LibIPC_Mutex_Lock(mutex->handle) );
    }
    return 0;
}

int LibMT_UtilMutex_Unlock(LibMT_UtilMutex_t *mutex)
{
    if (Lib_IsMT()) {
        int retVal;
        ASSERT_CHK( retVal, LibIPC_Mutex_Unlock(mutex->handle) );
    }
    return 0;
}

LibMT_UtilMutex_t gTestUtilMutex1;
#define SHOW_RACE_CONDITION ( 0 )
int race_condition = 100;
void *LibMT_UtilMutex_Demo_Thread1(void *arg)
{
    FOREACH_I(1000000) {
#if SHOW_RACE_CONDITION
#else
        LibMT_UtilMutex_Lock(&gTestUtilMutex1);
#endif
        race_condition++;
#if SHOW_RACE_CONDITION
#else
        LibMT_UtilMutex_Unlock(&gTestUtilMutex1);
#endif
    }
    return 0;
}

void *LibMT_UtilMutex_Demo_Thread2(void *arg)
{
    FOREACH_I(1000000) {
#if SHOW_RACE_CONDITION
#else
        LibMT_UtilMutex_Lock(&gTestUtilMutex1);
#endif
        race_condition--;
#if SHOW_RACE_CONDITION
#else
        LibMT_UtilMutex_Unlock(&gTestUtilMutex1);
#endif
    }
    return 0;
}

void LibMT_UtilMutex_Demo(void)
{
    THREAD_HANDLE_t threadHdlAry[2] = {0};
    int retVal;

#if SHOW_RACE_CONDITION
#else
    LibMT_UtilMutex_Init(&gTestUtilMutex1);
#endif

    DUMPND(race_condition);
    ASSERT_CHK( retVal, LibThread_NewHandle(&threadHdlAry[0]) );
    ASSERT_CHK( retVal, LibThread_NewHandle(&threadHdlAry[1]) );
    ASSERT_CHK( retVal, LibThread_Create(threadHdlAry[0], LibMT_UtilMutex_Demo_Thread1) );
    ASSERT_CHK( retVal, LibThread_Create(threadHdlAry[1], LibMT_UtilMutex_Demo_Thread2) );
    LibThread_WaitThread(threadHdlAry[0]);
    LibThread_WaitThread(threadHdlAry[1]);
    ASSERT_CHK( retVal, LibThread_DestroyHandle(threadHdlAry[0]) );
    ASSERT_CHK( retVal, LibThread_DestroyHandle(threadHdlAry[1]) );
    DUMPND(race_condition);

#if SHOW_RACE_CONDITION
#else
    LibMT_UtilMutex_Uninit(&gTestUtilMutex1);
#endif

    REMOVE_UNUSED_WRANING(retVal);
}

int LibMT_Init(void)
{
    return 0;
}

int LibMT_Uninit(void)
{
    return 0;
}
