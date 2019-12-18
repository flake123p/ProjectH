
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

#define SHOW_RACE_CONDITION ( 1 )

int race_condition = 100;
LibMT_UtilMutex_t gTestUtilMutex1;
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
    if (NOT(Lib_IsMT())) {
        printf("Please set LIB_MT_ENABLE flag at Lib_Init()... Protect condition demo failed...\n");
        return;
    }
#endif
    LibMT_UtilMutex_Init(&gTestUtilMutex1);

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

    LibMT_UtilMutex_Uninit(&gTestUtilMutex1);

    REMOVE_UNUSED_WRANING(retVal);
}

LibMT_UtilMutex_t gLibMT_MsgLock;
DLList_Head_t gLibMT_MsgHead;
#define LIB_MT_PREALLOCATE_MSG_NUM ( 16 )
#define LIB_MT_MSG_LOCK   LibMT_UtilMutex_Lock(&gLibMT_MsgLock);
#define LIB_MT_MSG_UNLOCK LibMT_UtilMutex_Unlock(&gLibMT_MsgLock);

LibMT_UtilMutex_t gLibMT_ThreadLock;
DLList_Head_t gLibMT_ThreadHead;
#define LIB_MT_THREAD_LOCK   LibMT_UtilMutex_Lock(&gLibMT_ThreadLock);
#define LIB_MT_THREAD_UNLOCK LibMT_UtilMutex_Unlock(&gLibMT_ThreadLock);

int _LibMT_DestroyMsgList(void)
{
    LibMT_Msg_t *currMsg;
    LibMT_Msg_t *prevMsg;

    DLLIST_WHILE_START(&gLibMT_MsgHead, currMsg, LibMT_Msg_t)
    {
        prevMsg = currMsg;
        DLLIST_WHILE_NEXT(currMsg, LibMT_Msg_t);
        free(prevMsg);
    }

    return 0;
}

int _LibMT_CreateMsgList(void)
{
    LibMT_Msg_t *msg;

    DLLIST_HEAD_RESET(&gLibMT_MsgHead);
    FOREACH_I(LIB_MT_PREALLOCATE_MSG_NUM)
    {
        msg = (LibMT_Msg_t *)malloc(sizeof(LibMT_Msg_t));
        if (msg == NULL) {
            _LibMT_DestroyMsgList();
            return 1;
        }
        msg->is_pre_allocate = 1;
        //Add to list
        DLLIST_INSERT_LAST(&gLibMT_MsgHead, msg);
    }

    return 0;
}

void *_LibMT_CommonShellFunc(void *hdl)
{
    return NULL;
}

int LibMT_Init(void)
{
    LibMT_UtilMutex_Init(&gLibMT_MsgLock);
    LibMT_UtilMutex_Init(&gLibMT_ThreadLock);

    _LibMT_CreateMsgList();
    DLLIST_HEAD_RESET(&gLibMT_ThreadHead);

    return 0;
}

int LibMT_Uninit(void)
{
    LibMT_UtilMutex_Uninit(&gLibMT_MsgLock);
    LibMT_UtilMutex_Uninit(&gLibMT_ThreadLock);

    _LibMT_DestroyMsgList();
    return 0;
}

LibMT_Msg_t *LibMT_MsgGet(void)
{
    LibMT_Msg_t *msg;

    LIB_MT_MSG_LOCK;
    if (DLLIST_IS_NOT_EMPTY(&gLibMT_MsgHead)) {
        msg = (LibMT_Msg_t *)DLLIST_FIRST(&gLibMT_MsgHead);
        DLLIST_REMOVE_FIRST(&gLibMT_MsgHead);
    } else {
        msg = (LibMT_Msg_t *)malloc(sizeof(LibMT_Msg_t));
        msg->is_pre_allocate = 0;
    }
    LIB_MT_MSG_UNLOCK;
    return msg;
}

int LibMT_MsgRelease(LibMT_Msg_t *msg)
{
    LIB_MT_MSG_LOCK;
    if (msg->is_pre_allocate) {
        DLLIST_INSERT_LAST(&gLibMT_MsgHead, msg);
    } else {
        free(msg);
    }
    LIB_MT_MSG_UNLOCK;
    return 0;
}

LibMT_ThreadInfo_t *LibMT_CreateThread(ThreadEntryFunc func)
{
    int retVal = 0;
    LibMT_ThreadInfo_t *info;

    LIB_MT_THREAD_LOCK;
    info = (LibMT_ThreadInfo_t *)malloc(sizeof(LibMT_ThreadInfo_t));
    DLLIST_INSERT_LAST(&gLibMT_ThreadHead, info);

    ASSERT_CHK( retVal, LibThread_NewHandle(&(info->threadHdl)) );
    ASSERT_CHK( retVal, LibIPC_Event_Create(&(info->evtHdl)) );
    ASSERT_CHK( retVal, LibIPC_Mutex_Create(&(info->msgLock)) );
    DLLIST_HEAD_RESET(&(info->msgHead));

    ASSERT_CHK( retVal, LibThread_Create(info->threadHdl, _LibMT_CommonShellFunc, (void *)info) );
    //LibOs_SleepMiliSeconds(10); // For linux, prevent SetEvent() is running before WaitEvent() !!

    LIB_MT_THREAD_UNLOCK;

    return info;
}

int LibMT_WaitThreadAndDestroy(LibMT_ThreadInfo_t *info)
{
    return 0;
}

int LibMT_WaitMainThreadAndDestroyAll(LibMT_ThreadInfo_t *info /* = NULL */)
{
    if (info != NULL) {
    }
    return 0;
}

void LibMT_Demo(void)
{
    if (Lib_IsMT()) {
        printf("Demo multi-threading mode now ...\n");
    } else {
        printf("Demo single-threading mode now ...\n");
    }

    
}