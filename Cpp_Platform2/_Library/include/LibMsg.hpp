

#ifndef _LIB_MSG_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================

// ============================== Library: Cross-Platform (Manager) ==============================
#include "My_Types.h"
#include "_LibLinkedList.hpp"



typedef enum {
    NODE_CB_RC__DO_NOTHING,
    NODE_CB_RC__REMOVE,
    NODE_CB_RC__STOP_SCAN,
} NODE_CB_RC_t;
typedef NODE_CB_RC_t (*Node_CB_t)(void *curr_node, void *argu, u32 curr_index);

#define __________SINGLY_LINKED_LIST__________
#define __________SINGLY_LINKED_LIST_________
void UTL_SListHeadInit(SLList_Head_t *head);
void UTL_SListInsertLast(SLList_Head_t *head, void *node);
void UTL_SListInsertLastSafely(SLList_Head_t *head, void *node);
void UTL_SListInsertLastSafelyEx(SLList_Head_t *head, u32 *nodeCtr, void *node);
void UTL_SListInsertFirst(SLList_Head_t *head, void *node);
void UTL_SListInsertFirstSafely(SLList_Head_t *head, void *node);
void UTL_SListInsertFirstSafelyEx(SLList_Head_t *head, u32 *nodeCtr, void *node);
void *UTL_SListPopFirst(SLList_Head_t *head);
void *UTL_SListPopFirstSafely(SLList_Head_t *head);
void *UTL_SListPopFirstSafelyEx(SLList_Head_t *head, u32 *nodeCtr);



#define __________DOUBLY_LINKED_LIST__________
#define __________DOUBLY_LINKED_LIST_________
void UTL_DListHeadInit(DLList_Head_t *head);
void UTL_DListInsertLast(DLList_Head_t *head, void *node);
void UTL_DListInsertLastSafely(DLList_Head_t *head, void *node);
void UTL_DListInsertLastSafelyEx(DLList_Head_t *head, u32 *nodeCtr, void *node);
void UTL_DListInsertFirst(DLList_Head_t *head, void *node);
void UTL_DListInsertFirstSafely(DLList_Head_t *head, void *node);
void UTL_DListInsertFirstSafelyEx(DLList_Head_t *head, u32 *nodeCtr, void *node);
void *UTL_DListPopFirst(DLList_Head_t *head);
void *UTL_DListPopFirstSafely(DLList_Head_t *head);
void *UTL_DListPopFirstSafelyEx(DLList_Head_t *head, u32 *nodeCtr);
void *UTL_DListPopLast(DLList_Head_t *head);
void *UTL_DListPopLastSafely(DLList_Head_t *head);
void *UTL_DListPopLastSafelyEx(DLList_Head_t *head, u32 *nodeCtr);



#define __________CMN_DESC__________
#define __________CMN_DESC_________

#define CMN_DESC_DBG_ENABLE ( 0 )
#if CMN_DESC_DBG_ENABLE
#define CMN_DESC_ASSERT BASIC_ASSERT
#define CMN_DESC_DBG_LOOP_CTR_INIT() u32 cmn_desc_dbg_loop_ctr=0
#define CMN_DESC_DBG_LOOP_CTR_ADD_N_CHECK(increment,max) cmn_desc_dbg_loop_ctr+=increment;CMN_DESC_ASSERT(cmn_desc_dbg_loop_ctr<max)
#else //if not : LM_DBG_ENABLE
#define CMN_DESC_ASSERT(...)
#define CMN_DESC_DBG_LOOP_CTR_INIT(...)
#define CMN_DESC_DBG_LOOP_CTR_ADD_N_CHECK(...)
#endif
typedef struct Cmn_Desc_Info_t{
    struct Cmn_Desc_Info_t *next; //May change to TAIL_Q if the delay is acceptable
    uint8_t done;
} Cmn_Desc_Info_t;
typedef int (*Cmn_Desc_CB)(Cmn_Desc_Info_t *desc_info, void *argu);

