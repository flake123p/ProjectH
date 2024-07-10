
#include "Everything_Lib_Mgr.hpp"

#if 1
#define THREAD_SAFE_PROTECT_CODE_BLOCK_S(option)
#define THREAD_SAFE_PROTECT_START(option)
#define THREAD_SAFE_PROTECT_END(option)
#define THREAD_SAFE_PROTECT_CODE_BLOCK_E(option)
#else
#define THREAD_SAFE_PROTECT_CODE_BLOCK_S(option) {UBaseType_t  uxSavedInterruptStatus;
#define THREAD_SAFE_PROTECT_START(option)        uxSavedInterruptStatus = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
#define THREAD_SAFE_PROTECT_END(option)          OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, uxSavedInterruptStatus);
#define THREAD_SAFE_PROTECT_CODE_BLOCK_E(option) }
#endif

#define __________SINGLY_LINKED_LIST__________
#define __________SINGLY_LINKED_LIST_________

void UTL_SListHeadInit(SLList_Head_t *head)
{
    SLLIST_HEAD_RESET(head);
}

void UTL_SListInsertLast(SLList_Head_t *head, void *node)
{
    SLLIST_INSERT_LAST(head, node);
}

void UTL_SListInsertLastSafely(SLList_Head_t *head, void *node)
{
    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    UTL_SListInsertLast(head, node);

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);
}

void UTL_SListInsertLastSafelyEx(SLList_Head_t *head, u32 *nodeCtr, void *node)
{
    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    UTL_SListInsertLast(head, node);
    *nodeCtr += 1;

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);
}

void UTL_SListInsertFirst(SLList_Head_t *head, void *node)
{
    SLLIST_INSERT_AFTER(head, head, node);
}

void UTL_SListInsertFirstSafely(SLList_Head_t *head, void *node)
{
    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    UTL_SListInsertFirst(head, node);

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);
}

void UTL_SListInsertFirstSafelyEx(SLList_Head_t *head, u32 *nodeCtr, void *node)
{
    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    UTL_SListInsertFirst(head, node);
    *nodeCtr += 1;

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);
}

void *UTL_SListPopFirst(SLList_Head_t *head)
{
    void *node;
    if (SLLIST_IS_EMPTY(head))
        return NULL;

    node = SLLIST_FIRST(head);
    SLLIST_REMOVE_FIRST(head);
    return node;
}

void *UTL_SListPopFirstSafely(SLList_Head_t *head)
{
    void *node;
    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    node = UTL_SListPopFirst(head);

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);
    return node;
}

void *UTL_SListPopFirstSafelyEx(SLList_Head_t *head, u32 *nodeCtr)
{
    void *node;
    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    if (*nodeCtr == 0) {
        THREAD_SAFE_PROTECT_END(1);
        return NULL;
    }
    node = UTL_SListPopFirst(head);
    *nodeCtr -= 1;

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);
    return node;
}

int UTL_SListScanAndRemove(SLList_Head_t *head, Node_CB_t node_cb, Node_CB_t post_remove_cb, void *cb_argu)
{
    u32 curr_index = 0;
    SLList_Entry_t *curr, *prev;
    NODE_CB_RC_t node_cb_rc;

    prev = (SLList_Entry_t *)head;
    SLLIST_FOREACH(head, curr, SLList_Entry_t)
    {
        node_cb_rc = (*node_cb)(curr, cb_argu, curr_index);
        switch (node_cb_rc)
        {
            case NODE_CB_RC__DO_NOTHING:
                break;

            case NODE_CB_RC__REMOVE:
                SLLIST_REMOVE_NEXT(head, prev);
                (*post_remove_cb)(curr, cb_argu, curr_index);
                break;

            case NODE_CB_RC__STOP_SCAN:
                return 1;

            default:
                BASIC_ASSERT(0);
                break;
        }

        prev = curr;
        curr_index += 1;
    }

    return 0;
}

