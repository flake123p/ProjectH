
#define ENABLE_PC_DEBUG ( 1 )

#define USE_DOUBLY_LIST ( 0 )

#define ENABLE_2_CELL   ( 1 )

#if ENABLE_2_CELL
#define CONN_ID_2ND_CELL_REQ 990
#define CONN_ID_2ND_CELL_RES 991
#endif
#define CONN_ID_TIMER_REQ_00 900
#define CONN_ID_TIMER_RES_00 901
#define CONN_ID_TIMER_REQ_01 902
#define CONN_ID_TIMER_RES_01 903
#define CONN_ID_TIMER_REQ_02 904
#define CONN_ID_TIMER_RES_02 905
/*
    Hx3 = 500Mhz
    1 cycle = 8 system clock
    1 second = 500,000,000 / 8 = 62,500,000 cycles
    1 ms = 62,500 cycles
    1 us = 62.5 cycles

    Hx4 = 2000Mhz
    1 cycle = 8 system clock ???
    1 second = 2,000,000,000 / 8 = 250,000,000 cycles
    1 ms = 250,000 cycles
    1 us = 250 cycles
*/

#if ENABLE_PC_DEBUG
#include "Everything_App.hpp"
#define mpx_shared
#else
#include <mpx.h>
mpx_celldef timer {
	mpx_input timer_00_in;
	mpx_input timer_01_in;
	mpx_input timer_02_in;
	mpx_output timer_00_out;
	mpx_output timer_01_out;
	mpx_output timer_02_out;
};
typedef uint32_t u32;
typedef struct {
    u32 hi;
    u32 lo;
} LibU64_t;
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

typedef enum {
    A_IS_GREATER = 0x01,
    B_IS_GREATER = 0x02,
    A_B_ARE_EQUAL = 0x04,
} LIB_U64_COMPARE_t;
//GTE stands for Greater Than or Equal
#define A_IS_GTE (A_IS_GREATER|A_B_ARE_EQUAL)
#define B_IS_GTE (B_IS_GREATER|A_B_ARE_EQUAL)
LIB_U64_COMPARE_t LibU64_Compare(LibU64_t *a, LibU64_t *b)
{
    LIB_U64_COMPARE_t ret;
    if (a->hi > b->hi) {
        ret = A_IS_GREATER;
    } else if (a->hi < b->hi) {
        ret = B_IS_GREATER;
    } else {
        if (a->lo > b->lo) {
        ret = A_IS_GREATER;
        } else if (a->lo < b->lo) {
            ret = B_IS_GREATER;
        } else {
            ret = A_B_ARE_EQUAL;
        }
    }
    return ret;
}
int LibU48_Add48To48(LibU64_t *from, LibU64_t *increment, LibU64_t *result) //return 1 if u64 overflow
{
    u32 isCarryHappened = 0;
    u32 temp;
    temp = from->lo + increment->lo;

    if (temp < from->lo) {
        isCarryHappened = 1;
    }
    result->lo = temp;

    //from->hi &= 0x0000FFFF;
    //increment->hi &= 0x0000FFFF;
    result->hi = from->hi + increment->hi + isCarryHappened;
#if 0
    result->hi &= 0x0000FFFF;
    return 0;
#else
    if (result->hi & 0xFFFF0000) {
        result->hi &= 0x0000FFFF;
        return 1;
    }
    return 0;
#endif
}

int LibU48_Sub48To48(LibU64_t *from, LibU64_t *decrement, LibU64_t *result) //return 1 if u64 overflow
{
    int isCarryHappened = 0;

    if(from->lo < decrement->lo) {
        isCarryHappened = 1;
        result->lo = 0xFFFFFFFF - decrement->lo + from->lo + 1;
    } else {
        result->lo = from->lo - decrement->lo;
    }

    result->hi = (from->hi - isCarryHappened) - decrement->hi;
#if 0
    result->hi &= 0x0000FFFF;
    return 0;
#else
    if (result->hi & 0xFFFF0000) {
        result->hi &= 0x0000FFFF;
        return 1;
    }
    return 0;
#endif
}

int LibU48_Mul32To48(u32 a, u32 x, LibU64_t *result)
{
    uint32_t ah = a >> 16;
    uint32_t al = a & 0xFFFF;
    uint32_t xh = x >> 16;
    uint32_t xl = x & 0xFFFF;
    uint32_t alxl = al * xl;
    uint32_t alxh = al * xh;
    uint32_t ahxl = ah * xl;
    uint32_t ahxh = ah * xh;
    uint32_t temp;

    //calculte result[31:16] = temp[15:0], temp[31:16] is carry
    temp = (alxl>>16) + (alxh&0xFFFF) + (ahxl&0xFFFF);
    result->lo = (temp<<16)|(0xFFFF&alxl);
    //calculate result[63:32] = temp[31:0]
    temp = (temp>>16)/*carry*/ + (alxh>>16) + (ahxl>>16) + ahxh;
    result->hi = temp;

#if 0
    result->hi &= 0x0000FFFF;
    return 0;
#else
    if (result->hi & 0xFFFF0000) {
        result->hi &= 0x0000FFFF;
        return 1;
    }
    return 0;
#endif
}