int UTL_CmnDescPushNew(Cmn_Desc_Info_t **head, Cmn_Desc_Info_t *new_node);
Cmn_Desc_Info_t *UTL_CmnDescSearchIncomplete(Cmn_Desc_Info_t *curr);
u32 UTL_CmnDescPopAllComplete(Cmn_Desc_Info_t *head, Cmn_Desc_CB release_cb, void *cb_argu, int enable_force_pop);
Cmn_Desc_Info_t *UTL_CmnDescPopFirstComplete(Cmn_Desc_Info_t **p_head);

/*
    QueueAryMsg module: (THREAD SAFE)
        send_index:    task 1 write, task 2 read
        receive_index: task 2 write, task 1 read
*/
typedef struct {
    u32 max;
    u32 send_index;
    u32 receive_index;
} LibMsg_QueueAryMsgInfo;
int LibMsg_QueueAryMsg_Init(LibMsg_QueueAryMsgInfo *msg_info, u32 max);
int LibMsg_QueueAryMsg_IsAryMsgFull(LibMsg_QueueAryMsgInfo *msg_info, u32 max);
int LibMsg_QueueAryMsg_IsOkToSend(LibMsg_QueueAryMsgInfo *msg_info);
int LibMsg_QueueAryMsg_GetNextSendIndex(LibMsg_QueueAryMsgInfo *msg_info, u32 *out_send_index); //return 0 for success
int LibMsg_QueueAryMsg_IncreaseSendIndex(LibMsg_QueueAryMsgInfo *msg_info); //return 0 for success
int LibMsg_QueueAryMsg_GetNewReceiveIndex(LibMsg_QueueAryMsgInfo *msg_info, u32 *out_receive_index); //return 0 for success
int LibMsg_QueueAryMsg_IncreaseReceiveIndex(LibMsg_QueueAryMsgInfo *msg_info); //return 0 for success
void LibMsg_QueueAryMsg_Dump(LibMsg_QueueAryMsgInfo *msg_info);

/*
    RandomAryMsg module: (THREAD SAFE)
        send flag: (make sure it's atom variable, like u32 in 32bit cpu)
            task 1: only write it to 1 from 0
            task 2: only write it to 0 from 1
*/
typedef struct {
    u32 send_flag;
    u32 dummy_in_64bit_cpu;
} LibMsg_RandomAryMsg_Flag;
typedef struct {
    u32 max;
    u32 search_ctr;
    u32 size_of_msg; // equal to or bigger than LibMsg_RandomAryMsg_Flag
    u32 size_of_msg_aligned;
    u8 *start_ptr;
} LibMsg_RandomAryMsgInfo;
int LibMsg_RandomAryMsg_Init(LibMsg_RandomAryMsgInfo *msg_info, u32 max, u32 size_of_msg);
int LibMsg_RandomAryMsg_Uninit(LibMsg_RandomAryMsgInfo *msg_info);
LibMsg_RandomAryMsg_Flag *LibMsg_RandomAryMsg_GetMsgToSend(LibMsg_RandomAryMsgInfo *msg_info);
void LibMsg_RandomAryMsg_Dump(LibMsg_RandomAryMsgInfo *msg_info);
#define RANDOM_ARY_MSG_DATA_SIZE (16)
typedef struct {
    u32 send_flag;
    u32 dummy_in_64bit_cpu;
    u8 data[RANDOM_ARY_MSG_DATA_SIZE];
} LibMsg_RandomAryMsg2_Flag;
typedef struct {
    u32 max;
    u32 search_ctr;
    LibMsg_RandomAryMsg2_Flag *start_ptr;
} LibMsg_RandomAryMsg2Info;
int LibMsg_RandomAryMsg2_Init(LibMsg_RandomAryMsg2Info *msg_info, u32 max);
int LibMsg_RandomAryMsg2_Uninit(LibMsg_RandomAryMsg2Info *msg_info);
LibMsg_RandomAryMsg2_Flag *LibMsg_RandomAryMsg2_GetMsgToSend(LibMsg_RandomAryMsg2Info *msg_info);
void LibMsg_RandomAryMsg2_Dump(LibMsg_RandomAryMsg2Info *msg_info);



void LibMsg_Demo(void);
void LibMsg_Demo2(void);
void LibMsg_Demo3(void);



#define _LIB_MSG_HPP_INCLUDED_
#endif//_LIB_MSG_HPP_INCLUDED_



