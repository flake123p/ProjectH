
#include "Everything_Lib_Mgr.hpp"

int _LibTimer_TemplateCallback(Timer_Msg_t *timerMsg)
{
    LibMT_Msg_t *msgToCaller = LibMT_MsgGet();
    msgToCaller->val = timerMsg->toCallerMsgVal;
    msgToCaller->id = timerMsg->uniqueID;
    msgToCaller->hdl = NULL;
    LibMT_MsgToThread(msgToCaller, timerMsg->callerThread);
    return 1;
}

int _LibTimer_TemplateCallback_WithTimerMsg(Timer_Msg_t *timerMsg)
{
    LibMT_Msg_t *msgToCaller = LibMT_MsgGet();
    msgToCaller->val = timerMsg->toCallerMsgVal;
    msgToCaller->id = timerMsg->uniqueID;
    msgToCaller->hdl = (void *)timerMsg;
    LibMT_MsgToThread(msgToCaller, timerMsg->callerThread);
    return 0;
}

void *LibTimer_Thread(void *hdl)
{
    LibMT_ThreadInfo_t *info = (LibMT_ThreadInfo_t *)hdl;
    LibMT_Msg_t *msg;
    Timer_Msg_t *timerMsg;
    u32 sleep_period_in_ms;
    DLList_Head_t jobsHead;
    int free_timer_msg;

    LibIPC_Event_Wait(info->evtHdl);
    msg = LibMT_MsgReceive(info);

    if (msg->val != TIMER_INIT) {
        SAFE_PRINT("You should use TIMER_INIT to init timer!! Error!!\n");
        return NULL;
    }

    timerMsg = (Timer_Msg_t *)msg->hdl;
    sleep_period_in_ms = timerMsg->data.sleep_period_in_ms;
    MM_FREE(timerMsg);
    LibMT_MsgRelease(msg);

    DLLIST_HEAD_RESET(&jobsHead);

    while (1)
    {
        Timer_Msg_t *currMsg;
        Timer_Msg_t *prevMsg;
        LibOs_SleepMiliSeconds(sleep_period_in_ms);
        //Countdown check
        DLLIST_WHILE_START(&jobsHead, currMsg, Timer_Msg_t)
        {
            prevMsg = currMsg;
            DLLIST_WHILE_NEXT(currMsg, Timer_Msg_t);
            if (prevMsg->data.sleep_time_in_ms_remain < sleep_period_in_ms) {
                DLLIST_REMOVE_NODE_SAFELY(&jobsHead, prevMsg);
                if ( (*(prevMsg->timesupCB))(prevMsg) ) {
                    MM_FREE(prevMsg);
                }
            } else {
                prevMsg->data.sleep_time_in_ms_remain -= sleep_period_in_ms;
            }
        }

        while (1)
        {
            msg = LibMT_MsgReceive(info);
            if (msg == NULL) {
                break;
            }
            timerMsg = (Timer_Msg_t *)msg->hdl;
            free_timer_msg = 1;
            switch (msg->val)
            {
                case TIMER_INIT:
                    SAFE_PRINT("This timer already inited ... rrror!!\n");
                    break;

                case TIMER_UNINIT: {
                    Timer_Msg_t *currMsg;
                    Timer_Msg_t *prevMsg;

                    DLLIST_WHILE_START(&jobsHead, currMsg, Timer_Msg_t)
                    {
                        prevMsg = currMsg;
                        DLLIST_WHILE_NEXT(currMsg, Timer_Msg_t);
                        MM_FREE(prevMsg);
                    }
                    if (timerMsg != NULL) {
                        MM_FREE(timerMsg);
                    }
                    LibMT_MsgRelease(msg);
                } return NULL; //end of thread

                case TIMER_ADD: {
                    DLLIST_INSERT_LAST(&jobsHead, timerMsg);
                    free_timer_msg = 0;
                } break;

                case TIMER_REMOVE: {
                    Timer_Msg_t *currMsg;

                    DLLIST_FOREACH(&jobsHead, currMsg, Timer_Msg_t)
                    {
                        if (currMsg->uniqueID == msg->id) {
                            DLLIST_REMOVE_NODE_SAFELY(&jobsHead, currMsg);
                            MM_FREE(currMsg);
                            break;
                        }
                    }
                } break;

                case TIMER_UPDATE: {
                    Timer_Msg_t *currMsg;

                    DLLIST_FOREACH(&jobsHead, currMsg, Timer_Msg_t)
                    {
                        if (currMsg->uniqueID == msg->id) {
                            currMsg->data.sleep_time_in_ms_remain = msg->para1;
                            currMsg->sleep_time_in_ms = msg->para1;
                            break;
                        }
                    }
                } break;

                case TIMER_RELOAD: {
                    Timer_Msg_t *currMsg;

                    DLLIST_FOREACH(&jobsHead, currMsg, Timer_Msg_t)
                    {
                        if (currMsg->uniqueID == msg->id) {
                            currMsg->data.sleep_time_in_ms_remain = currMsg->sleep_time_in_ms;
                            break;
                        }
                    }
                } break;

                default:
                    SAFE_PRINT("Invalid command(%d) for timer ... error!!\n", msg->val);
                    break;
            }
            if (free_timer_msg) {
                if (timerMsg != NULL) {
                    MM_FREE(timerMsg);
                }
            }
            LibMT_MsgRelease(msg);
        }
    }
}