int LibU48_Div32To48(LibU64_t *dividend64, u32 divisor32, LibU64_t *quotient, u32 *remainder)
{
    int i, carry;
    u32 q_47_16 = 0;
    u32 q_15_0 = 0;
    u32 temp = 0;
    u32 currDividend = dividend64->hi << 16;
    u32 divisorComp = ~divisor32;
    for (i = 0; i < 16; i++)
    {
        temp = (temp << 1) + ((currDividend & 0x80000000) ? 1 : 0);
        currDividend = currDividend << 1;

        if (temp >= divisor32)
        {
            temp -= divisor32;
            q_47_16 = (q_47_16 << 1) | 1;
        }
        else
        {
            q_47_16 = (q_47_16 << 1) | 0;
        }
    }
    currDividend = dividend64->lo;
    for (i = 0; i < 16; i++)
    {
        temp = (temp << 1) + ((currDividend & 0x80000000) ? 1 : 0);
        currDividend = currDividend << 1;

        if (temp >= divisor32)
        {
            temp -= divisor32;
            q_47_16 = (q_47_16 << 1) | 1;
        }
        else
        {
            q_47_16 = (q_47_16 << 1) | 0;
        }
    }
    for (i = 0; i < 16; i++)
    {
        if (temp & 0x80000000)
            carry = 1;
        else
            carry = 0;
        // update temp
        temp = (temp << 1) + ((currDividend & 0x80000000) ? 1 : 0);
        currDividend = currDividend << 1;
        if (carry)
        {
            temp += divisorComp + 1;
            q_15_0 = (q_15_0 << 1) | 1;
        }
        else
        {
            if (temp >= divisor32)
            {
                temp -= divisor32;
                q_15_0 = (q_15_0 << 1) | 1;
            }
            else
            {
                q_15_0 = (q_15_0 << 1) | 0;
            }
        }
    }

    *remainder = temp;
    quotient->hi = q_47_16 >> 16;
    quotient->lo = (q_47_16 << 16) | q_15_0;
    return 0;
}

LIB_U64_COMPARE_t LibU48_Diff48(LibU64_t *a, LibU64_t *b, LibU64_t *result, u32 turnaround_mask_63_32)
{
    LIB_U64_COMPARE_t ret = LibU64_Compare(a, b);

    if (ret == A_IS_GREATER) {
        LibU48_Sub48To48(a, b, result);
    } else {
        LibU48_Sub48To48(b, a, result);
    }

    // result of diff
    if (result->hi >= turnaround_mask_63_32)
    {
        // do revert
        result->hi = ~result->hi;
        result->lo = (~result->lo ) + 1;
        //result->lo += 1;
        if (result->lo == 0)
            result->hi += 1;
        result->hi = 0x0000FFFF & result->hi;
        if (ret == A_IS_GREATER)
            ret = B_IS_GREATER;
        else if (ret == B_IS_GREATER)
            ret = A_IS_GREATER;
    }
    return ret;
}
#define ADD48(from,increment,result) LibU48_Add48To48((LibU64_t *)from, (LibU64_t *)increment, (LibU64_t *)result)
#define SUB48(from,decrement,result) LibU48_Sub48To48((LibU64_t *)from, (LibU64_t *)decrement, (LibU64_t *)result)
#define MUL48S(a,x,result)           LibU48_Mul32To48(a, x, (LibU64_t *)result)
#define DIV48S(dividend64,divisor32,quotient,p_remainder) LibU48_Div32To48((LibU64_t *)dividend64, divisor32, (LibU64_t *)quotient, p_remainder)
#define DIFF48(a,b,result,mask)      LibU48_Diff48((LibU64_t *)a, (LibU64_t *)b, (LibU64_t *)result, mask)
#endif //ENABLE_PC_DEBUG


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



struct time48_16x3 {
    uint16_t hi;
    uint16_t me;
    uint16_t lo;
};

struct time48_32x2 {
    uint32_t hi;
    uint32_t lo;
};

// no flag for one shot timer
#define REPEAT_FINITE   0x0001
#define REPEAT_INFINITE 0x0002
// options
#define RETURN_DURATION 0x0010
//#define TIME_IN_US      0x0020
//#define NO_RETURN       0x0040
// commands & events
#define ADD_TIMER                  0x0100
#define ADD_TIMER_RETURN_DURATION  (ADD_TIMER|RETURN_DURATION)
#define DEL_TIMER                  0x0200
#define RESTART_TIMER              0x0400
#define RESERVED_0x0800            0x0800
#define RESERVED_0x1000            0x1000
#define TIMER_TIMES_UP             0x2000
#define CAN_NOT_FIND_TIMER         0x4000
#define TIMER_MEMORY_RAN_OUT       0x8000

struct timer_request {
    uint16_t command_flags;
    uint16_t timer_id;
    uint16_t time_in_ms;
    uint16_t repeat_counts;
};

struct timer_response {
    uint16_t command_flags;
    uint16_t timer_id;
    uint16_t duration_in_ms;
};

#define TIMER_FLAG_ADD_COMPENSATE           0x0001
#define TIMER_FLAG_REPEAT_COMPENSATE        0x0002

struct timer_request_node {
    LList_Entry_t entry;
    uint16_t comm_id;
    uint16_t curr_repeat_counts;
    uint16_t flags;
    struct time48_32x2 duration_in_list;
    struct time48_32x2 start_time;
    struct timer_request req;
#if ENABLE_PC_DEBUG
    LibU64_t dbg_end_time;
#endif
};

#define TIMER48_INSERT_FIRST  LLIST_INSERT_FIRST
#define TIMER48_INSERT_LAST   LLIST_INSERT_LAST

#if ENABLE_PC_DEBUG
#define MAX_NUM_OF_TIMER_REQUEST (8)
#define MS_TO_TIMER_TICKS (0x80000000)
int g_is_times_up; //sim, stub
int g_is_message_in; //sim, stub
u32 gMsToTimerTicks = MS_TO_TIMER_TICKS;
extern void dump_node(struct timer_request_node *node);
extern void dump_all(void);
extern void dump_curr_clk(void);
#else
#define MAX_NUM_OF_TIMER_REQUEST (128)
#define REPEAT_FUNC_COMPENSATE (0) //non linear
#define MS_TO_TIMER_TICKS (62500)
#endif

#define COMPENSATE_ADD    (135)
#define COMPENSATE_REPEAT (54)

LList_Head_t g_unused_list;
LList_Head_t g_used_list;
#define UNUSED_LIST (&g_unused_list)
#define USED_LIST (&g_used_list)
struct timer_request_node *g_deleted_timer;

struct timer_request_node g_nodes[MAX_NUM_OF_TIMER_REQUEST];
struct timer_request g_timer_request_buf;
struct timer_request g_timer_request_buf1;
struct timer_request g_timer_request_buf2;
struct timer_request g_timer_request_buf3;
struct timer_response g_timer_response_buf;
struct time48_32x2 g_pseudo_system_clock;
uint16_t g_comm_id;
int g_is_timer_exist;
//struct time48_32x2 g_timesup_clock; //sim, stub
struct time48_32x2 g_elapsed_clock; //sim, stub
struct time48_32x2 g_elapsed_clock_hw = {0,0}; //sim, stub
struct time48_32x2 g_start_clock_complement; //sim, stub

