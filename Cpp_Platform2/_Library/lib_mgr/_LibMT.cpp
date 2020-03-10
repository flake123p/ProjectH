
#include "Everything_Lib_Mgr.hpp"

int LibMT_UtilMutex_Init(LibMT_UtilMutex_t *mutex)
{
    if (Lib_IsMT()) {
        int retVal;
        BASIC_ASSERT(mutex->handle == NULL);
        ASSERT_CHK( retVal, LibIPC_Mutex_Create(&(mutex->handle)) );
    }
    return 0;
}

int LibMT_UtilMutex_Uninit(LibMT_UtilMutex_t *mutex)
{
    if (Lib_IsMT()) {
        int retVal;
        BASIC_ASSERT(mutex->handle != NULL);
        ASSERT_CHK( retVal, LibIPC_Mutex_Destroy(mutex->handle) );
    }
    return 0;
}

int LibMT_UtilMutex_Lock(LibMT_UtilMutex_t *mutex)
{
    if (Lib_IsMT()) {
        int retVal;
        BASIC_ASSERT(mutex->handle != NULL);
        ASSERT_CHK( retVal, LibIPC_Mutex_Lock(mutex->handle) );
    }
    return 0;
}

int LibMT_UtilMutex_Unlock(LibMT_UtilMutex_t *mutex)
{
    if (Lib_IsMT()) {
        int retVal;
        BASIC_ASSERT(mutex->handle != NULL);
        ASSERT_CHK( retVal, LibIPC_Mutex_Unlock(mutex->handle) );
    }
    return 0;
}

#define SHOW_RACE_CONDITION ( 0 )

