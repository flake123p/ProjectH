

#ifndef __LIB_MT_HPP_INCLUDED_

#include "My_Types.h"
#include "_LibLinkedList.hpp" //LinkedListClass

#include "LibThread.hpp"
//#include "_LibDesc.hpp"

// ============================== Debug ==============================

// ============================== Define ==============================
typedef enum {
    MUTEX_SAFE_PRINT,
    MUTEX_TEST,
    MUTEX_LIB_VCD,
    MUTEX_LIB_TIME,
 
    MUTEX_TOTAL_NUM,
} LIBMT_MUTEX_t;
int LibMT_Mutex_Lock(LIBMT_MUTEX_t index);
int LibMT_Mutex_Unlock(LIBMT_MUTEX_t index);
#define MUTEX_SAFE_PRINT_LOCK   LibMT_Mutex_Lock(MUTEX_SAFE_PRINT)
#define MUTEX_SAFE_PRINT_UNLOCK LibMT_Mutex_Unlock(MUTEX_SAFE_PRINT)
#define MUTEX_TEST_LOCK         LibMT_Mutex_Lock(MUTEX_TEST)
#define MUTEX_TEST_UNLOCK       LibMT_Mutex_Unlock(MUTEX_TEST)
#define MUTEX_LIB_VCD_LOCK      LibMT_Mutex_Lock(MUTEX_LIB_VCD)
#define MUTEX_LIB_VCD_UNLOCK    LibMT_Mutex_Unlock(MUTEX_LIB_VCD)
#define MUTEX_LIB_TIME_LOCK      LibMT_Mutex_Lock(MUTEX_LIB_TIME)
#define MUTEX_LIB_TIME_UNLOCK    LibMT_Mutex_Unlock(MUTEX_LIB_TIME)
#define SAFE_PRINT(...) MUTEX_SAFE_PRINT_LOCK;printf(__VA_ARGS__);MUTEX_SAFE_PRINT_UNLOCK

typedef struct {
    //int initiated;
    MUTEX_HANDLE_t handle;
} LibMT_UtilMutex_t; //init with = {0, NULL};

typedef struct {
    DLList_Entry_t list_entry;
    int is_pre_allocate;
    u32 val;
    u32 id;
    u32 para1;
    u32 para2;
    void *hdl;
} LibMT_Msg_t;

typedef int (*LibMT_EntryFunc)(LibMT_Msg_t *msg); //return true for end of thread
typedef struct {
    DLList_Entry_t internal_entry;

    THREAD_HANDLE_t threadHdl;
    EVENT_HANDLE_t  evtHdl;
    MUTEX_HANDLE_t  msgLock;
    DLList_Head_t   msgHead;

    LibMT_EntryFunc func;
} LibMT_ThreadInfo_t;

//destroy in LibMT_Uninit();
int LibMT_UtilMutex_Init(LibMT_UtilMutex_t *mutex);
int LibMT_UtilMutex_Uninit(LibMT_UtilMutex_t *mutex);
int LibMT_UtilMutex_Lock(LibMT_UtilMutex_t *mutex);
int LibMT_UtilMutex_Unlock(LibMT_UtilMutex_t *mutex);
void LibMT_UtilMutex_Demo(void);

int LibMT_Init(void);
int LibMT_Uninit(void);
LibMT_Msg_t *LibMT_MsgGet(void);
int LibMT_MsgRelease(LibMT_Msg_t *msg);
int LibMT_MsgToThread(LibMT_Msg_t *msg, LibMT_ThreadInfo_t *info);
int LibMT_MsgToThreadLite(u32 val, LibMT_ThreadInfo_t *info);
LibMT_Msg_t *LibMT_MsgReceive(LibMT_ThreadInfo_t *info);

LibMT_ThreadInfo_t *LibMT_CreateThread(LibMT_EntryFunc func);
LibMT_ThreadInfo_t *LibMT_CreateThreadEx(ThreadEntryFunc func);
int LibMT_WaitThreadAndDestroy(LibMT_ThreadInfo_t *info);
int LibMT_WaitMainThreadAndDestroyAll(LibMT_ThreadInfo_t *info = NULL);

void LibMT_Demo(void);
void LibMT_Demo_Safe_Print(void);

#define __LIB_MT_HPP_INCLUDED_
#endif//__LIB_MT_HPP_INCLUDED_