mpx_shared struct time48_32x2 g_to_2nd_a;
mpx_shared struct time48_32x2 g_to_2nd_b;
mpx_shared struct time48_32x2 g_to_2nd_c;
mpx_shared struct time48_32x2 g_to_2nd_ret;
#if ENABLE_2_CELL
#define CALL_2ND_CELL(request) \
{\
    uint16_t req = request;\
    MPX_Send( &req, 1, MPX_INT, CONN_ID_2ND_CELL_REQ, MPX_DMA | MPX_NONBLOCKING );\
    MPX_Recv( &req, 1, MPX_INT, CONN_ID_2ND_CELL_RES, MPX_DMA | MPX_NONBLOCKING );\
    while(1)\
    {\
        if (MPX_Rtest(CONN_ID_2ND_CELL_RES) == 0) {\
            break;\
        }\
    }\
}
#endif


void Timer48_Message_Out(uint16_t comm_id)
{
    //send out g_timer_response_buf
#if ENABLE_PC_DEBUG

#else
    switch (comm_id)
    {
        case CONN_ID_TIMER_REQ_00:
            while(MPX_Stest(timer_00_out) != 0){};
            MPX_Send( &g_timer_response_buf, sizeof(struct timer_response)/sizeof(uint16_t), MPX_INT, timer_00_out, MPX_DMA | MPX_NONBLOCKING );
            break;

        case CONN_ID_TIMER_REQ_01:
            while(MPX_Stest(timer_01_out) != 0){};
            MPX_Send( &g_timer_response_buf, sizeof(struct timer_response)/sizeof(uint16_t), MPX_INT, timer_01_out, MPX_DMA | MPX_NONBLOCKING );
            break;

        case CONN_ID_TIMER_REQ_02:
            while(MPX_Stest(timer_02_out) != 0){};
            MPX_Send( &g_timer_response_buf, sizeof(struct timer_response)/sizeof(uint16_t), MPX_INT, timer_02_out, MPX_DMA | MPX_NONBLOCKING );
            break;

        default:
            break;
    }
#endif
}

void Timer48_Get_Elapsed(void)
{
    if (g_is_timer_exist)
    {
#if ENABLE_PC_DEBUG
        g_elapsed_clock = g_elapsed_clock_hw;
#else
        struct time48_16x3 temp; //sim, stub
        MPX_Timer_Get_Elapsed_Count( &temp );
        g_elapsed_clock.hi = temp.hi;
        g_elapsed_clock.lo = ((uint32_t)temp.me)<<16 | temp.lo;
        g_elapsed_clock.hi -= g_start_clock_complement.hi;
        g_elapsed_clock.lo -= g_start_clock_complement.lo;

#endif
    }
}

void Timer48_Update_System_Clock(void)
{
    //int LibU48_Add48To48(LibU64_t *from, LibU64_t *increment, LibU64_t *result); //return 1 if u64 overflow
    LibU48_Add48To48((LibU64_t *)&g_pseudo_system_clock, (LibU64_t *)&g_elapsed_clock, (LibU64_t *)&g_pseudo_system_clock);
    g_elapsed_clock.hi = 0;
    g_elapsed_clock.lo = 0;
#if ENABLE_PC_DEBUG
    g_elapsed_clock_hw.hi = 0;
    g_elapsed_clock_hw.lo = 0;
#endif
    //dump_time32(&g_pseudo_system_clock);
}

void Timer48_Start(struct timer_request_node *node)
{
    //Timer48_Update_System_Clock();

    //int LibU48_Add48To48(LibU64_t *from, LibU64_t *increment, LibU64_t *result); //return 1 if u64 overflow
    //LibU48_Add48To48((LibU64_t *)&g_pseudo_system_clock, (LibU64_t *)&(node->duration_in_list), (LibU64_t *)&g_timesup_clock);
    g_is_timer_exist = 1;

    //start HW timer
    g_start_clock_complement.hi = ~node->duration_in_list.hi;
    g_start_clock_complement.lo = ~node->duration_in_list.lo;
#if ENABLE_PC_DEBUG
#else
    //struct time48_32x2 after_compensate;
    //SUB48(&(node->duration_in_list), &g_compensate, &after_compensate);
    MPX_Timer_Clear();
    MPX_Timer_Set_WakeMeUp();
    MPX_Timer_Set_PresetHi( (uint16_t)node->duration_in_list.hi );
    MPX_Timer_Set_PresetMed( (uint16_t)((node->duration_in_list.lo)>>16) );
    MPX_Timer_Set_PresetLo( (uint16_t)node->duration_in_list.lo );
    MPX_Timer_Start();
#endif
    Timer48_Update_System_Clock();
}

void Timer48_Stop(void)
{
    if (g_is_timer_exist)
    {
        //Timer48_Get_Elapsed();
        //Timer48_Update_System_Clock();
        g_is_timer_exist = 0;
#if ENABLE_PC_DEBUG
#else
        MPX_Timer_Disable();
#endif
    }
}

void Timer48_Get_Duration16_Cell_2nd(void)
{
    //struct time48_32x2 temp, quotient;
    u32 remainder;
    // temp = current system clock
    //LibU48_Add48To48((LibU64_t *)&g_pseudo_system_clock, (LibU64_t *)&g_elapsed_clock, (LibU64_t *)&temp);
    // temp = duration = current clock - start clock
    //node->start_time.hi = 0x00000000;
    //node->start_time.lo = 0x00000000;
    LibU48_Diff48((LibU64_t *)&g_to_2nd_a, (LibU64_t *)&g_to_2nd_b, (LibU64_t *)&g_to_2nd_a, 0x00008000);
    //DUMPNX32(temp.hi);
    //DUMPNX32(temp.lo);

#if 0
    g_to_2nd_ret.lo = g_to_2nd_a.lo / MS_TO_TIMER_TICKS;
#else
    #if ENABLE_PC_DEBUG
        DIV48S(&g_to_2nd_a, gMsToTimerTicks, &g_to_2nd_ret, &remainder);
    #else
        //LibU48_Div32To48((LibU64_t *)&temp, MS_TO_TIMER_TICKS, (LibU64_t *)&quotient, &remainder);
        DIV48S(&g_to_2nd_a, MS_TO_TIMER_TICKS, &g_to_2nd_ret, &remainder);
    #endif
#endif
    //*result = (uint16_t)quotient.lo;
}