LibMT_ThreadInfo_t *LibTimer_Create(void)
{
    return LibMT_CreateThreadEx(LibTimer_Thread);
}

int LibTimer_Start(LibMT_ThreadInfo_t *timerThreadInfo, u32 sleep_period_in_ms)
{
    LibMT_Msg_t *msg = LibMT_MsgGet();
    Timer_Msg_t *timerMsg = (Timer_Msg_t *)MM_ALLOC(sizeof(Timer_Msg_t));

    msg->val = TIMER_INIT;
    msg->hdl = (void *)timerMsg;
    timerMsg->data.sleep_period_in_ms = sleep_period_in_ms;

    LibMT_MsgToThread(msg, timerThreadInfo);
    return 0;
}

int LibTimer_Destroy(LibMT_ThreadInfo_t *timerThreadInfo)
{
    LibMT_Msg_t *msg = LibMT_MsgGet();

    msg->val = TIMER_UNINIT;
    msg->hdl = NULL;

    LibMT_MsgToThread(msg, timerThreadInfo);
    return 0;
}

//return unique ID
u32 LibTimer_Add(LibMT_ThreadInfo_t *timerThreadInfo, u32 sleep_time_in_ms, Timer_Timesup_CB callback)
{
    u32 uniqueID = LibUtil_GetUniqueU32();
    LibMT_Msg_t *msg = LibMT_MsgGet();
    Timer_Msg_t *timerMsg = (Timer_Msg_t *)MM_ALLOC(sizeof(Timer_Msg_t));

    msg->val = TIMER_ADD;
    msg->id  = uniqueID;
    msg->hdl = (void *)timerMsg;
    timerMsg->uniqueID = uniqueID;
    timerMsg->sleep_time_in_ms = sleep_time_in_ms;
    timerMsg->data.sleep_time_in_ms_remain = sleep_time_in_ms;
    timerMsg->timesupCB = callback;

    LibMT_MsgToThread(msg, timerThreadInfo);
    return uniqueID;
}

u32 LibTimer_AddLite(LibMT_ThreadInfo_t *timerThreadInfo, u32 sleep_time_in_ms, LibMT_ThreadInfo_t *callerThread, u32 toCallerMsgVal)
{
    u32 uniqueID = LibUtil_GetUniqueU32();
    LibMT_Msg_t *msg = LibMT_MsgGet();
    Timer_Msg_t *timerMsg = (Timer_Msg_t *)MM_ALLOC(sizeof(Timer_Msg_t));

    msg->val = TIMER_ADD;
    msg->id  = uniqueID;
    msg->hdl = (void *)timerMsg;
    timerMsg->uniqueID = uniqueID;
    timerMsg->sleep_time_in_ms = sleep_time_in_ms;
    timerMsg->data.sleep_time_in_ms_remain = sleep_time_in_ms;
    timerMsg->timesupCB = _LibTimer_TemplateCallback;
    timerMsg->callerThread = callerThread;
    timerMsg->toCallerMsgVal = toCallerMsgVal;

    LibMT_MsgToThread(msg, timerThreadInfo);
    return uniqueID;
}

u32 LibTimer_AddLite_WithTimerMsg(LibMT_ThreadInfo_t *timerThreadInfo, u32 sleep_time_in_ms, LibMT_ThreadInfo_t *callerThread, u32 toCallerMsgVal)
{
    u32 uniqueID = LibUtil_GetUniqueU32();
    LibMT_Msg_t *msg = LibMT_MsgGet();
    Timer_Msg_t *timerMsg = (Timer_Msg_t *)MM_ALLOC(sizeof(Timer_Msg_t));

    msg->val = TIMER_ADD;
    msg->id  = uniqueID;
    msg->hdl = (void *)timerMsg;
    timerMsg->uniqueID = uniqueID;
    timerMsg->sleep_time_in_ms = sleep_time_in_ms;
    timerMsg->data.sleep_time_in_ms_remain = sleep_time_in_ms;
    timerMsg->timesupCB = _LibTimer_TemplateCallback_WithTimerMsg;
    timerMsg->callerThread = callerThread;
    timerMsg->toCallerMsgVal = toCallerMsgVal;

    LibMT_MsgToThread(msg, timerThreadInfo);
    return uniqueID;
}