#define __________DOUBLY_LINKED_LIST__________
#define __________DOUBLY_LINKED_LIST_________

void UTL_DListHeadInit(DLList_Head_t *head)
{
    DLLIST_HEAD_RESET(head);
}

void UTL_DListInsertLast(DLList_Head_t *head, void *node)
{
    DLLIST_INSERT_LAST(head, node);
}

void UTL_DListInsertLastSafely(DLList_Head_t *head, void *node)
{
    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    UTL_DListInsertLast(head, node);

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);
}

void UTL_DListInsertLastSafelyEx(DLList_Head_t *head, u32 *nodeCtr, void *node)
{
    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    UTL_DListInsertLast(head, node);
    *nodeCtr += 1;

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);
}

void UTL_DListInsertFirst(DLList_Head_t *head, void *node)
{
    DLLIST_INSERT_AFTER(head, head, node);
}

void UTL_DListInsertFirstSafely(DLList_Head_t *head, void *node)
{
    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    UTL_DListInsertFirst(head, node);

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);
}

void UTL_DListInsertFirstSafelyEx(DLList_Head_t *head, u32 *nodeCtr, void *node)
{
    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    UTL_DListInsertFirst(head, node);
    *nodeCtr += 1;

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);
}

void *UTL_DListPopFirst(DLList_Head_t *head)
{
    void *node;
    if (DLLIST_IS_EMPTY(head))
        return NULL;

    node = DLLIST_FIRST(head);
    DLLIST_REMOVE_FIRST(head);
    return node;
}

void *UTL_DListPopFirstSafely(DLList_Head_t *head)
{
    void *node;
    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    node = UTL_DListPopFirst(head);

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);
    return node;
}

void *UTL_DListPopFirstSafelyEx(DLList_Head_t *head, u32 *nodeCtr)
{
    void *node;

    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    if (*nodeCtr == 0) {
        THREAD_SAFE_PROTECT_END(1);
        return NULL;
    }
    node = UTL_DListPopFirst(head);
    *nodeCtr -= 1;

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);

    return node;
}

void *UTL_DListPopLast(DLList_Head_t *head)
{
    void *node;
    if (DLLIST_IS_EMPTY(head))
        return NULL;

    node = DLLIST_LAST(head);
    DLLIST_REMOVE_LAST(head);
    return node;
}

void *UTL_DListPopLastSafely(DLList_Head_t *head)
{
    void *node;
    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    node = UTL_DListPopLast(head);

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);
    return node;
}

void *UTL_DListPopLastSafelyEx(DLList_Head_t *head, u32 *nodeCtr)
{
    void *node;

    THREAD_SAFE_PROTECT_CODE_BLOCK_S(1);
    THREAD_SAFE_PROTECT_START(1);

    if (*nodeCtr == 0) {
        THREAD_SAFE_PROTECT_END(1);
        return NULL;
    }
    node = UTL_DListPopLast(head);
    *nodeCtr -= 1;

    THREAD_SAFE_PROTECT_END(1);
    THREAD_SAFE_PROTECT_CODE_BLOCK_E(1);

    return node;
}

#define __________CMN_DESC__________
#define __________CMN_DESC_________

int UTL_CmnDescPushNew(Cmn_Desc_Info_t **head, Cmn_Desc_Info_t *new_node)
{
    CMN_DESC_DBG_LOOP_CTR_INIT();

    while (1)
    {
        if (*head == NULL)
        {
            *head = new_node;
            break;
        }
        else
        {
            head = (Cmn_Desc_Info_t **)*head;
        }
        CMN_DESC_DBG_LOOP_CTR_ADD_N_CHECK(1,100);
    }

    return 0;
}