/*
    a = current clock
    b = node start_time

    ret = quotient
*/
void Timer48_Get_Duration16_Cell_1st(struct timer_request_node *node, uint16_t *result)
{
    // temp = current system clock
    LibU48_Add48To48((LibU64_t *)&g_pseudo_system_clock, (LibU64_t *)&g_elapsed_clock, (LibU64_t *)&g_to_2nd_a);
    g_to_2nd_b = node->start_time;
#if ENABLE_PC_DEBUG
    Timer48_Get_Duration16_Cell_2nd();
#else
    CALL_2ND_CELL(1);
#endif
    *result = (uint16_t)g_to_2nd_ret.lo;
}

void Timer48_Get_Duration16(struct timer_request_node *node, uint16_t *result)
{
    struct time48_32x2 temp, quotient;
    u32 remainder;
    // temp = current system clock
    LibU48_Add48To48((LibU64_t *)&g_pseudo_system_clock, (LibU64_t *)&g_elapsed_clock, (LibU64_t *)&temp);
    // temp = duration = current clock - start clock
    //node->start_time.hi = 0x00000000;
    //node->start_time.lo = 0x00000000;
    LibU48_Diff48((LibU64_t *)&temp, (LibU64_t *)&(node->start_time), (LibU64_t *)&temp, 0x00008000);
    //DUMPNX32(temp.hi);
    //DUMPNX32(temp.lo);

#if 0
    *result = temp.lo / MS_TO_TIMER_TICKS;
#else
    #if ENABLE_PC_DEBUG
        DIV48S(&temp, gMsToTimerTicks, &quotient, &remainder);
    #else
        //LibU48_Div32To48((LibU64_t *)&temp, MS_TO_TIMER_TICKS, (LibU64_t *)&quotient, &remainder);
        DIV48S(&temp, MS_TO_TIMER_TICKS, &quotient, &remainder);
    #endif
#endif
    *result = (uint16_t)quotient.lo;
}

void Timer48_Delete(uint16_t timer_id_to_delete)
{
#if USE_DOUBLY_LIST
    struct timer_request_node *curr, *next;
#else
    struct timer_request_node *curr, *next, *prev;
    prev = (struct timer_request_node *)USED_LIST;
#endif

    curr = (struct timer_request_node *)LLIST_HEAD(USED_LIST);
    do
    {
        if (curr->req.timer_id == timer_id_to_delete)
        {
            if (curr == (struct timer_request_node *)LLIST_HEAD(USED_LIST))
            {
                Timer48_Stop();
                next = (struct timer_request_node *)LLIST_NEXT(curr);
                if (next != NULL)
                {
                    LibU48_Add48To48((LibU64_t *)&(next->duration_in_list), (LibU64_t *)&(curr->duration_in_list), (LibU64_t *)&(next->duration_in_list));
                    LibU48_Sub48To48((LibU64_t *)&(next->duration_in_list), (LibU64_t *)&g_elapsed_clock, (LibU64_t *)&(next->duration_in_list));
                    Timer48_Start(next);
                }
            }
            else
            {
                next = (struct timer_request_node *)LLIST_NEXT(curr);
                if (next != NULL)
                {
                    LibU48_Add48To48((LibU64_t *)&(next->duration_in_list), (LibU64_t *)&(curr->duration_in_list), (LibU64_t *)&(next->duration_in_list));
                }
            }
#if USE_DOUBLY_LIST
            DLLIST_REMOVE_NODE(USED_LIST, curr);
#else
            SLLIST_REMOVE_NEXT(USED_LIST, prev);
#endif
            g_deleted_timer = curr;

            //DLLIST_REMOVE_NODE(USED_LIST, curr);
            //DLLIST_INSERT_LAST(UNUSED_LIST, curr);
            //dump_list1("unused ", UNUSED_LIST);
            //dump_list3("used   ", USED_LIST);
            break;
        }
#if USE_DOUBLY_LIST
#else
        prev = curr;
#endif
        curr = (struct timer_request_node *)curr->entry.next;
    } while (curr != NULL);
}

void Timer48_Delete_And_Message_Out(uint16_t timer_id_to_delete)
{
    uint16_t dura = 0;
    Timer48_Delete(timer_id_to_delete);
    if (g_deleted_timer != NULL)
    {
        if ((g_deleted_timer->req.command_flags & RETURN_DURATION) || (g_timer_request_buf.command_flags & RETURN_DURATION))
        {
#if ENABLE_2_CELL
            Timer48_Get_Duration16_Cell_1st(g_deleted_timer, &dura);
#else
            Timer48_Get_Duration16(g_deleted_timer, &dura);
#endif
        }
        g_timer_response_buf.command_flags = g_timer_request_buf.command_flags;
    }
    else
    {
        g_timer_response_buf.command_flags = g_timer_request_buf.command_flags | CAN_NOT_FIND_TIMER;
    }
    g_timer_response_buf.timer_id = timer_id_to_delete;
    g_timer_response_buf.duration_in_ms = dura;

    Timer48_Message_Out(g_comm_id);
}