int LibTimer_Remove(LibMT_ThreadInfo_t *timerThreadInfo, u32 uniqueID)
{
    LibMT_Msg_t *msg = LibMT_MsgGet();

    msg->val = TIMER_REMOVE;
    msg->id  = uniqueID;
    msg->hdl = NULL;
    LibMT_MsgToThread(msg, timerThreadInfo);
    return 0;
}

int LibTimer_Update(LibMT_ThreadInfo_t *timerThreadInfo, u32 uniqueID, u32 sleep_time_in_ms)
{
    LibMT_Msg_t *msg = LibMT_MsgGet();

    msg->val = TIMER_UPDATE;
    msg->id  = uniqueID;
    msg->hdl = NULL;
    msg->para1 = sleep_time_in_ms;
    LibMT_MsgToThread(msg, timerThreadInfo);
    return 0;
}

int LibTimer_Reload(LibMT_ThreadInfo_t *timerThreadInfo, u32 uniqueID)
{
    LibMT_Msg_t *msg = LibMT_MsgGet();

    msg->val = TIMER_RELOAD;
    msg->id  = uniqueID;
    msg->hdl = NULL;
    LibMT_MsgToThread(msg, timerThreadInfo);
    return 0;
}

LibMT_ThreadInfo_t *gLibTimer_DemoTimer;
LibMT_ThreadInfo_t *gLibTimer_DemoTimer2;
int Demo_Timer_Timesup_CB(Timer_Msg_t *timerMsg)
{
    SAFE_PRINT("xxx id:%d\n", timerMsg->uniqueID);
    return 1;
}

void LibTimer_Demo(void)
{
    u32 id1, id2, id3, idx;
    gLibTimer_DemoTimer = LibTimer_Create();
    gLibTimer_DemoTimer2 = LibTimer_Create();
    LibTimer_Start(gLibTimer_DemoTimer, 100);
    LibTimer_Start(gLibTimer_DemoTimer2, 50);
    id1 = LibTimer_Add(gLibTimer_DemoTimer, 1000, Demo_Timer_Timesup_CB);
    id2 = LibTimer_Add(gLibTimer_DemoTimer, 2000, Demo_Timer_Timesup_CB);
    id3 = LibTimer_Add(gLibTimer_DemoTimer, 3000, Demo_Timer_Timesup_CB);
    idx = LibTimer_Add(gLibTimer_DemoTimer2, 4000, Demo_Timer_Timesup_CB);
    SAFE_PRINT("id1:%d\n", id1);
    SAFE_PRINT("id2:%d\n", id2);
    SAFE_PRINT("id3:%d\n", id3);
    SAFE_PRINT("idx:%d\n", idx);
    LibTimer_Remove(gLibTimer_DemoTimer, id2);

    LibOs_SleepMiliSeconds(5000);
    SAFE_PRINT("Demo over ... \n");
    LibTimer_Destroy(gLibTimer_DemoTimer);
    LibTimer_Destroy(gLibTimer_DemoTimer2);

    LibMT_WaitMainThreadAndDestroyAll(gLibTimer_DemoTimer);
}

LibMT_ThreadInfo_t *gLibTimer_DemoTask;

int LibTimer_DemoTask(LibMT_Msg_t *msg)
{
    SAFE_PRINT("%s() %d\n", __func__, msg->val);
    if (msg->hdl != NULL) {
        MM_FREE(msg->hdl);
    }
    LibTimer_Destroy(gLibTimer_DemoTimer);
    return 1;  //return true for end of thread
}

void LibTimer_DemoEx(void)
{
    u32 id1;
    gLibTimer_DemoTimer = LibTimer_Create();
    gLibTimer_DemoTask = LibMT_CreateThread(LibTimer_DemoTask);
    LibTimer_Start(gLibTimer_DemoTimer, 100);
    id1 = LibTimer_AddLite(gLibTimer_DemoTimer, 2000, gLibTimer_DemoTask, 979);
    SAFE_PRINT("id1:%d\n", id1);
    LibMT_WaitMainThreadAndDestroyAll(gLibTimer_DemoTask);
    MM_ALLOC(8);
}