Cmn_Desc_Info_t *UTL_CmnDescSearchIncomplete(Cmn_Desc_Info_t *curr)
{
    CMN_DESC_DBG_LOOP_CTR_INIT();

    if (curr == NULL)
        return NULL;

    while (1)
    {
        if (curr != NULL)
        {
            if (curr->done == 0)
                break;
            else
                curr = curr->next;
        }
        else
        {
            break;
        }

        CMN_DESC_DBG_LOOP_CTR_ADD_N_CHECK(1,100);
    }

    return curr;
}

u32 UTL_CmnDescPopAllComplete(Cmn_Desc_Info_t *head, Cmn_Desc_CB release_cb, void *cb_argu, int enable_force_pop)
{
    CMN_DESC_DBG_LOOP_CTR_INIT();

    u32 num_of_popped_desc = 0;
    Cmn_Desc_Info_t *prev = head;
    Cmn_Desc_Info_t *curr;

    for (curr = head->next; curr != NULL;)
    {
        if (enable_force_pop || curr->done)
        {
            Cmn_Desc_Info_t *popped_desc = curr;

            num_of_popped_desc++;
            curr = curr->next;
            prev->next = curr;

            if (release_cb != NULL)
                (*release_cb)(popped_desc, cb_argu);
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }

        CMN_DESC_DBG_LOOP_CTR_ADD_N_CHECK(1,100);
    }

    return num_of_popped_desc;
}

Cmn_Desc_Info_t *UTL_CmnDescPopFirstComplete(Cmn_Desc_Info_t **p_head)
{
    Cmn_Desc_Info_t *curr = *p_head;

    if (curr != NULL)
    {
        if (curr->done)
        {
            *p_head = curr->next;
            return curr;
        }
    }

    return NULL;
}

int LibMsg_QueueAryMsg_Init(LibMsg_QueueAryMsgInfo *msg_info, u32 max)
{
    msg_info->max = max;
    msg_info->send_index = 0;
    msg_info->receive_index = max - 1;
    return 0;
}

int LibMsg_QueueAryMsg_IsAryMsgFull(LibMsg_QueueAryMsgInfo *msg_info)
{
    return msg_info->send_index == msg_info->receive_index;
}

int LibMsg_QueueAryMsg_GetNextSendIndex(LibMsg_QueueAryMsgInfo *msg_info, u32 *out_send_index) //return 0 for success
{
    if (msg_info->send_index != msg_info->receive_index)
    {
        if (out_send_index != NULL)
        {
            *out_send_index = msg_info->send_index;
        }
        return 0;
    }

    return 1;
}

int LibMsg_QueueAryMsg_IncreaseSendIndex(LibMsg_QueueAryMsgInfo *msg_info) //return 0 for success
{
    if (msg_info->send_index != msg_info->receive_index)
    {
        msg_info->send_index = (msg_info->send_index + 1 == msg_info->max) ? 0 : msg_info->send_index + 1;
        return 0;
    }

    return 1;
}

int LibMsg_QueueAryMsg_GetNewReceiveIndex(LibMsg_QueueAryMsgInfo *msg_info, u32 *out_receive_index) //return 0 for success
{
    u32 next = (msg_info->receive_index + 1 == msg_info->max) ? 0 : msg_info->receive_index + 1;

    if (next != msg_info->send_index)
    {
        if (out_receive_index != NULL)
        {
            *out_receive_index = next;
        }
        return 0;
    }

    return 1;
}
int LibMsg_QueueAryMsg_IncreaseReceiveIndex(LibMsg_QueueAryMsgInfo *msg_info) //return 0 for success
{
    u32 next = (msg_info->receive_index + 1 == msg_info->max) ? 0 : msg_info->receive_index + 1;

    if (next != msg_info->send_index)
    {
        msg_info->receive_index = next;
        return 0;
    }

    return 1;
}

void LibMsg_QueueAryMsg_Dump(LibMsg_QueueAryMsgInfo *msg_info)
{
    DUMPD(msg_info->max);DUMPD(msg_info->send_index);DUMPND(msg_info->receive_index);
}