volatile int race_condition = 100;
LibMT_UtilMutex_t gTestUtilMutex1;
void *LibMT_UtilMutex_Demo_Thread1(void *arg)
{
    LibOs_SleepMiliSeconds(1);
    FOREACH_I(100000) {
#if SHOW_RACE_CONDITION
#else
        LibMT_UtilMutex_Lock(&gTestUtilMutex1);
#endif
        race_condition++;
        race_condition++;
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
    FOREACH_I(100000) {
#if SHOW_RACE_CONDITION
#else
        LibMT_UtilMutex_Lock(&gTestUtilMutex1);
#endif
        race_condition--;
        race_condition--;
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
        MM_FREE(prevMsg);
    }

    return 0;
}

int _LibMT_CreateMsgList(void)
{
    LibMT_Msg_t *msg;

    DLLIST_HEAD_RESET(&gLibMT_MsgHead);
    FOREACH_I(LIB_MT_PREALLOCATE_MSG_NUM)
    {
        msg = (LibMT_Msg_t *)MM_ALLOC(sizeof(LibMT_Msg_t));
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

void *_LibMT_CommonShellEntry(void *hdl)
{
    LibMT_ThreadInfo_t *info = (LibMT_ThreadInfo_t *)hdl;
    LibMT_Msg_t *msg;

    while (1)
    {
        LibIPC_Event_Wait(info->evtHdl);

        while (1)
        {
            msg = LibMT_MsgReceive(info);
            if (msg == NULL)
                break;

            if ( (*(info->func))(msg) ) {
                LibMT_MsgRelease(msg);
                return NULL;
            }
            LibMT_MsgRelease(msg);
        }
    }
    return NULL;
}

int _LibMT_DestroyThreadMsgList(DLList_Head_t *head)
{
    LibMT_Msg_t *currMsg;
    LibMT_Msg_t *prevMsg;

    DLLIST_WHILE_START(head, currMsg, LibMT_Msg_t)
    {
        prevMsg = currMsg;
        DLLIST_WHILE_NEXT(currMsg, LibMT_Msg_t);
        LibMT_MsgRelease(prevMsg);
    }

    return 0;
}

LibMT_UtilMutex_t gLibMT_Mutex_Array[MUTEX_TOTAL_NUM];
int LibMT_Mutex_Lock(LIBMT_MUTEX_t index)
{
    return LibMT_UtilMutex_Lock(&(gLibMT_Mutex_Array[index]));
}

int LibMT_Mutex_Unlock(LIBMT_MUTEX_t index)
{
    return LibMT_UtilMutex_Unlock(&(gLibMT_Mutex_Array[index]));
}

int LibMT_Init(void)
{
    LibMT_UtilMutex_Init(&gLibMT_MsgLock);
    LibMT_UtilMutex_Init(&gLibMT_ThreadLock);

    FOREACH_I(MUTEX_TOTAL_NUM) {
        LibMT_UtilMutex_Init(&(gLibMT_Mutex_Array[i]));
    }

    _LibMT_CreateMsgList();
    DLLIST_HEAD_RESET(&gLibMT_ThreadHead);

    return 0;
}

int LibMT_Uninit(void)
{
    LibMT_UtilMutex_Uninit(&gLibMT_MsgLock);
    LibMT_UtilMutex_Uninit(&gLibMT_ThreadLock);

    FOREACH_I(MUTEX_TOTAL_NUM) {
        LibMT_UtilMutex_Uninit(&(gLibMT_Mutex_Array[i]));
    }

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
        msg = (LibMT_Msg_t *)MM_ALLOC(sizeof(LibMT_Msg_t));
        msg->is_pre_allocate = 0;
    }
    LIB_MT_MSG_UNLOCK;
    msg->hdl = NULL;
    return msg;
}

int LibMT_MsgRelease(LibMT_Msg_t *msg)
{
    LIB_MT_MSG_LOCK;
    if (msg->is_pre_allocate) {
        DLLIST_INSERT_LAST(&gLibMT_MsgHead, msg);
    } else {
        MM_FREE(msg);
    }
    LIB_MT_MSG_UNLOCK;
    return 0;
}

int LibMT_MsgToThread(LibMT_Msg_t *msg, LibMT_ThreadInfo_t *info)
{
    LibIPC_Mutex_Lock(info->msgLock);
    DLLIST_INSERT_LAST(&(info->msgHead), msg);
    LibIPC_Mutex_Unlock(info->msgLock);

    LibIPC_Event_Set(info->evtHdl);
    return 0;
}

int LibMT_MsgToThreadLite(u32 val, LibMT_ThreadInfo_t *info)
{
    LibMT_Msg_t *msg = LibMT_MsgGet();
    msg->val = val;
    LibMT_MsgToThread(msg, info);
    return 0;
}

LibMT_Msg_t *LibMT_MsgReceive(LibMT_ThreadInfo_t *info)
{
    LibMT_Msg_t *msg = NULL;

    LIB_MT_MSG_LOCK;
    if (DLLIST_IS_NOT_EMPTY(&(info->msgHead))) {
        msg = (LibMT_Msg_t *)DLLIST_FIRST(&(info->msgHead));
        DLLIST_REMOVE_FIRST(&(info->msgHead));
    }
    LIB_MT_MSG_UNLOCK;
    return msg;
}

LibMT_ThreadInfo_t *LibMT_CreateThread(LibMT_EntryFunc func)
{
    int retVal = 0;
    LibMT_ThreadInfo_t *info;

    LIB_MT_THREAD_LOCK;
    info = (LibMT_ThreadInfo_t *)MM_ALLOC(sizeof(LibMT_ThreadInfo_t));
    DLLIST_INSERT_LAST(&gLibMT_ThreadHead, info);

    ASSERT_CHK( retVal, LibThread_NewHandle(&(info->threadHdl)) );
    ASSERT_CHK( retVal, LibIPC_Event_Create(&(info->evtHdl)) );
    ASSERT_CHK( retVal, LibIPC_Mutex_Create(&(info->msgLock)) );
    DLLIST_HEAD_RESET(&(info->msgHead));
    info->func = func;

    ASSERT_CHK( retVal, LibThread_Create(info->threadHdl, _LibMT_CommonShellEntry, (void *)info) );

    LibOs_SleepMiliSeconds(10); // For linux, prevent SetEvent() is running before WaitEvent() !!

    LIB_MT_THREAD_UNLOCK;

    return info;
}

LibMT_ThreadInfo_t *LibMT_CreateThreadEx(ThreadEntryFunc func)
{
    int retVal = 0;
    LibMT_ThreadInfo_t *info;

    LIB_MT_THREAD_LOCK;
    info = (LibMT_ThreadInfo_t *)MM_ALLOC(sizeof(LibMT_ThreadInfo_t));
    DLLIST_INSERT_LAST(&gLibMT_ThreadHead, info);

    ASSERT_CHK( retVal, LibThread_NewHandle(&(info->threadHdl)) );
    ASSERT_CHK( retVal, LibIPC_Event_Create(&(info->evtHdl)) );
    ASSERT_CHK( retVal, LibIPC_Mutex_Create(&(info->msgLock)) );
    DLLIST_HEAD_RESET(&(info->msgHead));
    info->func = (LibMT_EntryFunc)func; //meanless

    ASSERT_CHK( retVal, LibThread_Create(info->threadHdl, func, (void *)info) );

    LibOs_SleepMiliSeconds(10); // For linux, prevent SetEvent() is running before WaitEvent() !!

    LIB_MT_THREAD_UNLOCK;

    return info;
}

int LibMT_WaitThreadAndDestroy(LibMT_ThreadInfo_t *info)
{
    int retVal;
    LibThread_WaitThread(info->threadHdl);
    LIB_MT_THREAD_LOCK;
    ASSERT_CHK( retVal, LibThread_DestroyHandle(info->threadHdl) );
    ASSERT_CHK( retVal, LibIPC_Event_Destroy(info->evtHdl) );
    ASSERT_CHK( retVal, LibIPC_Mutex_Destroy(info->msgLock) );
    _LibMT_DestroyThreadMsgList(&(info->msgHead));
    DLLIST_REMOVE_NODE_SAFELY(&gLibMT_ThreadHead, info);
    MM_FREE(info);
    LIB_MT_THREAD_UNLOCK;
    return 0;
}

int LibMT_WaitMainThreadAndDestroyAll(LibMT_ThreadInfo_t *info /* = NULL */)
{
    if (info != NULL) {
        LibThread_WaitThread(info->threadHdl);
    }

    {
        LibMT_ThreadInfo_t *currInfo;
        LibMT_ThreadInfo_t *prevInfo;

        DLLIST_WHILE_START(&gLibMT_ThreadHead, currInfo, LibMT_ThreadInfo_t)
        {
            prevInfo = currInfo;
            DLLIST_WHILE_NEXT(currInfo, LibMT_ThreadInfo_t);
            LibMT_WaitThreadAndDestroy(prevInfo);
        }
    }
    return 0;
}

LibMT_ThreadInfo_t *taskH;
LibMT_ThreadInfo_t *taskL;
int LibMT_Demo_ThreadH(LibMT_Msg_t *msg)
{
    //additional demo
    LibVCD_ClockAdd(LibTime_StopMicroSecondClock());
    LibVCD_ValueChange(0, 1);

    printf("%s() %d\n", __func__, msg->val);

    if (msg->val < 100) {
        LibMT_Msg_t *msgToL = LibMT_MsgGet();
        msgToL->val = msg->val;
        LibMT_MsgToThread(msgToL, taskL);
    }

    //additional demo
    LibVCD_ClockAdd(LibTime_StopMicroSecondClock());
    LibVCD_ValueChange(0, 0);

    if (msg->val == 999) {
        return 1;  //return true for end of thread
    } else {
        return 0;
    }
}

int LibMT_Demo_ThreadL(LibMT_Msg_t *msg)
{
    LibMT_Msg_t *msgToH;

    //additional demo
    LibVCD_ClockAdd(LibTime_StopMicroSecondClock());
    LibVCD_ValueChange(1, 1);

    msgToH = LibMT_MsgGet();
    if (msg->val == 49)
        msgToH->val = 999;
    else
        msgToH->val = msg->val + 100;
    LibMT_MsgToThread(msgToH, taskH);

    //additional demo
    LibVCD_ClockAdd(LibTime_StopMicroSecondClock());
    LibVCD_ValueChange(1, 0);

    if (msg->val == 49) {
        return 1;  //return true for end of thread
    } else {
        return 0;
    }
}

void LibMT_Demo(void)
{
    if (Lib_IsMT()) {
        printf("Demo multi-threading mode now ...\n");
    } else {
        printf("Demo single-threading mode now ...\n");
    }

    //additional demo
    LibVCD_WireInfo_t test[2] = {
        {1, "H", VALUE_IN_FOLLOWING, 0},
        {1, "L", VALUE_IN_FOLLOWING, 0},
    };
    LibVCD_Init("example.vcd", 1, TIME_UNIT_US, test, 2);
    LibTime_StartMicroSecondClock();

    taskH = LibMT_CreateThread(LibMT_Demo_ThreadH);
    taskL = LibMT_CreateThread(LibMT_Demo_ThreadL);
    FOREACH_I(50) {
        LibMT_MsgToThreadLite(i, taskH);
        LibOs_SleepMiliSeconds(1);
    }
    LibMT_WaitMainThreadAndDestroyAll(taskH);

    //additional demo
    LibVCD_ClockAdd(LibTime_StopMicroSecondClock());
    LibVCD_Uninit();
}

#define DEMO_SAFE_PRINT ( 0 )
void *LibMT_Demo_Print1(void *dummy)
{
    LibOs_SleepMiliSeconds(10);
    FOREACH_I(50) {
#if DEMO_SAFE_PRINT
        SAFE_PRINT("%d\n", i);
#else
        printf("%d\n", i);
#endif
    }
    return NULL;
}

void *LibMT_Demo_Print2(void *dummy)
{
    FOREACH_I(50) {
#if DEMO_SAFE_PRINT
        SAFE_PRINT("%d\n", i+100000000);
#else
        printf("%d\n", i+100000000);
#endif
    }
    return NULL;
}

void LibMT_Demo_Safe_Print(void)
{
    taskH = LibMT_CreateThreadEx(LibMT_Demo_Print1);
    taskL = LibMT_CreateThreadEx(LibMT_Demo_Print2);
    LibMT_WaitMainThreadAndDestroyAll(taskH);
}