void Timer48_Insert_Cell_2nd(void)
{
    /*
        a.hi = new_node->req.time_in_ms
        a.lo = new_node->flags (ret)

        ret = new_node->duration_in_list
    */
#if 0
    new_node->duration_in_list.hi = 0;
    new_node->duration_in_list.lo = new_node->req.time_in_ms * MS_TO_TIMER_TICKS;
#else
    #if ENABLE_PC_DEBUG
    MUL48S(gMsToTimerTicks, g_to_2nd_a.hi, &(g_to_2nd_ret));
    #else
    MUL48S(MS_TO_TIMER_TICKS, g_to_2nd_a.hi, &(g_to_2nd_ret));
    if (g_to_2nd_a.lo & TIMER_FLAG_ADD_COMPENSATE)
    {
        struct time48_32x2 compensate;
        compensate.hi = 0;
        compensate.lo = COMPENSATE_ADD;
        SUB48(&(g_to_2nd_ret), &compensate, &(g_to_2nd_ret));

        g_to_2nd_a.lo &= (~TIMER_FLAG_ADD_COMPENSATE);
        g_to_2nd_a.lo |= TIMER_FLAG_REPEAT_COMPENSATE;
    }
    else if (g_to_2nd_a.lo & TIMER_FLAG_REPEAT_COMPENSATE)
    {
        struct time48_32x2 compensate;
        compensate.hi = 0;
        compensate.lo = COMPENSATE_REPEAT;
        SUB48(&(g_to_2nd_ret), &compensate, &(g_to_2nd_ret));
    }
    #endif // #if ENABLE_PC_DEBUG
#endif // #if 0 or 1
}

// return does_timer_start
int Timer48_Insert(struct timer_request_node *new_node)
{
    int does_timer_start = 0;
    struct timer_request_node *curr, *next;
    struct time48_32x2 first_node_remain;
    LIB_U64_COMPARE_t cmp = A_B_ARE_EQUAL;

    /*
        a.hi = new_node->req.time_in_ms
        a.lo = new_node->flags (ret)

        ret = new_node->duration_in_list
    */
#if ENABLE_2_CELL
    g_to_2nd_a.hi = new_node->req.time_in_ms;
    g_to_2nd_a.lo = new_node->flags;
    #if ENABLE_PC_DEBUG
    Timer48_Insert_Cell_2nd();
    #else
    CALL_2ND_CELL(2);
    #endif
    new_node->flags = (uint16_t)g_to_2nd_a.lo;
    new_node->duration_in_list = g_to_2nd_ret;
#else
#if 0
    new_node->duration_in_list.hi = 0;
    new_node->duration_in_list.lo = new_node->req.time_in_ms * MS_TO_TIMER_TICKS;
#else
    #if ENABLE_PC_DEBUG
    MUL48S(gMsToTimerTicks, new_node->req.time_in_ms, &(new_node->duration_in_list));
    #else
    MUL48S(MS_TO_TIMER_TICKS, new_node->req.time_in_ms, &(new_node->duration_in_list));
    if (new_node->flags & TIMER_FLAG_ADD_COMPENSATE)
    {
        struct time48_32x2 compensate;
        compensate.hi = 0;
        compensate.lo = COMPENSATE_ADD;
        SUB48(&(new_node->duration_in_list), &compensate, &(new_node->duration_in_list));

        new_node->flags &= (~TIMER_FLAG_ADD_COMPENSATE);
        new_node->flags |= TIMER_FLAG_REPEAT_COMPENSATE;
    }
    else if (new_node->flags & TIMER_FLAG_REPEAT_COMPENSATE)
    {
        struct time48_32x2 compensate;
        compensate.hi = 0;
        compensate.lo = COMPENSATE_REPEAT;
        SUB48(&(new_node->duration_in_list), &compensate, &(new_node->duration_in_list));
    }
    #endif // #if ENABLE_PC_DEBUG
#endif // #if 0 or 1
#endif // #if ENABLE_2_CELL

    curr = (struct timer_request_node *)LLIST_HEAD(USED_LIST);

    if (curr == NULL)
    {
        TIMER48_INSERT_FIRST(USED_LIST, new_node);
        Timer48_Start(new_node);
        does_timer_start = 1;
    }
    else
    {
        // get first_node_remain, first_node_remain now is elapsed time
        //LibU48_Diff48((LibU64_t *)&(curr->start_time), (LibU64_t *)&g_pseudo_system_clock, (LibU64_t *)&first_node_remain, 0x00008000);

        // first_node_remain = duration - elapsed
        LibU48_Sub48To48((LibU64_t *)&(curr->duration_in_list), (LibU64_t *)&g_elapsed_clock, (LibU64_t *)&first_node_remain);

        // compare "first node remain" to "new node duration"
        //LIB_U64_COMPARE_t LibU64_Compare(LibU64_t *a, LibU64_t *b)
        cmp = LibU64_Compare((LibU64_t *)&first_node_remain, (LibU64_t *)&(new_node->duration_in_list));
        if (cmp == A_IS_GREATER)
        {
            // new node is 1st node now, start timer as fast as possible
            Timer48_Stop();
            Timer48_Start(new_node);
            does_timer_start = 1;
            // insert new 1st, adjust duration of old 1st node
            // old 1st duration = remain - new 1st
            LibU48_Sub48To48((LibU64_t *)&first_node_remain, (LibU64_t *)&(new_node->duration_in_list), (LibU64_t *)&(curr->duration_in_list));
            TIMER48_INSERT_FIRST(USED_LIST, new_node);
        }
        else
        {
            //update new node duration & compare, leave HW timer alone now
            LibU48_Sub48To48((LibU64_t *)&(new_node->duration_in_list), (LibU64_t *)&first_node_remain, (LibU64_t *)&(new_node->duration_in_list));

            // search & insert
            do
            {
                next = (struct timer_request_node *)LLIST_NEXT(curr);
                if (next == NULL)
                {
                    LLIST_INSERT_AFTER(USED_LIST, curr, new_node);
                    break;
                }
                // compare "next node duration_in_list" to "new node duration"
                cmp = LibU64_Compare((LibU64_t *)&(next->duration_in_list), (LibU64_t *)&(new_node->duration_in_list));
                // if new is bigger, search next
                if (cmp & B_IS_GTE)
                {
                    LibU48_Sub48To48((LibU64_t *)&(new_node->duration_in_list), (LibU64_t *)&(next->duration_in_list), (LibU64_t *)&(new_node->duration_in_list));
                    curr = next;
                    //keep searching
                }
                // if next is bigger or equal, searching is over
                else
                {
                    LLIST_INSERT_AFTER(USED_LIST, curr, new_node);
                    LibU48_Sub48To48((LibU64_t *)&(next->duration_in_list), (LibU64_t *)&(new_node->duration_in_list), (LibU64_t *)&(next->duration_in_list));
                    break;
                }
            } while (curr != NULL);
        }
    }

    LibU48_Add48To48((LibU64_t *)&g_pseudo_system_clock, (LibU64_t *)&g_elapsed_clock, (LibU64_t *)&(new_node->start_time));
#if ENABLE_PC_DEBUG
    {
        LibU64_t total_time;
        MUL48S(gMsToTimerTicks, new_node->req.time_in_ms, &total_time);
        ADD48(&total_time, &(new_node->start_time), &(new_node->dbg_end_time));
    }
#endif
    return does_timer_start;
}