LibMsg_QueueAryMsgInfo g_demo_q_ary_msg;

void LibMsg_Demo(void)
{
    int ret;
    int isFull;
    u32 send_index = 99;
    u32 receive_index = 88;

    LibMsg_QueueAryMsg_Init(&g_demo_q_ary_msg, 4);
    LibMsg_QueueAryMsg_Dump(&g_demo_q_ary_msg);

    FOREACH_I(6)
    {
        isFull = LibMsg_QueueAryMsg_IsAryMsgFull(&g_demo_q_ary_msg);
        DUMPD(i);DUMPND(isFull);
        ret = LibMsg_QueueAryMsg_GetNextSendIndex(&g_demo_q_ary_msg, &send_index);
        DUMPD(ret);DUMPND(send_index);
        send_index = 99;
        ret = LibMsg_QueueAryMsg_IncreaseSendIndex(&g_demo_q_ary_msg);
        LibMsg_QueueAryMsg_Dump(&g_demo_q_ary_msg);
    }

    ret = LibMsg_QueueAryMsg_GetNewReceiveIndex(&g_demo_q_ary_msg, &receive_index);
    DUMPD(ret);DUMPND(receive_index);
    LibMsg_QueueAryMsg_Dump(&g_demo_q_ary_msg);
    PRINT_LINE;
    LibMsg_QueueAryMsg_GetNewReceiveIndex(&g_demo_q_ary_msg, &receive_index);
    ret = LibMsg_QueueAryMsg_IncreaseReceiveIndex(&g_demo_q_ary_msg);
    LibMsg_QueueAryMsg_Dump(&g_demo_q_ary_msg);DUMPD(ret);DUMPND(receive_index);

    ret = LibMsg_QueueAryMsg_IncreaseReceiveIndex(&g_demo_q_ary_msg);
    FOREACH_I(6)
    {
        isFull = LibMsg_QueueAryMsg_IsAryMsgFull(&g_demo_q_ary_msg);
        DUMPD(i);DUMPND(isFull);
        ret = LibMsg_QueueAryMsg_GetNextSendIndex(&g_demo_q_ary_msg, &send_index);
        DUMPD(ret);DUMPND(send_index);
        send_index = 99;
        ret = LibMsg_QueueAryMsg_IncreaseSendIndex(&g_demo_q_ary_msg);
        LibMsg_QueueAryMsg_Dump(&g_demo_q_ary_msg);
    }

    PRINT_LINE;
    LibMsg_QueueAryMsg_GetNewReceiveIndex(&g_demo_q_ary_msg, &receive_index);
    ret = LibMsg_QueueAryMsg_IncreaseReceiveIndex(&g_demo_q_ary_msg);
    LibMsg_QueueAryMsg_Dump(&g_demo_q_ary_msg);DUMPD(ret);DUMPND(receive_index);
    LibMsg_QueueAryMsg_GetNewReceiveIndex(&g_demo_q_ary_msg, &receive_index);
    ret = LibMsg_QueueAryMsg_IncreaseReceiveIndex(&g_demo_q_ary_msg);
    LibMsg_QueueAryMsg_Dump(&g_demo_q_ary_msg);DUMPD(ret);DUMPND(receive_index);
    LibMsg_QueueAryMsg_GetNewReceiveIndex(&g_demo_q_ary_msg, &receive_index);
    ret = LibMsg_QueueAryMsg_IncreaseReceiveIndex(&g_demo_q_ary_msg);
    LibMsg_QueueAryMsg_Dump(&g_demo_q_ary_msg);DUMPD(ret);DUMPND(receive_index);
    LibMsg_QueueAryMsg_GetNewReceiveIndex(&g_demo_q_ary_msg, &receive_index);
    ret = LibMsg_QueueAryMsg_IncreaseReceiveIndex(&g_demo_q_ary_msg);
    LibMsg_QueueAryMsg_Dump(&g_demo_q_ary_msg);DUMPD(ret);DUMPND(receive_index);

    FOREACH_I(0)
    {
        isFull = LibMsg_QueueAryMsg_IsAryMsgFull(&g_demo_q_ary_msg);
        DUMPD(i);DUMPND(isFull);
        ret = LibMsg_QueueAryMsg_GetNextSendIndex(&g_demo_q_ary_msg, &send_index);
        DUMPD(ret);DUMPND(send_index);
        send_index = 99;
        ret = LibMsg_QueueAryMsg_IncreaseSendIndex(&g_demo_q_ary_msg);
        LibMsg_QueueAryMsg_Dump(&g_demo_q_ary_msg);
    }

}

