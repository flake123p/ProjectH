

#ifndef _LIB_MSG_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================

// ============================== Library: Cross-Platform (Manager) ==============================
#include "My_Types.h"

typedef struct {
    void *next;
} SLList_Entry_t;
typedef struct {
    void *head;
    void *tail_or_self; //pointer to head struct when list is empty
} SLList_Head_t;
#define SLLIST_NEXT(node) ((SLList_Entry_t *)node)->next
#define SLLIST_HEAD(node) ((SLList_Head_t *)node)->head
#define SLLIST_TAIL(node) ((SLList_Head_t *)node)->tail_or_self
#define SLLIST_TAIL_IS_VALID(head,tail) ((void *)tail != (void *)head) /*tail_or_self is a pointer to head struct when list is empty*/

#define SLLIST_IS_EMPTY(head)     (SLLIST_HEAD(head)==NULL)
#define SLLIST_IS_NOT_EMPTY(head) (!SLLIST_IS_EMPTY(head))
#define SLLIST_FIRST(head)        SLLIST_HEAD(head)
#define SLLIST_LAST(head)         SLLIST_IS_EMPTY(head)?NULL:SLLIST_TAIL(head)

#define SLLIST_HEAD_INIT(head) {NULL,(void *)head}
#define SLLIST_HEAD_RESET(head) SLLIST_HEAD(head)=NULL; SLLIST_TAIL(head)=(void *)head;

#define SLLIST_FOREACH(head,curr,type) for(curr=(type *)SLLIST_HEAD(head); curr!=NULL; curr=(type *)SLLIST_NEXT(curr))

#define SLLIST_INSERT_LAST(head,new_node) \
    SLLIST_NEXT(new_node) = NULL;\
    SLLIST_NEXT(SLLIST_TAIL(head)) = (void *)new_node;\
    SLLIST_TAIL(head)=(void *)new_node;

#define SLLIST_INSERT_AFTER(head,node,new_node) \
    if (SLLIST_NEXT(node) == NULL) {\
        SLLIST_INSERT_LAST(head,new_node);/*update tail*/\
    } else { \
        SLLIST_NEXT(new_node) = SLLIST_NEXT(node);\
        SLLIST_NEXT(node) = (void *)new_node;\
    }

#define SLLIST_REMOVE_FIRST(head) \
    if (SLLIST_NEXT(SLLIST_HEAD(head)) == NULL) {\
        SLLIST_HEAD_RESET(head);/*update tail*/\
    } else {\
        SLLIST_HEAD(head) = SLLIST_NEXT(SLLIST_HEAD(head)); \
    }

#define SLLIST_REMOVE_FIRST_SAFELY(head) \
    if (SLLIST_IS_NOT_EMPTY(head)) {\
        SLLIST_REMOVE_FIRST(head);\
    }

#define SLLIST_REMOVE_NEXT(head, node) \
    if (SLLIST_NEXT(node) == SLLIST_TAIL(head)) {\
        SLLIST_TAIL(head) = (void *)node;/*update tail*/\
    }\
    SLLIST_NEXT(node) = SLLIST_NEXT(SLLIST_NEXT(node));

#define SLLIST_REMOVE_NEXT_SAFELY(head, node) \
    if (SLLIST_NEXT(node) != NULL) {\
        SLLIST_REMOVE_NEXT(head, node);\
    }

typedef struct {
    void *next;
    void *prev;
} DLList_Entry_t;
typedef struct {
    void *head;
    void *tail_or_self; //pointer to head struct when list is empty
} DLList_Head_t;
#define DLLIST_NEXT(node) ((DLList_Entry_t *)node)->next
#define DLLIST_PREV(node) ((DLList_Entry_t *)node)->prev
#define DLLIST_HEAD(node) ((DLList_Head_t *)node)->head
#define DLLIST_TAIL(node) ((DLList_Head_t *)node)->tail_or_self
#define DLLIST_TAIL_IS_VALID(head,tail) ((void *)tail != (void *)head) /*tail_or_self is a pointer to head struct when list is empty*/

#define DLLIST_IS_EMPTY(head) (DLLIST_HEAD(head)==NULL)
#define DLLIST_IS_NOT_EMPTY(head) (!DLLIST_IS_EMPTY(head))
#define DLLIST_FIRST(head) DLLIST_HEAD(head)
#define DLLIST_LAST(head)  DLLIST_IS_EMPTY(head)?NULL:DLLIST_TAIL(head)

#define DLLIST_HEAD_INIT(head) {NULL,(void *)head}
#define DLLIST_HEAD_RESET(head) DLLIST_HEAD(head)=NULL; DLLIST_TAIL(head)=(void *)head;

#define DLLIST_FOREACH(head,curr,type) for(curr=(type *)DLLIST_HEAD(head); curr!=NULL; curr=(type *)DLLIST_NEXT(curr))

#define DLLIST_INSERT_LAST(head,new_node) \
    DLLIST_NEXT(new_node) = NULL;\
    DLLIST_PREV(new_node) = DLLIST_TAIL(head);\
    DLLIST_NEXT(DLLIST_TAIL(head)) = (void *)new_node;\
    DLLIST_TAIL(head)=(void *)new_node;\

#define DLLIST_INSERT_AFTER(head,node,new_node) \
    if (DLLIST_NEXT(node) == NULL) {\
        DLLIST_INSERT_LAST(head,new_node);/*update tail*/\
    } else { \
        DLLIST_NEXT(new_node) = DLLIST_NEXT(node);\
        DLLIST_PREV(new_node) = (void *)node;\
        DLLIST_PREV(DLLIST_NEXT(node)) = (void *)new_node;\
        DLLIST_NEXT(node) = (void *)new_node;\
    } \

#define DLLIST_REMOVE_FIRST(head) \
    if (DLLIST_NEXT(DLLIST_HEAD(head)) == NULL) {\
        DLLIST_HEAD_RESET(head);/*update tail*/\
    } else {\
        DLLIST_PREV(DLLIST_NEXT(DLLIST_HEAD(head))) = (void *)(head);\
        DLLIST_HEAD(head) = DLLIST_NEXT(DLLIST_HEAD(head)); \
    }

#define DLLIST_REMOVE_FIRST_SAFELY(head) \
    if (DLLIST_IS_NOT_EMPTY(head)) {\
        DLLIST_REMOVE_FIRST(head);\
    }

#define DLLIST_REMOVE_LAST(head) \
    DLLIST_NEXT(DLLIST_PREV(DLLIST_TAIL(head))) = NULL;\
    DLLIST_TAIL(head) = DLLIST_PREV(DLLIST_TAIL(head));\

#define DLLIST_REMOVE_LAST_SAFELY(head) \
    if (DLLIST_IS_NOT_EMPTY(head)) {\
        DLLIST_REMOVE_LAST(head);\
    }

#define DLLIST_REMOVE_NODE(head, node) \
    if (DLLIST_NEXT(node) == NULL) {\
        DLLIST_REMOVE_LAST(head);\
    } else {\
        DLLIST_PREV(DLLIST_NEXT(node)) = DLLIST_PREV(node);\
        DLLIST_NEXT(DLLIST_PREV(node)) = DLLIST_NEXT(node);\
    }\

#define DLLIST_REMOVE_NODE_SAFELY(head, node) \
    if (DLLIST_IS_NOT_EMPTY(head)) {\
        if (node != head) {\
            DLLIST_REMOVE_NODE(head);\
        }\
    }

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

#define _LIB_MSG_HPP_INCLUDED_
#endif//_LIB_MSG_HPP_INCLUDED_