void Timer48_Timesup(void)
{
    int is_next_very_close;
    int do_reinsert;
    struct timer_request_node *first;

    Timer48_Get_Elapsed();

    do
    {
        is_next_very_close = 0;
        do_reinsert = 0;
        first = (struct timer_request_node *)LLIST_HEAD(USED_LIST);
        LLIST_REMOVE_FIRST(USED_LIST);

        /*
            1.send back message
            2.infinite loop check
            3.finite loop check
            4.one-shot check and remove node to un-used list
        */
        // 1.send back message
        g_timer_response_buf.command_flags = first->req.command_flags | TIMER_TIMES_UP;
        g_timer_response_buf.timer_id = first->req.timer_id;
        g_timer_response_buf.duration_in_ms = first->req.time_in_ms;
        Timer48_Message_Out(first->comm_id);

#if ENABLE_PC_DEBUG
        {
            if (NULL == first)
            {
                printf("Error : time's up with no node.\n");
                dump_all();
                BASIC_ASSERT(0);
            }
            printf("time's up on timer_id:%d\n", first->req.timer_id);
            g_is_timer_exist = 0;
            g_is_times_up = 0;
        }
        {
            LibU64_t curr_clk;
            ADD48(&g_pseudo_system_clock, &g_elapsed_clock, &curr_clk);
            //curr clock mush equals to dbg_end_time
            if (A_B_ARE_EQUAL != LibU64_Compare(&(first->dbg_end_time), &curr_clk))
            {
                printf("Error : end time not match\n");
                dump_curr_clk();
                dump_node(first);
                dump_all();
                BASIC_ASSERT(0);
            }
            {
                extern void Timer48_TimesupStub(struct timer_request_node *first, LibU64_t *p_curr_clk);
                Timer48_TimesupStub(first, &curr_clk);
            }
        }
#endif

        // 2.infinite loop check
        if (first->req.command_flags & REPEAT_INFINITE)
        {
            do_reinsert = 1;
        }

        // 3.finite loop check
        if (first->req.command_flags & REPEAT_FINITE)
        {
            first->curr_repeat_counts++;

            if (first->req.repeat_counts != 0)
            {
                if (first->req.repeat_counts != first->curr_repeat_counts)
                {
                    do_reinsert = 1;
                }
            }
        }

        // !!! may insert before start
        // 4.one-shot check and remove node to un-used list
        // start timer if there are remainders
        struct timer_request_node *new_first_node = (struct timer_request_node *)LLIST_HEAD(USED_LIST);
        if (new_first_node != NULL)
        {
            // 0 compensate for now
            if (new_first_node->duration_in_list.hi | new_first_node->duration_in_list.lo)
            {
                Timer48_Start(new_first_node);
            }
            else
            {
                is_next_very_close = 1;
            }
        }

        if (do_reinsert)
        {
            //must update, a new start
            Timer48_Update_System_Clock();

            Timer48_Insert(first);
            //printf("re insert ...\n");
            //dump_all();
        }
        else
        {
            TIMER48_INSERT_LAST(UNUSED_LIST, first);
        }
    } while(is_next_very_close);
}

void Timer48_Message_In_Delete_Timer(void)
{
    int do_restart = g_timer_request_buf.command_flags & RESTART_TIMER;

    g_deleted_timer = NULL;
    Timer48_Delete_And_Message_Out(g_timer_request_buf.timer_id);
    if (g_deleted_timer != NULL)
    {
#if ENABLE_PC_DEBUG
        if (do_restart)
            printf("restart timer_id:%d, and send back message, dura=%d\n", g_deleted_timer->req.timer_id, g_timer_response_buf.duration_in_ms);
        else
            printf("del timer_id:%d, and send back message, dura=%d\n", g_deleted_timer->req.timer_id, g_timer_response_buf.duration_in_ms);
#endif
        if (do_restart)
        {
            //re-insert
            g_deleted_timer->curr_repeat_counts = 0;

            Timer48_Insert(g_deleted_timer);
        }
        else
        {
            TIMER48_INSERT_LAST(UNUSED_LIST, g_deleted_timer);
        }
    }
    else
    {
        //CAN_NOT_FIND_TIMER
    }
}

/*
void Timer48_Message_In_Delete_Timer_2nd_Cell(void)
{
#if ENABLE_PC_DEBUG
    Timer48_Message_In_Delete_Timer();
#else
    //MPX_Print("[Task:%T Cycle:%Y] aaa USED_LIST  = %d", (uint16_t)USED_LIST);
    uint16_t req = 0;
    MPX_Send( &req, 1, MPX_INT, CONN_ID_2ND_CELL_REQ, MPX_DMA | MPX_NONBLOCKING );
    MPX_Recv( &req, 1, MPX_INT, CONN_ID_2ND_CELL_RES, MPX_DMA | MPX_NONBLOCKING );
    while(1)
    {
    	if (MPX_Rtest(CONN_ID_2ND_CELL_RES) == 0) {
    		break;
    	}
    }
#endif
}
*/

