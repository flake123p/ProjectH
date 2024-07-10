

#ifndef __LIB_LINKED_LIST_HPP_INCLUDED_

#include "My_Types.h"

// ============================== Debug ==============================
#define LINKED_LIST_LOG  (0)
#define LINKED_LIST_WARN (0)
#define LINKED_LIST_ERR  (0)

#if LINKED_LIST_LOG
#define LINKED_LIST_LOG_MSG printf
#else
#define LINKED_LIST_LOG_MSG(...)
#endif

#if LINKED_LIST_WARN
#define LINKED_LIST_WARN_MSG printf
#else
#define LINKED_LIST_WARN_MSG(...)
#endif

#if LINKED_LIST_ERR
#define LINKED_LIST_ERR_MSG printf
#else
#define LINKED_LIST_ERR_MSG(...)
#endif

// ============================== Define ==============================
typedef struct LinkedListNode {
	LinkedListNode *next;
	LinkedListNode *prev;
} LinkedListNode;

typedef struct LinkedListInfo {
	LinkedListNode *head;
	LinkedListNode *tail;
	u32 count;
} LinkedListInfo;

#define NODE_IS_HEAD(node) ((node)->prev==NULL)
#define NODE_IS_TAIL(node) ((node)->next==NULL)
#define NODE_IS_ONLY_ONE(node) (NODE_IS_HEAD(node)&&NODE_IS_TAIL(node))

//Error Code
#define NODE_ISNT_IN_LIST     (1)


typedef enum {
	DO_NOT_AUTO_FREE,
	DO_AUTO_FREE,
}AUTO_FREE;
void LibLinkedList_InitInfo(LinkedListInfo *info);
u32 LibLinkedList_Length(LinkedListInfo *info);
void LibLinkedList_PushBack(LinkedListInfo *info, LinkedListNode *newNode);
void LibLinkedList_PopBack(LinkedListInfo *info, AUTO_FREE autoFree = DO_NOT_AUTO_FREE);
int LibLinkedList_Remove(LinkedListInfo *info, LinkedListNode *nodeForRemove, AUTO_FREE autoFree = DO_NOT_AUTO_FREE);
void LibLinkedList_RemoveAll(LinkedListInfo *info, AUTO_FREE autoFree = DO_NOT_AUTO_FREE);
int LibLinkedList_InsertBack(LinkedListInfo *info, LinkedListNode *nodeBeInserted, LinkedListNode *nodeForInsert);
int LibLinkedList_InsertFront(LinkedListInfo *info, LinkedListNode *nodeBeInserted, LinkedListNode *nodeForInsert);
int LibLinkedList_IsThisNodeInList(LinkedListInfo *info, LinkedListNode *nodeForCheck);
void LibLinkedList_Dump(LinkedListInfo *info);
void LibLinkedList_Diagnose(LinkedListInfo *info, const char *caller);


class LinkedListClass{
public:
	LinkedListInfo info;
	
	LinkedListClass(){InitInfo();}; // Do nothing.
	~LinkedListClass(void){}; // Do nothing.

	void InitInfo(void);
	u32  Length(void);
	void PushBack(LinkedListNode *newNode);
	void PopBack(AUTO_FREE autoFree = DO_NOT_AUTO_FREE);
	int  Remove(LinkedListNode *nodeForRemove, AUTO_FREE autoFree = DO_NOT_AUTO_FREE);
	void RemoveAll(AUTO_FREE autoFree = DO_NOT_AUTO_FREE);
	int  InsertBack(LinkedListNode *nodeBeInserted, LinkedListNode *nodeForInsert);
	int  InsertFront(LinkedListNode *nodeBeInserted, LinkedListNode *nodeForInsert);
	int  IsThisNodeInList(LinkedListNode *nodeForCheck);
	void Dump(void);
	void Diagnose(const char *caller);
};



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
#define SLLIST_WHILE_START(head,curr,type) curr=(type *)SLLIST_HEAD(head);while(curr!=NULL)
#define SLLIST_WHILE_NEXT(curr,type) curr=(type *)SLLIST_NEXT(curr)

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

#define SLLIST_INSERT_FIRST(head,new_node) \
    SLLIST_INSERT_AFTER(head,head,new_node)

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
#define DLLIST_NEXT(pNode) ((DLList_Entry_t *)pNode)->next
#define DLLIST_PREV(pNode) ((DLList_Entry_t *)pNode)->prev
#define DLLIST_HEAD(pHead) ((DLList_Head_t *)pHead)->head
#define DLLIST_TAIL(pHead) ((DLList_Head_t *)pHead)->tail_or_self
#define DLLIST_TAIL_IS_VALID(head,tail) ((void *)tail != (void *)head) /*tail_or_self is a pointer to head struct when list is empty*/

#define DLLIST_IS_EMPTY(head) (DLLIST_HEAD(head)==NULL)
#define DLLIST_IS_NOT_EMPTY(head) (!DLLIST_IS_EMPTY(head))
#define DLLIST_FIRST(head) DLLIST_HEAD(head)
#define DLLIST_LAST(head)  DLLIST_IS_EMPTY(head)?NULL:DLLIST_TAIL(head)

#define DLLIST_HEAD_INIT(head) {NULL,(void *)head}
#define DLLIST_HEAD_RESET(head) DLLIST_HEAD(head)=NULL; DLLIST_TAIL(head)=(void *)head;

#define DLLIST_FOREACH(head,curr,type) for(curr=(type *)DLLIST_HEAD(head); curr!=NULL; curr=(type *)DLLIST_NEXT(curr))
#define DLLIST_WHILE_START(head,curr,type) curr=(type *)DLLIST_HEAD(head);while(curr!=NULL)
#define DLLIST_WHILE_NEXT(curr,type) curr=(type *)DLLIST_NEXT(curr)

