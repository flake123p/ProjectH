

#ifndef __LIB_TIMER_HPP_INCLUDED_

#include "My_Types.h"
#include "_LibLinkedList.hpp" //LinkedListClass

#include "LibThread.hpp"
#include "_LibDesc.hpp"
#include "_LibMT.hpp"


// ============================== Debug ==============================

// ============================== Define ==============================
enum {
    TIMER_INIT,
    TIMER_UNINIT,
    TIMER_ADD,
    TIMER_REMOVE,
    TIMER_UPDATE,
    TIMER_RELOAD,
};

typedef struct Timer_Msg_t{
    DLList_Entry_t entry;
    u32 uniqueID;
    union {
        u32 sleep_period_in_ms; // for TIMER_INIT
        u32 sleep_time_in_ms_remain;
    } data;
    u32 sleep_time_in_ms;
    int (*timesupCB)(struct Timer_Msg_t *timerMsg); //return true for auto free timerMsg
    LibMT_ThreadInfo_t *callerThread;
    u32 toCallerMsgVal;
} Timer_Msg_t;
typedef int (*Timer_Timesup_CB)(Timer_Msg_t *timerMsg); //return true for auto free timerMsg

void *LibTimer_Thread(void *hdl);

LibMT_ThreadInfo_t *LibTimer_Create(void);
int LibTimer_Start(LibMT_ThreadInfo_t *timerThreadInfo, u32 sleep_period_in_ms);
int LibTimer_Destroy(LibMT_ThreadInfo_t *timerThreadInfo);
//return unique ID
u32 LibTimer_Add(LibMT_ThreadInfo_t *timerThreadInfo, u32 sleep_time_in_ms, Timer_Timesup_CB callback);
u32 LibTimer_AddLite(LibMT_ThreadInfo_t *timerThreadInfo, u32 sleep_time_in_ms, LibMT_ThreadInfo_t *callerThread, u32 toCallerMsgVal);
u32 LibTimer_AddLite_WithTimerMsg(LibMT_ThreadInfo_t *timerThreadInfo, u32 sleep_time_in_ms, LibMT_ThreadInfo_t *callerThread, u32 toCallerMsgVal);
int LibTimer_Remove(LibMT_ThreadInfo_t *timerThreadInfo, u32 uniqueID);
int LibTimer_Update(LibMT_ThreadInfo_t *timerThreadInfo, u32 uniqueID, u32 sleep_time_in_ms);
int LibTimer_Reload(LibMT_ThreadInfo_t *timerThreadInfo, u32 uniqueID);


void LibTimer_Demo(void);
void LibTimer_DemoEx(void);

#define __LIB_TIMER_HPP_INCLUDED_
#endif//__LIB_TIMER_HPP_INCLUDED_