void Timer48_Message_In_Add_Timer(void)
{
    struct timer_request_node *new_node;
    new_node = (struct timer_request_node *)LLIST_HEAD(UNUSED_LIST);
    if (new_node == NULL)
    {
        //g_timer_request_buf.command_flags = TIMER_MEMORY_RAN_OUT;
        //TBD??
    }
    LLIST_REMOVE_FIRST(UNUSED_LIST);
    new_node->comm_id = g_comm_id;
    new_node->curr_repeat_counts = 0;
    new_node->req = g_timer_request_buf;

    new_node->flags = TIMER_FLAG_ADD_COMPENSATE;
    Timer48_Insert(new_node);
    //g_compensate.lo = 0;
}

void Timer48_Message_In(void)
{
    Timer48_Get_Elapsed();

    if (g_timer_request_buf.command_flags & ADD_TIMER)
    {
        Timer48_Message_In_Add_Timer();
    }
    else if (g_timer_request_buf.command_flags & (DEL_TIMER | RESTART_TIMER))
    {
#if 0//ENABLE_2_CELL
        g_2nd_cell_act = 0;
        Timer48_Message_In_Delete_Timer_2nd_Cell();

        Timer48_Message_Out(g_comm_id);
        if (g_2nd_cell_act == 1)
        {
            Timer48_Insert(g_deleted_timer);
        }
#else
        Timer48_Message_In_Delete_Timer();
#endif
    }
}

void Timer48_Init(void)
{
    int i;
    g_is_timer_exist = 0;
#if ENABLE_PC_DEBUG
    g_is_times_up = 0;
    g_is_message_in = 0;
    g_elapsed_clock.hi = 0;
    g_elapsed_clock.lo = 0;
    g_elapsed_clock_hw.hi = 0;
    g_elapsed_clock_hw.lo = 0;
#endif
    g_pseudo_system_clock.hi = 0;
    g_pseudo_system_clock.lo = 0;
    // timer nodes init
    LLIST_HEAD_RESET(UNUSED_LIST);
    LLIST_HEAD_RESET(USED_LIST);
    for(i = 0; i< MAX_NUM_OF_TIMER_REQUEST; i++)
    {
        TIMER48_INSERT_LAST(UNUSED_LIST, &(g_nodes[i]));
    }
}

#if ENABLE_PC_DEBUG
void Timer48(void)
{
    int ret;

    while (1)
    {
        Timer48_Init();

        while (1)
        {
            Timer48_TestStub(1);
            // wait message or time's up event
            if (g_is_message_in)
            {
                //Timer48_Get_Elapsed();
                //Timer48_Update_System_Clock();
                Timer48_Message_In();
                g_is_message_in = 0;
                //TODO: keep recieving
            }
            if (g_is_timer_exist)
            {
                if (g_is_times_up)
                {
                    //Timer48_Get_Elapsed();
                    //Timer48_Update_System_Clock();
                    Timer48_Timesup();
                    //g_is_times_up = 0;
                }
            }
            ret = Timer48_TestStub(0);
            if (ret == 1)
            {
                printf("reset test environment ...\n\n");
                break; //reset test case
            }
            else if (ret == 2)
            {
                printf("all tests done ...\n\n");
                return;
            }
        }
    }
}
#endif

#if ENABLE_PC_DEBUG
//...
#else

void Timer48_MPX_Cell(void)
{
	int recheck = 1;
    Timer48_Init();

    MPX_SetupWake(timer_00_in);
    MPX_Recv( &g_timer_request_buf1, sizeof(struct timer_request)/sizeof(uint16_t), MPX_INT, timer_00_in, MPX_DONT_RESET_WRR | MPX_NONBLOCKING );
    MPX_SetupWake(timer_01_in);
    MPX_Recv( &g_timer_request_buf2, sizeof(struct timer_request)/sizeof(uint16_t), MPX_INT, timer_01_in, MPX_DONT_RESET_WRR | MPX_NONBLOCKING );
    MPX_SetupWake(timer_02_in);
    MPX_Recv( &g_timer_request_buf3, sizeof(struct timer_request)/sizeof(uint16_t), MPX_INT, timer_02_in, MPX_DONT_RESET_WRR | MPX_NONBLOCKING );

    while ( 1 )
    {
        MPX_Wait();
        do
        {
			if (MPX_Rtest(timer_00_in) == 0) {
				//MPX_Print("---S--- RX done : %T %Y, buf[0]=%d", buf[0]);
				MPX_ResetWMR(timer_00_in);
				g_comm_id = CONN_ID_TIMER_REQ_00;
				//Timer48_Get_Elapsed();
				g_timer_request_buf = g_timer_request_buf1;
				Timer48_Message_In();
				MPX_SetupWake(timer_00_in);
				MPX_Recv( &g_timer_request_buf1, sizeof(struct timer_request)/sizeof(uint16_t), MPX_INT, timer_00_in, MPX_DONT_RESET_WRR | MPX_NONBLOCKING );
				//MPX_Print("---E--- RX done : %T %Y, buf[1]=%d", buf[1]);
				recheck = 2;
			}
			if (MPX_Rtest(timer_01_in) == 0) {
				//MPX_Print("---S--- RX done : %T %Y, buf[0]=%d", buf[0]);
				MPX_ResetWMR(timer_01_in);
				g_comm_id = CONN_ID_TIMER_REQ_01;
				//Timer48_Get_Elapsed();
				g_timer_request_buf = g_timer_request_buf2;
				Timer48_Message_In();
				MPX_SetupWake(timer_01_in);
				MPX_Recv( &g_timer_request_buf2, sizeof(struct timer_request)/sizeof(uint16_t), MPX_INT, timer_01_in, MPX_DONT_RESET_WRR | MPX_NONBLOCKING );
				//MPX_Print("---E--- RX done : %T %Y, buf[1]=%d", buf[1]);
				recheck = 2;
			}
			if (MPX_Rtest(timer_02_in) == 0) {
				//MPX_Print("---S--- RX done : %T %Y, buf[0]=%d", buf[0]);
				MPX_ResetWMR(timer_02_in);
				g_comm_id = CONN_ID_TIMER_REQ_02;
				//Timer48_Get_Elapsed();
				g_timer_request_buf = g_timer_request_buf3;
				Timer48_Message_In();
				MPX_SetupWake(timer_02_in);
				MPX_Recv( &g_timer_request_buf3, sizeof(struct timer_request)/sizeof(uint16_t), MPX_INT, timer_02_in, MPX_DONT_RESET_WRR | MPX_NONBLOCKING );
				//MPX_Print("---E--- RX done : %T %Y, buf[1]=%d", buf[1]);
				recheck = 2;
			}
			if (g_is_timer_exist)
			{
				if (MPX_Timer_Get_Status() == 1) {
					//MPX_Print("---S--- Times up : %T %Y");
					MPX_Timer_Clear_Status();
					//MPX_Print("---E--- Times up : %T %Y");
					//Timer48_Get_Elapsed();
					Timer48_Timesup();
					recheck = 2;
				}
			}
			if (recheck)
				recheck -= 1;
        } while(recheck);
    }
}