#define DLLIST_INSERT_FIRST(head,new_node) \
    if (DLLIST_IS_EMPTY(head)) {\
        DLLIST_HEAD(head) = (void *)new_node;\
        DLLIST_TAIL(head) = (void *)new_node;\
        DLLIST_NEXT(new_node) = NULL;\
    } else { \
        DLLIST_PREV(DLLIST_HEAD(head)) = (void *)new_node;\
        DLLIST_NEXT(new_node) = DLLIST_HEAD(head);\
        DLLIST_HEAD(head) = (void *)new_node;\
    }\

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
    if (DLLIST_HEAD(head) == node) {\
        DLLIST_REMOVE_FIRST(head);\
    } else {\
        if (DLLIST_NEXT(node) == NULL) {\
            DLLIST_REMOVE_LAST(head);\
        } else {\
            DLLIST_PREV(DLLIST_NEXT(node)) = DLLIST_PREV(node);\
            DLLIST_NEXT(DLLIST_PREV(node)) = DLLIST_NEXT(node);\
        }\
    }\

#define DLLIST_REMOVE_NODE_SAFELY(head, node) \
    if (DLLIST_IS_NOT_EMPTY(head)) {\
        if ((void *)node != (void *)head) {\
            DLLIST_REMOVE_NODE(head, node);\
        }\
    }

// for application
/*
#if USE_DOUBLY_LIST
#define LLIST_NEXT                  DLLIST_NEXT
#define LLIST_PREV                  DLLIST_PREV
#define LLIST_HEAD                  DLLIST_HEAD
#define LLIST_TAIL                  DLLIST_TAIL
#define LLIST_TAIL_IS_VALID         DLLIST_TAIL_IS_VALID
#define LLIST_IS_EMPTY              DLLIST_IS_EMPTY
#define LLIST_IS_NOT_EMPTY          DLLIST_IS_NOT_EMPTY
#define LLIST_FIRST                 DLLIST_FIRST
#define LLIST_LAST                  DLLIST_LAST
#define LLIST_HEAD_INIT             DLLIST_HEAD_INIT
#define LLIST_HEAD_RESET            DLLIST_HEAD_RESET
#define LLIST_FOREACH               DLLIST_FOREACH
#define LLIST_WHILE_START           DLLIST_WHILE_START
#define LLIST_WHILE_NEXT            DLLIST_WHILE_NEXT
#define LLIST_INSERT_FIRST          DLLIST_INSERT_FIRST
#define LLIST_INSERT_LAST           DLLIST_INSERT_LAST
#define LLIST_INSERT_AFTER          DLLIST_INSERT_AFTER
#define LLIST_REMOVE_FIRST          DLLIST_REMOVE_FIRST
#define LLIST_REMOVE_FIRST_SAFELY   DLLIST_REMOVE_FIRST_SAFELY
#define LLIST_REMOVE_LAST           DLLIST_REMOVE_LAST
#define LLIST_REMOVE_LAST_SAFELY    DLLIST_REMOVE_LAST_SAFELY
//doubly only
//#define DLLIST_REMOVE_NODE(head, node)
//#define DLLIST_REMOVE_NODE_SAFELY(head, node)
#define LList_Head_t                DLList_Head_t
#define LList_Entry_t               DLList_Entry_t
#else
#define LLIST_NEXT                  SLLIST_NEXT
// imposible
//#define LLIST_PREV                  SLLIST_PREV
#define LLIST_HEAD                  SLLIST_HEAD
#define LLIST_TAIL                  SLLIST_TAIL
#define LLIST_TAIL_IS_VALID         SLLIST_TAIL_IS_VALID
#define LLIST_IS_EMPTY              SLLIST_IS_EMPTY
#define LLIST_IS_NOT_EMPTY          SLLIST_IS_NOT_EMPTY
#define LLIST_FIRST                 SLLIST_FIRST
#define LLIST_LAST                  SLLIST_LAST
#define LLIST_HEAD_INIT             SLLIST_HEAD_INIT
#define LLIST_HEAD_RESET            SLLIST_HEAD_RESET
#define LLIST_FOREACH               SLLIST_FOREACH
#define LLIST_WHILE_START           SLLIST_WHILE_START
#define LLIST_WHILE_NEXT            SLLIST_WHILE_NEXT
#define LLIST_INSERT_FIRST          SLLIST_INSERT_FIRST
#define LLIST_INSERT_LAST           SLLIST_INSERT_LAST
#define LLIST_INSERT_AFTER          SLLIST_INSERT_AFTER
#define LLIST_REMOVE_FIRST          SLLIST_REMOVE_FIRST
#define LLIST_REMOVE_FIRST_SAFELY   SLLIST_REMOVE_FIRST_SAFELY
//singly only
//#define SLLIST_REMOVE_NEXT(head, node)
//#define SLLIST_REMOVE_NEXT_SAFELY(head, node)
//TODO, needs to travel all
//#define LLIST_REMOVE_LAST           SLLIST_REMOVE_LAST
//#define LLIST_REMOVE_LAST_SAFELY    SLLIST_REMOVE_LAST_SAFELY
//#define LLIST_REMOVE_NODE           SLLIST_REMOVE_NODE
//#define LLIST_REMOVE_NODE_SAFELY    SLLIST_REMOVE_NODE_SAFELY
#define LList_Head_t                SLList_Head_t
#define LList_Entry_t               SLList_Entry_t
#endif
*/

void LibLinkedList_Demo(void);


#define __LIB_LINKED_LIST_HPP_INCLUDED_
#endif//__LIB_LINKED_LIST_HPP_INCLUDED_