int LibMsg_RandomAryMsg_Init(LibMsg_RandomAryMsgInfo *msg_info, u32 max, u32 size_of_msg)
{
    LibMsg_RandomAryMsg_Flag *curr_msg;

    BASIC_ASSERT(size_of_msg >= sizeof(LibMsg_RandomAryMsg_Flag));
    msg_info->size_of_msg = size_of_msg;
    msg_info->size_of_msg_aligned = ALIGN_SIZE(size_of_msg, 8);
    
    msg_info->max = max;
    msg_info->search_ctr = 0;
    msg_info->start_ptr = (u8 *)malloc(max * msg_info->size_of_msg_aligned);
    FOREACH_I(max)
    {
        curr_msg = (LibMsg_RandomAryMsg_Flag *)(msg_info->start_ptr + (i * msg_info->size_of_msg_aligned));
        curr_msg->send_flag = 0;
    }
    return 0;
}

int LibMsg_RandomAryMsg_Uninit(LibMsg_RandomAryMsgInfo *msg_info)
{
    free(msg_info->start_ptr);
    return 0;
}

LibMsg_RandomAryMsg_Flag *LibMsg_RandomAryMsg_GetMsgToSend(LibMsg_RandomAryMsgInfo *msg_info)
{
    LibMsg_RandomAryMsg_Flag *curr_msg;

    FOREACH_I(msg_info->max)
    {
        if (msg_info->search_ctr == msg_info->max)
        {
            msg_info->search_ctr = 0;
        }
        curr_msg = (LibMsg_RandomAryMsg_Flag *)(msg_info->start_ptr + (msg_info->search_ctr * msg_info->size_of_msg_aligned));
        if (curr_msg->send_flag == 0)
        {
            msg_info->search_ctr++;
            return curr_msg;
        }
        msg_info->search_ctr++;
    }
    return NULL;
}

void LibMsg_RandomAryMsg_Dump(LibMsg_RandomAryMsgInfo *msg_info)
{
    LibMsg_RandomAryMsg_Flag *curr_msg;

    DUMPND(msg_info->max);
    DUMPND(msg_info->search_ctr);
    printf("send flag : ");
    FOREACH_I(msg_info->max)
    {
        curr_msg = (LibMsg_RandomAryMsg_Flag *)(msg_info->start_ptr + (i * msg_info->size_of_msg_aligned));
        printf("%d ", curr_msg->send_flag);
    }
    NEWLINE;
}

LibMsg_RandomAryMsgInfo g_demo_random_ary_msg;