/*
int main()
{
    if (MPX_RANK == 0)
    {
        Timer48_MPX_Cell();
    }

    if (MPX_RANK == 1)
    {
        struct timer_request request;
        struct timer_response response;
        volatile int i = 0;
        request.command_flags = ADD_TIMER|REPEAT_INFINITE;
        request.repeat_counts = 2;
        request.timer_id = 1111;
        request.time_in_ms = 1;
        MPX_Print("---S---1 Times up : %T %Y");
        MPX_Send( &request, sizeof(struct timer_request)/sizeof(uint16_t), MPX_INT, CONN_ID_TIMER_REQ_00, MPX_DMA); //MPX_NONBLOCKING
        //MPX_Print("---S---2 Times up : %T %Y");
        while (1)
        {
            MPX_Recv( &response, sizeof(struct timer_response)/sizeof(uint16_t), MPX_INT, CONN_ID_TIMER_RES_00, MPX_DMA);
            MPX_Print("---E---  Times up : %T %Y");
            //MPX_Recv( &response, sizeof(struct timer_response)/sizeof(uint16_t), MPX_INT, CONN_ID_TIMER_RES_00, MPX_DMA | MPX_NONBLOCKING);
            MPX_Print("command = %d", response.command_flags);
            MPX_Print("id = %d", response.timer_id);
            MPX_Print("dura = %d", response.duration_in_ms);
        }
    }

    if (MPX_RANK == 2)
    {
        struct timer_request request2;
        struct timer_response response2;
        volatile int i = 0;
        request2.command_flags = ADD_TIMER|REPEAT_INFINITE;
        request2.repeat_counts = 2;
        request2.timer_id = 2222;
        request2.time_in_ms = 2;
        while(i != 300){i++;i=i;}i=0;
        MPX_Print("---S---1 Times up : %T %Y");
        MPX_Send( &request2, sizeof(struct timer_request)/sizeof(uint16_t), MPX_INT, CONN_ID_TIMER_REQ_01, MPX_DMA); //MPX_NONBLOCKING
        //MPX_Print("---S---2 Times up : %T %Y");
        while (1)
        {
            MPX_Recv( &response2, sizeof(struct timer_response)/sizeof(uint16_t), MPX_INT, CONN_ID_TIMER_RES_01, MPX_DMA);
            MPX_Print("---E---  Times up : %T %Y");
            //MPX_Recv( &response, sizeof(struct timer_response)/sizeof(uint16_t), MPX_INT, CONN_ID_TIMER_RES_00, MPX_DMA | MPX_NONBLOCKING);
            MPX_Print("command = %d", response2.command_flags);
            MPX_Print("id = %d", response2.timer_id);
            MPX_Print("dura = %d", response2.duration_in_ms);
        }
    }

    if (MPX_RANK == 3)
    {
        struct timer_request request3;
        struct timer_response response3;
        volatile int i = 0;
        request3.command_flags = ADD_TIMER|REPEAT_INFINITE;
        request3.repeat_counts = 2;
        request3.timer_id = 3333;
        request3.time_in_ms = 3;
        while(i != 600){i++;i=i;}i=0;
        MPX_Print("---S---1 Times up : %T %Y");
        MPX_Send( &request3, sizeof(struct timer_request)/sizeof(uint16_t), MPX_INT, CONN_ID_TIMER_REQ_02, MPX_DMA); //MPX_NONBLOCKING
        //MPX_Print("---S---2 Times up : %T %Y");
        while (1)
        {
            MPX_Recv( &response3, sizeof(struct timer_response)/sizeof(uint16_t), MPX_INT, CONN_ID_TIMER_RES_02, MPX_DMA);
            MPX_Print("---E---  Times up : %T %Y");
            //MPX_Recv( &response, sizeof(struct timer_response)/sizeof(uint16_t), MPX_INT, CONN_ID_TIMER_RES_00, MPX_DMA | MPX_NONBLOCKING);
            MPX_Print("command = %d", response3.command_flags);
            MPX_Print("id = %d", response3.timer_id);
            MPX_Print("dura = %d", response3.duration_in_ms);
        }
    }

    if (MPX_RANK == 4)
    {
#if ENABLE_2_CELL
        uint16_t req = 0;
        while (1)
        {
            MPX_Recv( &req, 1, MPX_INT, CONN_ID_2ND_CELL_REQ, MPX_DMA );
            Timer48_Message_In_Delete_Timer();
            MPX_Send( &req, 1, MPX_INT, CONN_ID_2ND_CELL_RES, MPX_DMA );
        }
#endif
    }

    return 0;
}
*/

mpx_view v()
{
    //TODO Auto-generated cell view: timer
    if (MPX_RANK == 0)
    {
        Timer48_MPX_Cell();
    }
#if ENABLE_2_CELL
    else if (MPX_RANK == 1)
    {
        uint16_t req = 0;
        while (1)
        {
            MPX_Recv( &req, 1, MPX_INT, CONN_ID_2ND_CELL_REQ, MPX_DMA );
            if (req == 1) {
                Timer48_Get_Duration16_Cell_2nd();
            } else if (req == 2) {
                Timer48_Insert_Cell_2nd();
            }
            MPX_Send( &req, 1, MPX_INT, CONN_ID_2ND_CELL_RES, MPX_DMA );
        }
    }
#endif
}
#endif //#if ENABLE_PC_DEBUG


