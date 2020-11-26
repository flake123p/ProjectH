
#define ENABLE_PC_DEBUG ( 1 )

#define ENABLE_HX3_OPTIMIZE ( 0 )

#define COMM_ID_REQUEST_0  900
#define COMM_ID_RESPONSE_0 901
#define COMM_ID_REQUEST_1  902
#define COMM_ID_RESPONSE_1 903
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
#else
#include <mpx.h>
typedef uint32_t u32;
typedef struct {
    u32 hi;
    u32 lo;
} LibU64_t;
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
    A_IS_BIGGER = 0,
    B_IS_BIGGER = 1,
    A_B_ARE_EQUAL = 2,
} LIB_U64_COMPARE_t;
LIB_U64_COMPARE_t LibU64_Compare(LibU64_t *a, LibU64_t *b)
{
    LIB_U64_COMPARE_t ret;
    if (a->hi > b->hi) {
        ret = A_IS_BIGGER;
    } else if (a->hi < b->hi) {
        ret = B_IS_BIGGER;
    } else {
        if (a->lo > b->lo) {
        ret = A_IS_BIGGER;
        } else if (a->lo < b->lo) {
            ret = B_IS_BIGGER;
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
#if ENABLE_HX3_OPTIMIZE
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
#if ENABLE_HX3_OPTIMIZE
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

#if ENABLE_HX3_OPTIMIZE
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

    if (ret == A_IS_BIGGER) {
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
        if (ret == A_IS_BIGGER)
            ret = B_IS_BIGGER;
        else if (ret == B_IS_BIGGER)
            ret = A_IS_BIGGER;
    }
    return ret;
}
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
#define RESERVED_0x2000            0x2000
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

struct timer_request_node {
    DLList_Entry_t entry;
    uint16_t comm_id;
    uint16_t curr_repeat_counts;
    struct time48_32x2 duration_in_list;
    struct time48_32x2 start_time;
    struct timer_request req;
};

#if ENABLE_PC_DEBUG
void dump_time16(struct time48_16x3 *t16)
{
    printf("hi:0x%04X, me:0x%04X, lo:0x%04X\n", t16->hi, t16->me, t16->lo);
}
void dump_time32(struct time48_32x2 *t32)
{
    struct time48_16x3 t16;
    t16.hi = (uint16_t)t32->hi;
    t16.me = (uint16_t)(t32->lo>>16);
    t16.lo = (uint16_t)t32->lo;
    dump_time16(&t16);
}
void dump_list1(const char *str, DLList_Head_t *pHead)
{
    //#define DLLIST_FOREACH(head,curr,type)
    struct timer_request_node *curr;

    printf("%s: ", str);

    DLLIST_FOREACH(pHead, curr, struct timer_request_node)
    {
        printf("comm_id=%d ->",
            curr->comm_id);
    }

    printf("\n");
}
void dump_list3(const char *str, DLList_Head_t *pHead)
{
    //#define DLLIST_FOREACH(head,curr,type)
    struct timer_request_node *curr;

    printf("%s:\n", str);

    DLLIST_FOREACH(pHead, curr, struct timer_request_node)
    {
        printf("command_flags=0x%04X, timer_id=%d, time_in_ms=%d, repeat_counts=%d(%d)\n",
            curr->req.command_flags,
            curr->req.timer_id,
            curr->req.time_in_ms,
            curr->req.repeat_counts,
            curr->curr_repeat_counts);
        printf("duration_in_list.hi=0x%08X, duration_in_list.lo=0x%08X, start_hi=0x%08X, start_lo=0x%08X\n\n",
            curr->duration_in_list.hi,
            curr->duration_in_list.lo,
            curr->start_time.hi,
            curr->start_time.lo);
    }
}
#else
#define dump_time16(...)
#define dump_time32(...)
#define dump_list1(...)
#define dump_list3(...)
#endif

#if ENABLE_PC_DEBUG
#define MAX_NUM_OF_TIMER_REQUEST (5)
#define MS_TO_TIMER_TICKS (0x80000000)
#else
#define MAX_NUM_OF_TIMER_REQUEST (128)
#define REPEAT_FUNC_COMPENSATE (0) //non linear
#define MS_TO_TIMER_TICKS (62500-REPEAT_FUNC_COMPENSATE)
#endif

struct timer_request_node g_nodes[MAX_NUM_OF_TIMER_REQUEST];
DLList_Head_t g_unused_list, g_used_list;
#define UNUSED_LIST (&g_unused_list)
#define USED_LIST (&g_used_list)
struct timer_request g_timer_request_buf;
struct timer_response g_timer_response_buf;
struct time48_32x2 g_pseudo_system_clock;
uint16_t g_comm_id;
int g_is_timer_exist;
int g_is_times_up; //sim, stub
int g_is_message_in; //sim, stub
int i;
struct time48_32x2 g_timesup_clock; //sim, stub
struct time48_32x2 g_elapsed_clock; //sim, stub
struct time48_32x2 g_elapsed_clock_hw = {0,0}; //sim, stub
struct time48_32x2 g_start_clock_complement; //sim, stub

void Timer48_Message_Out(uint16_t comm_id)
{
    //send out g_timer_response_buf
#if ENABLE_PC_DEBUG
    
#else
    switch (comm_id)
    {
        case COMM_ID_REQUEST_0:
            MPX_Send( &g_timer_response_buf, sizeof(struct timer_response)/sizeof(uint16_t), MPX_INT, COMM_ID_RESPONSE_0, MPX_DMA | MPX_NONBLOCKING );
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
    g_elapsed_clock_hw.hi = 0;
    g_elapsed_clock_hw.lo = 0;
    //dump_time32(&g_pseudo_system_clock);
}

void Timer48_Start(struct timer_request_node *node)
{
    //Timer48_Update_System_Clock();

    //int LibU48_Add48To48(LibU64_t *from, LibU64_t *increment, LibU64_t *result); //return 1 if u64 overflow
    LibU48_Add48To48((LibU64_t *)&g_pseudo_system_clock, (LibU64_t *)&(node->duration_in_list), (LibU64_t *)&g_timesup_clock);
    g_is_timer_exist = 1;

    //start HW timer
    g_start_clock_complement.hi = ~node->duration_in_list.hi;
    g_start_clock_complement.lo = ~node->duration_in_list.lo;
#if ENABLE_PC_DEBUG
#else
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

//#if ENABLE_HX3_OPTIMIZE
#if 0
    *result = temp.lo / MS_TO_TIMER_TICKS;
#else
    //int LibU48_Div32To48(LibU64_t *dividend64, u32 divisor32, LibU64_t *quotient, u32 *remainder)
    LibU48_Div32To48((LibU64_t *)&temp, MS_TO_TIMER_TICKS, (LibU64_t *)&quotient, &remainder);
    //DUMPNX32(quotient.hi);
    //DUMPNX32(quotient.lo);
#endif
    *result = (uint16_t)quotient.lo;
}

void Timer48_Delete(uint16_t timer_id_to_del, struct timer_request_node **p_deleted_timer)
{
    struct timer_request_node *curr, *next;
    *p_deleted_timer = NULL;
    DLLIST_FOREACH(USED_LIST, curr, struct timer_request_node)
    {
        if (curr->req.timer_id == timer_id_to_del)
        {
            if (curr == (struct timer_request_node *)DLLIST_HEAD(USED_LIST))
            {
                Timer48_Stop();
                next = (struct timer_request_node *)DLLIST_NEXT(curr);
                if (next != NULL)
                {
                    LibU48_Add48To48((LibU64_t *)&(next->duration_in_list), (LibU64_t *)&(curr->duration_in_list), (LibU64_t *)&(next->duration_in_list));
                    LibU48_Sub48To48((LibU64_t *)&(next->duration_in_list), (LibU64_t *)&g_elapsed_clock, (LibU64_t *)&(next->duration_in_list));
                    Timer48_Start(next);
                }
            }
            else
            {
                next = (struct timer_request_node *)DLLIST_NEXT(curr);
                if (next != NULL)
                {
                    LibU48_Add48To48((LibU64_t *)&(next->duration_in_list), (LibU64_t *)&(curr->duration_in_list), (LibU64_t *)&(next->duration_in_list));
                }
            }
            *p_deleted_timer = curr;
            //DLLIST_REMOVE_NODE(USED_LIST, curr);
            //DLLIST_INSERT_LAST(UNUSED_LIST, curr);
            //dump_list1("unused ", UNUSED_LIST);
            //dump_list3("used   ", USED_LIST);
            break;
        }
    }
}

// return does_timer_start
int Timer48_Insert(struct timer_request_node *new_node)
{
    int does_timer_start = 0;
    struct timer_request_node *curr, *next;
    struct time48_32x2 first_node_remain;
    LIB_U64_COMPARE_t cmp = A_B_ARE_EQUAL;

#if ENABLE_HX3_OPTIMIZE
    new_node->duration_in_list.hi = 0;
    new_node->duration_in_list.lo = new_node->req.time_in_ms * MS_TO_TIMER_TICKS;
#else
    //int LibU48_Mul32To48(u32 a, u32 x, LibU64_t *result);
    LibU48_Mul32To48(MS_TO_TIMER_TICKS, new_node->req.time_in_ms, (LibU64_t *)&(new_node->duration_in_list));
#endif

    curr = (struct timer_request_node *)DLLIST_HEAD(USED_LIST);

    if (curr == NULL)
    {
        DLLIST_INSERT_FIRST(USED_LIST, new_node);
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
        if (cmp == A_IS_BIGGER)
        {
            // new node is 1st node now, start timer as fast as possible
            Timer48_Stop();
            Timer48_Start(new_node);
            does_timer_start = 1;
            // insert new 1st, adjust duration of old 1st node
            // old 1st duration = remain - new 1st
            LibU48_Sub48To48((LibU64_t *)&first_node_remain, (LibU64_t *)&(new_node->duration_in_list), (LibU64_t *)&(curr->duration_in_list));
            DLLIST_INSERT_FIRST(USED_LIST, new_node);
        }
        else
        {
            //update new node duration & compare, leave HW timer alone now
            LibU48_Sub48To48((LibU64_t *)&(new_node->duration_in_list), (LibU64_t *)&first_node_remain, (LibU64_t *)&(new_node->duration_in_list));

            // search & insert
            do
            {
                next = (struct timer_request_node *)DLLIST_NEXT(curr);
                if (next == NULL)
                {
                    DLLIST_INSERT_AFTER(USED_LIST, curr, new_node);
                    break;
                }
                // compare "next node duration_in_list" to "new node duration"
                cmp = LibU64_Compare((LibU64_t *)&(next->duration_in_list), (LibU64_t *)&(new_node->duration_in_list));
                // if new is bigger, search next
                if (cmp == B_IS_BIGGER)
                {
                    LibU48_Sub48To48((LibU64_t *)&(new_node->duration_in_list), (LibU64_t *)&(next->duration_in_list), (LibU64_t *)&(new_node->duration_in_list));
                    curr = next;
                    //keep searching
                }
                // if next is bigger or equal, searching is over
                else
                {
                    DLLIST_INSERT_AFTER(USED_LIST, curr, new_node);
                    LibU48_Sub48To48((LibU64_t *)&(next->duration_in_list), (LibU64_t *)&(new_node->duration_in_list), (LibU64_t *)&(next->duration_in_list));
                    break;
                }
            } while (curr != NULL);
        }
    }

    //Timer48_Update_System_Clock();
    LibU48_Add48To48((LibU64_t *)&g_pseudo_system_clock, (LibU64_t *)&g_elapsed_clock, (LibU64_t *)&(new_node->start_time));

    return does_timer_start;
}

void Timer48_Timesup(void)
{
    int do_reinsert = 0;
    struct timer_request_node *first = (struct timer_request_node *)DLLIST_HEAD(USED_LIST);
    DLLIST_REMOVE_FIRST(USED_LIST);

    g_is_timer_exist = 0;
    g_is_times_up = 0;
    /*
        1.send back message
        2.infinite loop check
        3.finite loop check
        4.one-shot check and remove node to un-used list
    */
    // 1.send back message
    g_timer_response_buf.command_flags = first->req.command_flags;
    g_timer_response_buf.timer_id = first->req.timer_id;
    g_timer_response_buf.duration_in_ms = first->req.time_in_ms;
#if ENABLE_PC_DEBUG
    printf("time's up on timer_id:%d\n", first->req.timer_id);
#endif
    Timer48_Message_Out(first->comm_id);

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

    // 4.one-shot check and remove node to un-used list
#if 1
    // start timer if there are remainders
    struct timer_request_node *new_first_node = (struct timer_request_node *)DLLIST_HEAD(USED_LIST);
    if (new_first_node != NULL)
    {
        Timer48_Start(new_first_node);
    }
    if (do_reinsert)
    {
        Timer48_Insert(first);
    }
    else
    {
        DLLIST_INSERT_LAST(UNUSED_LIST, first);
    }
#else
    if (do_reinsert)
    {
        if(Timer48_Insert(first) == 0)
        {
            struct timer_request_node *new_first_node = (struct timer_request_node *)DLLIST_HEAD(USED_LIST);
            Timer48_Start(new_first_node);
        }
    }
    else
    {
        // start timer if there are remainders
        struct timer_request_node *new_first_node = (struct timer_request_node *)DLLIST_HEAD(USED_LIST);
        if (new_first_node != NULL)
        {
            Timer48_Start(new_first_node);
        }
        DLLIST_INSERT_LAST(UNUSED_LIST, first);
    }
#endif
}

void Timer48_Message_In(void)
{
    if (g_timer_request_buf.command_flags & ADD_TIMER)
    {
        struct timer_request_node *new_node;
        new_node = (struct timer_request_node *)DLLIST_HEAD(UNUSED_LIST);
        if (new_node == NULL)
        {
            g_timer_request_buf.command_flags = TIMER_MEMORY_RAN_OUT;
            //TBD??
        }
        DLLIST_REMOVE_FIRST(UNUSED_LIST);
        #if ENABLE_HX3_OPTIMIZE
        new_node->comm_id = COMM_ID_REQUEST_0;
        #else
        new_node->comm_id = g_comm_id;
        #endif
        new_node->curr_repeat_counts = 0;
        new_node->req = g_timer_request_buf;
        Timer48_Insert(new_node);
    }
    else if (g_timer_request_buf.command_flags & DEL_TIMER)
    {
        struct timer_request_node *deleted_timer;
        uint16_t dura = 0;
        Timer48_Delete(g_timer_request_buf.timer_id, &deleted_timer);
        if (deleted_timer != NULL)
        {
            DLLIST_REMOVE_NODE(USED_LIST, deleted_timer);
            if (deleted_timer->req.command_flags & RETURN_DURATION || g_timer_request_buf.command_flags & RETURN_DURATION)
            {
                Timer48_Get_Duration16(deleted_timer, &dura);
            }
            g_timer_response_buf.command_flags = g_timer_request_buf.command_flags;
            g_timer_response_buf.timer_id = deleted_timer->req.timer_id;
            g_timer_response_buf.duration_in_ms = dura;
#if ENABLE_PC_DEBUG
            printf("del timer_id:%d, and send back message, dura=%d\n", deleted_timer->req.timer_id, dura);
#endif
            Timer48_Message_Out(deleted_timer->comm_id);
            DLLIST_INSERT_LAST(UNUSED_LIST, deleted_timer);
        }
        else
        {
            //CAN_NOT_FIND_TIMER
        }
    }
#if ENABLE_HX3_OPTIMIZE
    //emtpy
#else
    else if (g_timer_request_buf.command_flags & RESTART_TIMER)
    {
        struct timer_request_node *deleted_timer;
        uint16_t dura = 0;
        Timer48_Delete(g_timer_request_buf.timer_id, &deleted_timer);
        if (deleted_timer != NULL)
        {
            DLLIST_REMOVE_NODE(USED_LIST, deleted_timer);
            if (deleted_timer->req.command_flags & RETURN_DURATION || g_timer_request_buf.command_flags & RETURN_DURATION)
            {
                Timer48_Get_Duration16(deleted_timer, &dura);
            }
            g_timer_response_buf.command_flags = g_timer_request_buf.command_flags;
            g_timer_response_buf.timer_id = deleted_timer->req.timer_id;
            g_timer_response_buf.duration_in_ms = dura;
#if ENABLE_PC_DEBUG
            printf("restart timer_id:%d, and send back message, dura=%d\n", deleted_timer->req.timer_id, dura);
#endif
            Timer48_Message_Out(deleted_timer->comm_id);
            //DLLIST_INSERT_LAST(UNUSED_LIST, deleted_timer);

            //re-insert
            deleted_timer->curr_repeat_counts = 0;
            Timer48_Insert(deleted_timer);
        }
        else
        {
            //CAN_NOT_FIND_TIMER
        }
    }
#endif
}

int Timer48_TestStub(int is_start)
{
#if ENABLE_PC_DEBUG
    static int timesup_flag;
    LibU64_t result_clock;
    LibU48_Add48To48((LibU64_t *)&g_pseudo_system_clock, (LibU64_t *)&g_elapsed_clock, (LibU64_t *)&result_clock);
    
    if (is_start)
    {
        timesup_flag = 0;
        printf("test stub ------ 0x%08X . 0x%08X /// 0x%08X . 0x%08X + 0x%08X . 0x%08X \n",
            result_clock.hi,
            result_clock.lo,
            g_pseudo_system_clock.hi,
            g_pseudo_system_clock.lo,
            g_elapsed_clock.hi,
            g_elapsed_clock.lo);

        {
            static LibU64_t a = {0, 0};
            if (A_B_ARE_EQUAL == LibU64_Compare(&a, (LibU64_t *)(&result_clock)))
            {
                g_timer_request_buf.command_flags = ADD_TIMER|REPEAT_FINITE;
                g_timer_request_buf.repeat_counts = 1;
                g_timer_request_buf.timer_id = 1;
                g_timer_request_buf.time_in_ms = 5;
                Timer48_Message_In();
                g_timer_request_buf.command_flags = ADD_TIMER|REPEAT_FINITE;
                g_timer_request_buf.repeat_counts = 2;
                g_timer_request_buf.timer_id = 2;
                g_timer_request_buf.time_in_ms = 3;
                Timer48_Message_In();
                g_timer_request_buf.command_flags = ADD_TIMER|REPEAT_FINITE;
                g_timer_request_buf.repeat_counts = 2;
                g_timer_request_buf.timer_id = 3;
                g_timer_request_buf.time_in_ms = 10;
                Timer48_Message_In();
                printf("initial list:\n");
                dump_list1("unused ", UNUSED_LIST);
                dump_list3("used   ", USED_LIST);
            }
        }

        {
            static LibU64_t a = {0, 0x80000000};
            if (A_B_ARE_EQUAL == LibU64_Compare(&a, (LibU64_t *)(&result_clock)))
            {
                g_is_message_in = 1;
                //add timer
                printf("add timer 4\n");
                g_timer_request_buf.command_flags = ADD_TIMER;
                g_timer_request_buf.repeat_counts = 0;
                g_timer_request_buf.timer_id = 4;
                g_timer_request_buf.time_in_ms = 1;
            }
        }

        {
            static LibU64_t a = {5, 0xa0000000};
            if (A_B_ARE_EQUAL == LibU64_Compare(&a, (LibU64_t *)(&result_clock)))
            {
                g_is_message_in = 1;
                //add timer
                printf("restart timer 3\n");
                g_timer_request_buf.command_flags = RESTART_TIMER | RETURN_DURATION;
                g_timer_request_buf.timer_id = 3;
            }
        }

        {
            struct timer_request_node *first = (struct timer_request_node *)DLLIST_FIRST(USED_LIST);
            if (first != NULL)
            {
                LIB_U64_COMPARE_t cmp = LibU64_Compare((LibU64_t *)&g_elapsed_clock_hw, (LibU64_t *)&(first->duration_in_list));
                //DUMPNX32(g_elapsed_clock_hw.hi);
                //DUMPNX32(g_elapsed_clock_hw.lo);
                //DUMPNX32(first->duration_in_list.hi);
                //DUMPNX32(first->duration_in_list.lo);
                if (cmp == A_B_ARE_EQUAL)
                {
                    g_is_times_up = 1;
                    timesup_flag = 1;
                }
            }
        }
    }
    else
    {
        //end
        {
            static LibU64_t a = {0, 0x80000000};
            if (A_B_ARE_EQUAL == LibU64_Compare(&a, (LibU64_t *)(&result_clock)))
            {
                printf("after add timer 4\n");
                dump_list1("unused ", UNUSED_LIST);
                dump_list3("used   ", USED_LIST);
            }
        }

        if (timesup_flag)
        {
            timesup_flag = 0;
            printf("after timer's up\n");
            dump_list1("unused ", UNUSED_LIST);
            dump_list3("used   ", USED_LIST);
        }

        //test over
        {
            static LibU64_t a = {16, 0};
            if (A_B_ARE_EQUAL == LibU64_Compare(&a, (LibU64_t *)(&result_clock)))
            {
                printf("test over\n");
                dump_list1("unused ", UNUSED_LIST);
                dump_list3("used   ", USED_LIST);
                return 1;
            }
        }

        {
            static LibU64_t slice = {0, 0x20000000};
            LibU48_Add48To48((LibU64_t *)&g_elapsed_clock_hw, (LibU64_t *)&slice, (LibU64_t *)&g_elapsed_clock_hw);
            

            //Timer48_Get_Elapsed(); only works when timer exist
            g_elapsed_clock = g_elapsed_clock_hw;

        }
    }
#endif
    return 0;
}

void Timer48_Init(void)
{
    g_is_timer_exist = 0;
    g_is_times_up = 0;
    g_is_message_in = 0;
    g_pseudo_system_clock.hi = 0;
    g_pseudo_system_clock.lo = 0;
    // timer nodes init
    DLLIST_HEAD_RESET(UNUSED_LIST);
    DLLIST_HEAD_RESET(USED_LIST);
    for(i = 0; i< MAX_NUM_OF_TIMER_REQUEST; i++)
    {
        DLLIST_INSERT_LAST(UNUSED_LIST, &(g_nodes[i]));
    }
}

void Timer48(void)
{
    Timer48_Init();

    while (1)
    {
#if ENABLE_PC_DEBUG
        Timer48_TestStub(1);
#endif
        // wait message or time's up event
        if (g_is_message_in)
        {
            Timer48_Get_Elapsed();
            //Timer48_Update_System_Clock();
            Timer48_Message_In();
            g_is_message_in = 0;
            //TODO: keep recieving
        }
        if (g_is_timer_exist)
        {
            if (g_is_times_up)
            {
                Timer48_Get_Elapsed();
                //Timer48_Update_System_Clock();
                Timer48_Timesup();
                //g_is_times_up = 0;
            }
        }
#if ENABLE_PC_DEBUG
        if (Timer48_TestStub(0))
        {
            break;
        }
#endif
    }
}