void LibMsg_Demo2(void)
{
    LibMsg_RandomAryMsg_Flag *curr_msg;

    LibMsg_RandomAryMsg_Init(&g_demo_random_ary_msg, 4, 40);
    LibMsg_RandomAryMsg_Dump(&g_demo_random_ary_msg);

    curr_msg = LibMsg_RandomAryMsg_GetMsgToSend(&g_demo_random_ary_msg);
    curr_msg = LibMsg_RandomAryMsg_GetMsgToSend(&g_demo_random_ary_msg);
    curr_msg = LibMsg_RandomAryMsg_GetMsgToSend(&g_demo_random_ary_msg);curr_msg->send_flag = 1;
    curr_msg = LibMsg_RandomAryMsg_GetMsgToSend(&g_demo_random_ary_msg);curr_msg->send_flag = 2;
    curr_msg = LibMsg_RandomAryMsg_GetMsgToSend(&g_demo_random_ary_msg);curr_msg->send_flag = 3;
    curr_msg = LibMsg_RandomAryMsg_GetMsgToSend(&g_demo_random_ary_msg);curr_msg->send_flag = 4;

    LibMsg_RandomAryMsg_Dump(&g_demo_random_ary_msg);

    LibMsg_RandomAryMsg_Uninit(&g_demo_random_ary_msg);
}

int LibMsg_RandomAryMsg2_Init(LibMsg_RandomAryMsg2Info *msg_info, u32 max)
{
    BASIC_ASSERT(IS_SIZE_ALIGNED(sizeof(LibMsg_RandomAryMsg2_Flag), 8));

    msg_info->max = max;
    msg_info->search_ctr = 0;
    msg_info->start_ptr = (LibMsg_RandomAryMsg2_Flag *)malloc(max * sizeof(LibMsg_RandomAryMsg2_Flag));
    FOREACH_I(max)
    {
        msg_info->start_ptr[i].send_flag = 0;
    }
    return 0;
}

int LibMsg_RandomAryMsg2_Uninit(LibMsg_RandomAryMsg2Info *msg_info)
{
    free(msg_info->start_ptr);
    return 0;
}

LibMsg_RandomAryMsg2_Flag *LibMsg_RandomAryMsg2_GetMsgToSend(LibMsg_RandomAryMsg2Info *msg_info)
{
    LibMsg_RandomAryMsg2_Flag *curr_msg;

    FOREACH_I(msg_info->max)
    {
        if (msg_info->search_ctr == msg_info->max)
        {
            msg_info->search_ctr = 0;
        }
        curr_msg = &(msg_info->start_ptr[msg_info->search_ctr]);
        if (curr_msg->send_flag == 0)
        {
            msg_info->search_ctr++;
            return curr_msg;
        }
        msg_info->search_ctr++;
    }
    return NULL;
}

void LibMsg_RandomAryMsg2_Dump(LibMsg_RandomAryMsg2Info *msg_info)
{
    DUMPND(msg_info->max);
    DUMPND(msg_info->search_ctr);
    printf("send flag : ");
    FOREACH_I(msg_info->max)
    {
        printf("%d ", msg_info->start_ptr[i].send_flag);
    }
    NEWLINE;
}

LibMsg_RandomAryMsg2Info g_demo_random_ary_msg2;

void LibMsg_Demo3(void)
{
    LibMsg_RandomAryMsg2_Flag *curr_msg;

    LibMsg_RandomAryMsg2_Init(&g_demo_random_ary_msg2, 4);
    LibMsg_RandomAryMsg2_Dump(&g_demo_random_ary_msg2);

    curr_msg = LibMsg_RandomAryMsg2_GetMsgToSend(&g_demo_random_ary_msg2);
    curr_msg = LibMsg_RandomAryMsg2_GetMsgToSend(&g_demo_random_ary_msg2);curr_msg->send_flag = 1;
    curr_msg = LibMsg_RandomAryMsg2_GetMsgToSend(&g_demo_random_ary_msg2);curr_msg->send_flag = 2;
    curr_msg = LibMsg_RandomAryMsg2_GetMsgToSend(&g_demo_random_ary_msg2);curr_msg->send_flag = 3;
    curr_msg = LibMsg_RandomAryMsg2_GetMsgToSend(&g_demo_random_ary_msg2);curr_msg->send_flag = 4;

    LibMsg_RandomAryMsg2_Dump(&g_demo_random_ary_msg2);

    LibMsg_RandomAryMsg2_Uninit(&g_demo_random_ary_msg2);
}