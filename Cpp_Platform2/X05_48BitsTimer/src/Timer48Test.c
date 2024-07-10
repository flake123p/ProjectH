
#include "Everything_App.hpp"

#define ENABLE_PC_DEBUG ( 1 )

#define USE_DOUBLY_LIST ( 0 )

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

typedef void (*TimesupCB_t)(struct timer_request_node *first, LibU64_t *p_curr_clk);

#define DEFAULT_SIM_TIME_SLICE 0x20000000
#define DUMP_TEST_OVER() printf("\n========= >>>>>> %s() over ... <<<<<< =========\n", __func__)
#define DUMP_TEST_START() printf("\n========= >>>>>> %s() start ... <<<<<< =========\n", __func__)
u32 gEnableMessageInDump = 0;
u32 gEnableTimesUpDump = 0;
u32 gEnableCurrClkDump = 1;
u32 gSimTimeSlice = DEFAULT_SIM_TIME_SLICE;
extern u32 gMsToTimerTicks;
TimesupCB_t gTest_TimesupCB = NULL;

//extern struct timer_request_node g_nodes[];
extern LList_Head_t g_unused_list, g_used_list;
#define UNUSED_LIST (&g_unused_list)
#define USED_LIST (&g_used_list)
extern struct timer_request g_timer_request_buf;
extern struct timer_response g_timer_response_buf;
extern struct time48_32x2 g_pseudo_system_clock;
extern uint16_t g_comm_id;
extern int g_is_timer_exist;
extern int g_is_times_up; //sim, stub
extern int g_is_message_in; //sim, stub
//extern int i;
extern struct time48_32x2 g_timesup_clock; //sim, stub
extern struct time48_32x2 g_elapsed_clock; //sim, stub
extern struct time48_32x2 g_elapsed_clock_hw; //sim, stub
extern struct time48_32x2 g_start_clock_complement; //sim, stub
extern void Timer48_Get_Elapsed(void);
extern void Timer48_Message_In(void);
extern void dump_time16(struct time48_16x3 *t16);
extern void dump_time32(struct time48_32x2 *t32);
extern void dump_list1(const char *str, LList_Head_t *pHead);
extern void dump_list3(const char *str, LList_Head_t *pHead);

#if 1//ENABLE_PC_DEBUG
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
void dump_list1(const char *str, LList_Head_t *pHead)
{
    //#define DLLIST_FOREACH(head,curr,type)
    struct timer_request_node *curr;

    printf("%s: ", str);

    LLIST_FOREACH(pHead, curr, struct timer_request_node)
    {
        printf("comm_id=%d ->",
            curr->comm_id);
    }

    printf("\n");
}
void dump_list3(const char *str, LList_Head_t *pHead)
{
    //#define DLLIST_FOREACH(head,curr,type)
    struct timer_request_node *curr;

    printf("%s:\n", str);

    LLIST_FOREACH(pHead, curr, struct timer_request_node)
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

void Timer48_Message_In_Dump(void)
{
    const char *cmdCtr;
    if (g_timer_request_buf.command_flags & ADD_TIMER)
    {
        cmdCtr = "ADD_TIMER";
    }
    else if (g_timer_request_buf.command_flags & DEL_TIMER)
    {
        cmdCtr = "DEL_TIMER";
    }
    else if (g_timer_request_buf.command_flags & RESTART_TIMER)
    {
        cmdCtr = "RESTART_TIMER";
    }
    if (gEnableMessageInDump) {
        printf("msg in --S--, cmd:%s (0x%04X), get du:%d, id=%d, re=%d\n",
            cmdCtr,
            g_timer_request_buf.command_flags,
            g_timer_request_buf.command_flags&RETURN_DURATION,
            g_timer_request_buf.timer_id,
            g_timer_request_buf.repeat_counts);
        dump_list3("used   ", USED_LIST);
    }
    Timer48_Message_In();
    if (gEnableMessageInDump) {
        printf("msg in --E--\n");
        dump_list3("used   ", USED_LIST);
    }
}

void dump_curr_clk(void)
{
    LibU64_t curr_clk;
    ADD48(&g_pseudo_system_clock, &g_elapsed_clock, &curr_clk);
    DUMPNX_LIBU64(curr_clk);
}

void dump_node(struct timer_request_node *node)
{
    DUMPND(node->comm_id);
    DUMPND(node->curr_repeat_counts);
    DUMPNX_LIBU64(node->duration_in_list);
    DUMPNX_LIBU64(node->start_time);
    DUMPNX_LIBU64(node->dbg_end_time);
    DUMPNX(node->req.command_flags);
    DUMPND(node->req.timer_id);
    DUMPND(node->req.time_in_ms);
    DUMPND(node->req.repeat_counts);
}

void dump_all(void)
{
    dump_list1("unused ", UNUSED_LIST);
    dump_list3("used   ", USED_LIST);
}

void dump_curr_clk_with_log(LibU64_t *p_result_clock, int do_print)
{
    LibU64_t result_clock;
    LibU48_Add48To48((LibU64_t *)&g_pseudo_system_clock, (LibU64_t *)&g_elapsed_clock, (LibU64_t *)&result_clock);

    if (do_print)
    {
        printf("test stub ------ 0x%08X . 0x%08X /// 0x%08X . 0x%08X + 0x%08X . 0x%08X \n",
            result_clock.hi,
            result_clock.lo,
            g_pseudo_system_clock.hi,
            g_pseudo_system_clock.lo,
            g_elapsed_clock.hi,
            g_elapsed_clock.lo);
    }

    if(p_result_clock != NULL)
    {
        *p_result_clock = result_clock;
    }
}

static int gIsThisRoundTimesUp;

void TestCase1_AddTimer_Test_S_RegisterTimers(void)
{
    // 1. add empty 5
    // 2. add in the front 2
    // 3. add in the middle 3
    // 4. add in the last 9
    g_timer_request_buf.command_flags = ADD_TIMER;
    g_timer_request_buf.repeat_counts = 1;
    g_timer_request_buf.timer_id = 5555;
    g_timer_request_buf.time_in_ms = 5;
    Timer48_Message_In_Dump();
    g_timer_request_buf.command_flags = ADD_TIMER;
    g_timer_request_buf.repeat_counts = 2;
    g_timer_request_buf.timer_id = 2222;
    g_timer_request_buf.time_in_ms = 2;
    Timer48_Message_In_Dump();
    g_timer_request_buf.command_flags = ADD_TIMER;
    g_timer_request_buf.repeat_counts = 2;
    g_timer_request_buf.timer_id = 3333;
    g_timer_request_buf.time_in_ms = 3;
    Timer48_Message_In_Dump();
    g_timer_request_buf.command_flags = ADD_TIMER;
    g_timer_request_buf.repeat_counts = 2;
    g_timer_request_buf.timer_id = 9999;
    g_timer_request_buf.time_in_ms = 9;
    Timer48_Message_In_Dump();
}

int TestCase1_AddTimer_Test_S_VerifyOneByOne(int *staticVerifyCtr)
{
    if (gIsThisRoundTimesUp)
    {
        struct timer_request_node *first = (struct timer_request_node *)LLIST_FIRST(USED_LIST);
        switch (*staticVerifyCtr) {
            case 0:
                BASIC_ASSERT(first->req.timer_id == 2222);
                //BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(1, 0, curr_clk));
                //dump_list1("unused ", UNUSED_LIST);
                //dump_list3("used   ", USED_LIST);
                //PRINT_LINE;
                break;
            case 1:
                BASIC_ASSERT(first->req.timer_id == 3333);
                //BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(1, 0x80000000, curr_clk));
                //dump_list1("unused ", UNUSED_LIST);
                //dump_list3("used   ", USED_LIST);
                //PRINT_LINE;
                break;
            case 2:
                BASIC_ASSERT(first->req.timer_id == 5555);
                //BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(2, 0x80000000, curr_clk));
                //dump_list1("unused ", UNUSED_LIST);
                //dump_list3("used   ", USED_LIST);
                //PRINT_LINE;
                break;
            case 3:
                BASIC_ASSERT(first->req.timer_id == 9999);
                //BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(4, 0x80000000, curr_clk));
                //dump_list1("unused ", UNUSED_LIST);
                //dump_list3("used   ", USED_LIST);
                //PRINT_LINE;
                break;
            default:
                BASIC_ASSERT(0);
                break;
        }
        *staticVerifyCtr = *staticVerifyCtr + 1;
    }
    return gIsThisRoundTimesUp;
}

int TestCase1_AddTimer_Test(int is_begin, LibU64_t *curr_clk)
{
    int ret;
    static int verifyCtr = 0;
    if (is_begin)
    {
        if (A_B_ARE_EQUAL == LibU64_Compare2(0, 0, curr_clk))
        {
            DUMP_TEST_START();
            TestCase1_AddTimer_Test_S_RegisterTimers();
            printf("initial list:\n");
            dump_list1("unused ", UNUSED_LIST);
            dump_list3("used   ", USED_LIST);
        }

        ret = TestCase1_AddTimer_Test_S_VerifyOneByOne(&verifyCtr);
        if (ret)
        {
            switch (verifyCtr) {
                case 1:
                    //BASIC_ASSERT(first->req.timer_id == 2222);
                    BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(1, 0, curr_clk));
                    //dump_list1("unused ", UNUSED_LIST);
                    //dump_list3("used   ", USED_LIST);
                    //PRINT_LINE;
                    break;
                case 2:
                    //BASIC_ASSERT(first->req.timer_id == 3333);
                    BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(1, 0x80000000, curr_clk));
                    //dump_list1("unused ", UNUSED_LIST);
                    //dump_list3("used   ", USED_LIST);
                    //PRINT_LINE;
                    break;
                case 3:
                    //BASIC_ASSERT(first->req.timer_id == 5555);
                    BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(2, 0x80000000, curr_clk));
                    //dump_list1("unused ", UNUSED_LIST);
                    //dump_list3("used   ", USED_LIST);
                    //PRINT_LINE;
                    break;
                case 4:
                    //BASIC_ASSERT(first->req.timer_id == 9999);
                    BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(4, 0x80000000, curr_clk));
                    //dump_list1("unused ", UNUSED_LIST);
                    //dump_list3("used   ", USED_LIST);
                    //PRINT_LINE;
                    break;
                default:
                    BASIC_ASSERT(0);
                    break;
            }
        }
    }
    else
    {
        //test end

        //test over
        {
            LIB_U64_COMPARE_t cmp = LibU64_Compare2(5, 0, (LibU64_t *)curr_clk);
            if (cmp & B_IS_GTE)
            {
                DUMP_TEST_OVER();
                if(verifyCtr != 4)
                { 
                    DUMPNX(cmp);
                    DUMPND(verifyCtr);
                    DUMPNX_LIBU64(*curr_clk);
                    dump_list3("used   ", USED_LIST);
                    BASIC_ASSERT(0);
                }
                return 1;
            }
        }
    }
    return 0;
}

int TestCase2_TurnAround_Test(int is_begin, LibU64_t *curr_clk)
{
    static int verifyCtr = 0;

    if (is_begin)
    {
        static int state = 0;
        
        if (state == 0)
        {
            state = 1;
            DUMP_TEST_START();
            gEnableCurrClkDump = 0;
            g_pseudo_system_clock.hi = 0x0000FFFE;
            printf("system clock is adjusted...\n");
            dump_curr_clk_with_log(NULL, 1);
            TestCase1_AddTimer_Test_S_RegisterTimers();
        }

        TestCase1_AddTimer_Test_S_VerifyOneByOne(&verifyCtr);
    }
    else
    {
        //test over
        {
            LibU64_t target, result;
            target.hi = 2;
            target.lo = 0;
            LIB_U64_COMPARE_t cmp = LibU48_Diff48(&target, curr_clk, &result, 0x00008000);
            if (cmp & B_IS_GTE)
            {
                DUMP_TEST_OVER();
                dump_list3("used   ", USED_LIST);
                if(verifyCtr != 3)
                { 
                    DUMPNX(cmp);
                    DUMPND(verifyCtr);
                    DUMPNX_LIBU64(*curr_clk);
                    dump_list3("used   ", USED_LIST);
                    BASIC_ASSERT(0);
                }
                return 1;
            }
        }
    }
    return 0;
}

int TestCase3_TurnAround_DiffUnit_62500_Test(int is_begin, LibU64_t *curr_clk)
{
    static int verifyCtr = 0;

    if (is_begin)
    {
        static int state = 0;
        
        if (state == 0)
        {
            state = 1;
            DUMP_TEST_START();
            gEnableCurrClkDump = 0;
            g_pseudo_system_clock.hi = 0x0000FFFF;
            g_pseudo_system_clock.lo = (0xFFFFFFFF - 100000) + 1;
            gSimTimeSlice = 2500;
            gMsToTimerTicks = 62500;
            printf("system clock is adjusted...\n");
            dump_curr_clk_with_log(NULL, 1);
            TestCase1_AddTimer_Test_S_RegisterTimers();
        }

        TestCase1_AddTimer_Test_S_VerifyOneByOne(&verifyCtr);
    }
    else
    {
        //test over
        // 9 x 62500 = 562500
        {
            LibU64_t target, result;
            target.hi = 0;
            target.lo = (62500 * 9) + 1;
            LIB_U64_COMPARE_t cmp = LibU48_Diff48(&target, curr_clk, &result, 0x00008000);
            if (cmp & B_IS_GTE)
            {
                DUMP_TEST_OVER();
                if(verifyCtr != 4)
                { 
                    DUMPNX(cmp);
                    DUMPND(verifyCtr);
                    DUMPNX_LIBU64(*curr_clk);
                    dump_list3("used   ", USED_LIST);
                    BASIC_ASSERT(0);
                }
                return 1;
            }
        }
    }
    return 0;
}

int TestCase4_TurnAround_DiffUnit_250000_Test(int is_begin, LibU64_t *curr_clk)
{
    static int verifyCtr = 0;

    if (is_begin)
    {
        static int state = 0;
        
        if (state == 0)
        {
            state = 1;
            DUMP_TEST_START();
            gEnableCurrClkDump = 0;
            g_pseudo_system_clock.hi = 0x0000FFFF;
            g_pseudo_system_clock.lo = (0xFFFFFFFF - 600000) + 1;
            gSimTimeSlice = 50000;
            gMsToTimerTicks = 250000;
            printf("system clock is adjusted...\n");
            dump_curr_clk_with_log(NULL, 1);
            TestCase1_AddTimer_Test_S_RegisterTimers();
        }

        TestCase1_AddTimer_Test_S_VerifyOneByOne(&verifyCtr);
    }
    else
    {
        //test over
        // 9 x 62500 = 562500
        {
            LibU64_t target, result;
            target.hi = 0;
            target.lo = (62500 * 4 * 9) + 1 - 600000;
            LIB_U64_COMPARE_t cmp = LibU48_Diff48(&target, curr_clk, &result, 0x00008000);
            if (cmp & B_IS_GTE)
            {
                DUMP_TEST_OVER();
                if(verifyCtr != 4)
                { 
                    DUMPNX(cmp);
                    DUMPND(verifyCtr);
                    DUMPNX_LIBU64(*curr_clk);
                    dump_list3("used   ", USED_LIST);
                    BASIC_ASSERT(0);
                }
                return 1;
            }
        }
    }
    return 0;
}

void TestCase5_Repeat_Test_TimesupCB(struct timer_request_node *first, LibU64_t *p_curr_clk)
{
    switch (first->req.timer_id)
    {
        case 1111:
            LibVCD_ValueChange(0, 1);
            break;
        case 2222:
            LibVCD_ValueChange(1, 1);
            break;
        case 3333:
            LibVCD_ValueChange(2, 1);
            break;
    }
}

int TestCase5_Repeat_Test(int is_begin, LibU64_t *curr_clk)
{
/*
    unit : 2
    time slice : 1
    Timer A : repeat 2 times,  du=3
    Timer B : repeat infinite, du=2
    Timer C : repeat 2 times,  du=4
     F6 F7 F8 F9 FA FB FC FD FE FF  0  1  2
    -10 -9 -8 -7 -6 -5 -4 -3 -2 -1  0  1  2  3  4  5  6  7  8  9  10  11  12
    (A)                 A                 A
       (B)           B           B           B           B             B
       (C)                       C                       C

    Sequence: B A CB A B CB B
*/
    LibVCD_WireInfo_t test[3] = {
        {2, "A", VALUE_IN_FOLLOWING, 0},
        {2, "B", VALUE_IN_FOLLOWING, 0},
        {2, "C", VALUE_IN_FOLLOWING, 0},
    };

    if (is_begin)
    {
        static int state = 0;
        
        switch (state)
        {
            case 0:
                state = 1;
                DUMP_TEST_START();
                gEnableTimesUpDump = 0;
                gEnableCurrClkDump = 0;
                g_pseudo_system_clock.hi = 0x0000FFFF;
                g_pseudo_system_clock.lo = (0xFFFFFFFF - 10) + 1;
                gSimTimeSlice = 1;
                gMsToTimerTicks = 2;
                printf("system clock is adjusted...\n");
                dump_curr_clk_with_log(NULL, 1);
                g_timer_request_buf.command_flags = ADD_TIMER | REPEAT_FINITE;
                g_timer_request_buf.repeat_counts = 2;
                g_timer_request_buf.timer_id = 1111;
                g_timer_request_buf.time_in_ms = 3;
                Timer48_Message_In_Dump();

                LibVCD_Init("example.vcd", 1, TIME_UNIT_US, test, 3);
                /*
                LibVCD_ClockAdd(300);
                LibVCD_ValueChange(0, 0);
                LibVCD_ClockAdd(200);
                LibVCD_ValueChangeToDontCare(2);
                LibVCD_ClockAdd(100);
                LibVCD_ValueChange(2, 255);
                LibVCD_ValueChange(1, 2);
                LibVCD_ClockAdd(100);
                LibVCD_Uninit();
                */
                gTest_TimesupCB = TestCase5_Repeat_Test_TimesupCB;
                LibVCD_ValueChange(0, 2);
                break;
            case 1:
                if (LibU64_Compare2(0x0000FFFF, 0xFFFFFFF7, curr_clk) == A_B_ARE_EQUAL)
                {
                    state = 2;
                    g_timer_request_buf.command_flags = ADD_TIMER | REPEAT_INFINITE;
                    g_timer_request_buf.repeat_counts = 2;
                    g_timer_request_buf.timer_id = 2222;
                    g_timer_request_buf.time_in_ms = 2;
                    Timer48_Message_In_Dump();
                    LibVCD_ValueChange(1, 2);
                    g_timer_request_buf.command_flags = ADD_TIMER | REPEAT_FINITE;
                    g_timer_request_buf.repeat_counts = 2;
                    g_timer_request_buf.timer_id = 3333;
                    g_timer_request_buf.time_in_ms = 4;
                    Timer48_Message_In_Dump();
                    LibVCD_ValueChange(2, 2);
                    dump_all();
                }
                break;
        }
    }
    else
    {
        LibVCD_ClockAdd(1);
        LibVCD_ValueChange(0, 0);
        LibVCD_ValueChange(1, 0);
        LibVCD_ValueChange(2, 0);

        //test over
        {
            LibU64_t target, result;
            target.hi = 0;
            target.lo = 12;
            LIB_U64_COMPARE_t cmp = LibU48_Diff48(&target, curr_clk, &result, 0x00008000);
            if (cmp & B_IS_GTE)
            {
                DUMP_TEST_OVER();
                LibVCD_Uninit();
                gTest_TimesupCB = NULL;
                /*
                if(verifyCtr != 4)
                { 
                    DUMPNX(cmp);
                    DUMPND(verifyCtr);
                    DUMPNX_LIBU64(*curr_clk);
                    dump_list3("used   ", USED_LIST);
                    BASIC_ASSERT(0);
                }
                */
                return 1;
            }
        }
    }
    return 0;
}

void TestCase6_DeleteTimer_GetDura_Test_TimesupCB(struct timer_request_node *first, LibU64_t *p_curr_clk)
{
    switch (first->req.timer_id)
    {
        case 1111:
            LibVCD_ValueChange(0, 1);
            break;
        case 2222:
            LibVCD_ValueChange(1, 1);
            break;
        case 3333:
            LibVCD_ValueChange(2, 1);
            break;
    }
}

int TestCase6_DeleteTimer_GetDura_Test(int is_begin, LibU64_t *curr_clk)
{
/*
    unit : 2
    time slice : 1
    Timer A : du=5
    Timer B : du=7
    Timer C : du=8
     F6 F7 F8 F9 FA FB FC FD FE FF  0  1  2
    -10 -9 -8 -7 -6 -5 -4 -3 -2 -1  0  1  2  3  4  5  6  7  8  9  10  11  12
    (A)                             A
    (B)                                         B
    (C)                                               C

    del mid   -B               du=1.5=1
    del tail        -C         du=2.5=2
    del head              -A   du=3.5=3
*/
    LibVCD_WireInfo_t test[3] = {
        {3, "A", VALUE_IN_FOLLOWING, 0},
        {3, "B", VALUE_IN_FOLLOWING, 0},
        {3, "C", VALUE_IN_FOLLOWING, 0},
    };

    static int state = 0;

    if (is_begin)
    {
        switch (state)
        {
            case 0:
                state = 1;
                DUMP_TEST_START();
                gEnableTimesUpDump = 0;
                gEnableCurrClkDump = 0;
                g_pseudo_system_clock.hi = 0x0000FFFF;
                g_pseudo_system_clock.lo = (0xFFFFFFFF - 10) + 1;
                gSimTimeSlice = 1;
                gMsToTimerTicks = 2;
                printf("system clock is adjusted...\n");
                dump_curr_clk_with_log(NULL, 1);
                LibVCD_Init("example6.vcd", 1, TIME_UNIT_US, test, 3);
                
                g_timer_request_buf.command_flags = ADD_TIMER_RETURN_DURATION;
                g_timer_request_buf.repeat_counts = 0;
                g_timer_request_buf.timer_id = 1111;
                g_timer_request_buf.time_in_ms = 5;
                Timer48_Message_In_Dump();
                LibVCD_ValueChange(0, 2);
                
                g_timer_request_buf.command_flags = ADD_TIMER;
                g_timer_request_buf.repeat_counts = 0;
                g_timer_request_buf.timer_id = 2222;
                g_timer_request_buf.time_in_ms = 7;
                Timer48_Message_In_Dump();
                LibVCD_ValueChange(1, 2);
                
                g_timer_request_buf.command_flags = ADD_TIMER;
                g_timer_request_buf.repeat_counts = 0;
                g_timer_request_buf.timer_id = 3333;
                g_timer_request_buf.time_in_ms = 8;
                Timer48_Message_In_Dump();
                LibVCD_ValueChange(2, 2);

                gTest_TimesupCB = TestCase6_DeleteTimer_GetDura_Test_TimesupCB;
                break;
            case 1:
                if (LibU64_Compare2(0x0000FFFF, 0xFFFFFFF9, curr_clk) == A_B_ARE_EQUAL)
                {
                    state = 2;
                    g_timer_request_buf.command_flags = DEL_TIMER | RETURN_DURATION;
                    //g_timer_request_buf.repeat_counts = 0;
                    g_timer_request_buf.timer_id = 2222;
                    //g_timer_request_buf.time_in_ms = 8;
                    Timer48_Message_In_Dump();
                    //DUMPNX(g_timer_response_buf.command_flags);
                    //DUMPND(g_timer_response_buf.timer_id);
                    //DUMPND(g_timer_response_buf.duration_in_ms);
                    BASIC_ASSERT(g_timer_response_buf.command_flags == (DEL_TIMER | RETURN_DURATION));
                    BASIC_ASSERT(g_timer_response_buf.timer_id == 2222);
                    BASIC_ASSERT(g_timer_response_buf.duration_in_ms == 1);
                    LibVCD_ValueChange(1, 4);
                }
                break;
            case 2:
                if (LibU64_Compare2(0x0000FFFF, 0xFFFFFFFB, curr_clk) == A_B_ARE_EQUAL)
                {
                    state = 3;
                    g_timer_request_buf.command_flags = DEL_TIMER | RETURN_DURATION;
                    //g_timer_request_buf.repeat_counts = 0;
                    g_timer_request_buf.timer_id = 3333;
                    //g_timer_request_buf.time_in_ms = 8;
                    Timer48_Message_In_Dump();
                    //DUMPNX(g_timer_response_buf.command_flags);
                    //DUMPND(g_timer_response_buf.timer_id);
                    //DUMPND(g_timer_response_buf.duration_in_ms);
                    BASIC_ASSERT(g_timer_response_buf.command_flags == (DEL_TIMER | RETURN_DURATION));
                    BASIC_ASSERT(g_timer_response_buf.timer_id == 3333);
                    BASIC_ASSERT(g_timer_response_buf.duration_in_ms == 2);
                    LibVCD_ValueChange(2, 4);
                }
                break;
            case 3:
                if (LibU64_Compare2(0x0000FFFF, 0xFFFFFFFD, curr_clk) == A_B_ARE_EQUAL)
                {
                    state = 4;
                    g_timer_request_buf.command_flags = DEL_TIMER;
                    //g_timer_request_buf.repeat_counts = 0;
                    g_timer_request_buf.timer_id = 1111;
                    //g_timer_request_buf.time_in_ms = 8;
                    Timer48_Message_In_Dump();
                    //DUMPNX(g_timer_response_buf.command_flags);
                    //DUMPND(g_timer_response_buf.timer_id);
                    //DUMPND(g_timer_response_buf.duration_in_ms);
                    BASIC_ASSERT(g_timer_response_buf.command_flags == (DEL_TIMER));
                    BASIC_ASSERT(g_timer_response_buf.timer_id == 1111);
                    BASIC_ASSERT(g_timer_response_buf.duration_in_ms == 3);
                    LibVCD_ValueChange(0, 4);
                    if (LLIST_IS_NOT_EMPTY(USED_LIST))
                    {
                        printf("Error : timer list should be empty ...\n");
                        BASIC_ASSERT(0);
                    }
                }
                break;
        }
    }
    else
    {
        LibVCD_ClockAdd(1);
        LibVCD_ValueChange(0, 0);
        LibVCD_ValueChange(1, 0);
        LibVCD_ValueChange(2, 0);

        //test over
        {
            LibU64_t target, result;
            target.hi = 0;
            target.lo = 12;
            LIB_U64_COMPARE_t cmp = LibU48_Diff48(&target, curr_clk, &result, 0x00008000);
            if (cmp & B_IS_GTE)
            {
                DUMP_TEST_OVER();
                LibVCD_Uninit();
                gTest_TimesupCB = NULL;

                if (state != 4)
                {
                    DUMPND(state);
                    BASIC_ASSERT(0);
                }
                if (LLIST_IS_NOT_EMPTY(USED_LIST))
                {
                    printf("Error : timer list should be empty ...\n");
                    BASIC_ASSERT(0);
                }
                return 1;
            }
        }
    }
    return 0;
}

void TestCase7_RestartTimer_GetDura_Test_TimesupCB(struct timer_request_node *first, LibU64_t *p_curr_clk)
{
    static int vCtr = 0;

    switch (first->req.timer_id)
    {
        case 1111:
            BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(0, 7, p_curr_clk));
            LibVCD_ValueChange(0, 1);
            break;
        case 2222:
            BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(0, 7, p_curr_clk));
            LibVCD_ValueChange(1, 1);
            break;
        case 3333:
            BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(0, 11, p_curr_clk));
            LibVCD_ValueChange(2, 1);
            break;
        case 4444:
            switch (vCtr) {
                case 0: vCtr=1; BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(0x0000FFFF, 0xFFFFFFFA, p_curr_clk));
                    break;
                case 1: vCtr=2; BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(0, 0, p_curr_clk));
                    break;
                case 2: vCtr=3; BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(0, 4, p_curr_clk));
                    break;
                case 3: vCtr=4; BASIC_ASSERT(A_B_ARE_EQUAL == LibU64_Compare2(0, 8, p_curr_clk));
                    break;
            };
            LibVCD_ValueChange(3, 1);
            break;
    }
}

int TestCase7_RestartTimer_GetDura_Test(int is_begin, LibU64_t *curr_clk)
{
/*
    unit : 2
    time slice : 1
    Timer A : du=5
    Timer B : du=7
    Timer C : du=8
    Timer D : du=2, repeat 3 times
     F6 F7 F8 F9 FA FB FC FD FE FF  0  1  2
    -10 -9 -8 -7 -6 -5 -4 -3 -2 -1  0  1  2  3  4  5  6  7  8  9  10  11  12
    (A)                             A...
    (B)                                         B...
    (C)                                               C...
    (D)           D    ~D du=1      D           D           D

     re mid   ~B               du=1.5=1                  B(time's up of re-start)
     re tail        ~C         du=2.5=2                                C(time's up of re-start)
     re head              ~A   du=3.5=3                  A(time's up of re-start)
*/
    LibVCD_WireInfo_t test[] = {
        {3, "A", VALUE_IN_FOLLOWING, 0},
        {3, "B", VALUE_IN_FOLLOWING, 0},
        {3, "C", VALUE_IN_FOLLOWING, 0},
        {3, "D", VALUE_IN_FOLLOWING, 0},
    };

    static int state = 0;

    if (is_begin)
    {
        switch (state)
        {
            case 0:
                state = 1;
                DUMP_TEST_START();
                gEnableTimesUpDump = 0;
                gEnableCurrClkDump = 1;
                g_pseudo_system_clock.hi = 0x0000FFFF;
                g_pseudo_system_clock.lo = (0xFFFFFFFF - 10) + 1;
                gSimTimeSlice = 1;
                gMsToTimerTicks = 2;
                printf("system clock is adjusted...\n");
                dump_curr_clk_with_log(NULL, 1);
                LibVCD_Init("example7.vcd", 1, TIME_UNIT_US, test, sizeof(test)/sizeof(LibVCD_WireInfo_t));
                
                g_timer_request_buf.command_flags = ADD_TIMER_RETURN_DURATION;
                g_timer_request_buf.repeat_counts = 0;
                g_timer_request_buf.timer_id = 1111;
                g_timer_request_buf.time_in_ms = 5;
                Timer48_Message_In_Dump();
                LibVCD_ValueChange(0, 2);
                
                g_timer_request_buf.command_flags = ADD_TIMER;
                g_timer_request_buf.repeat_counts = 0;
                g_timer_request_buf.timer_id = 2222;
                g_timer_request_buf.time_in_ms = 7;
                Timer48_Message_In_Dump();
                LibVCD_ValueChange(1, 2);
                
                g_timer_request_buf.command_flags = ADD_TIMER;
                g_timer_request_buf.repeat_counts = 0;
                g_timer_request_buf.timer_id = 3333;
                g_timer_request_buf.time_in_ms = 8;
                Timer48_Message_In_Dump();
                LibVCD_ValueChange(2, 2);

                g_timer_request_buf.command_flags = ADD_TIMER_RETURN_DURATION | REPEAT_FINITE;
                g_timer_request_buf.repeat_counts = 3;
                g_timer_request_buf.timer_id = 4444;
                g_timer_request_buf.time_in_ms = 2;
                Timer48_Message_In_Dump();
                LibVCD_ValueChange(3, 2);

                gTest_TimesupCB = TestCase7_RestartTimer_GetDura_Test_TimesupCB;
                break;
            case 1:
                if (LibU64_Compare2(0x0000FFFF, 0xFFFFFFF9, curr_clk) == A_B_ARE_EQUAL)
                {
                    state = 2;
                    g_timer_request_buf.command_flags = RESTART_TIMER | RETURN_DURATION;
                    //g_timer_request_buf.repeat_counts = 0;
                    g_timer_request_buf.timer_id = 2222;
                    //g_timer_request_buf.time_in_ms = 8;
                    Timer48_Message_In_Dump();
                    //DUMPNX(g_timer_response_buf.command_flags);
                    //DUMPND(g_timer_response_buf.timer_id);
                    //DUMPND(g_timer_response_buf.duration_in_ms);
                    BASIC_ASSERT(g_timer_response_buf.command_flags == (RESTART_TIMER | RETURN_DURATION));
                    BASIC_ASSERT(g_timer_response_buf.timer_id == 2222);
                    BASIC_ASSERT(g_timer_response_buf.duration_in_ms == 1);
                    LibVCD_ValueChange(1, 4);
                }
                break;
            case 2:
                if (LibU64_Compare2(0x0000FFFF, 0xFFFFFFFB, curr_clk) == A_B_ARE_EQUAL)
                {
                    state = 3;
                    g_timer_request_buf.command_flags = RESTART_TIMER | RETURN_DURATION;
                    //g_timer_request_buf.repeat_counts = 0;
                    g_timer_request_buf.timer_id = 3333;
                    //g_timer_request_buf.time_in_ms = 8;
                    Timer48_Message_In_Dump();
                    //DUMPNX(g_timer_response_buf.command_flags);
                    //DUMPND(g_timer_response_buf.timer_id);
                    //DUMPND(g_timer_response_buf.duration_in_ms);
                    BASIC_ASSERT(g_timer_response_buf.command_flags == (RESTART_TIMER | RETURN_DURATION));
                    BASIC_ASSERT(g_timer_response_buf.timer_id == 3333);
                    BASIC_ASSERT(g_timer_response_buf.duration_in_ms == 2);
                    LibVCD_ValueChange(2, 4);
                }
                break;
            case 3:
                if (LibU64_Compare2(0x0000FFFF, 0xFFFFFFFC, curr_clk) == A_B_ARE_EQUAL)
                {
                    state = 4;
                    g_timer_request_buf.command_flags = RESTART_TIMER;
                    //g_timer_request_buf.repeat_counts = 0;
                    g_timer_request_buf.timer_id = 4444;
                    //g_timer_request_buf.time_in_ms = 8;
                    Timer48_Message_In_Dump();
                    //DUMPNX(g_timer_response_buf.command_flags);
                    //DUMPND(g_timer_response_buf.timer_id);
                    //DUMPND(g_timer_response_buf.duration_in_ms);
                    BASIC_ASSERT(g_timer_response_buf.command_flags == (RESTART_TIMER));
                    BASIC_ASSERT(g_timer_response_buf.timer_id == 4444);
                    BASIC_ASSERT(g_timer_response_buf.duration_in_ms == 1);
                    LibVCD_ValueChange(3, 4);
                }
                break;
            case 4:
                if (LibU64_Compare2(0x0000FFFF, 0xFFFFFFFD, curr_clk) == A_B_ARE_EQUAL)
                {
                    state = 5;
                    g_timer_request_buf.command_flags = RESTART_TIMER;
                    //g_timer_request_buf.repeat_counts = 0;
                    g_timer_request_buf.timer_id = 1111;
                    //g_timer_request_buf.time_in_ms = 8;
                    Timer48_Message_In_Dump();
                    //DUMPNX(g_timer_response_buf.command_flags);
                    //DUMPND(g_timer_response_buf.timer_id);
                    //DUMPND(g_timer_response_buf.duration_in_ms);
                    BASIC_ASSERT(g_timer_response_buf.command_flags == (RESTART_TIMER));
                    BASIC_ASSERT(g_timer_response_buf.timer_id == 1111);
                    BASIC_ASSERT(g_timer_response_buf.duration_in_ms == 3);
                    LibVCD_ValueChange(0, 4);
                }
                break;
        }
    }
    else
    {
        LibVCD_ClockAdd(1);
        LibVCD_ValueChange(0, 0);
        LibVCD_ValueChange(1, 0);
        LibVCD_ValueChange(2, 0);
        LibVCD_ValueChange(3, 0);

        //test over
        {
            LibU64_t target, result;
            target.hi = 0;
            target.lo = 12;
            LIB_U64_COMPARE_t cmp = LibU48_Diff48(&target, curr_clk, &result, 0x00008000);
            if (cmp & B_IS_GTE)
            {
                DUMP_TEST_OVER();
                LibVCD_Uninit();
                gTest_TimesupCB = NULL;

                if (state != 5)
                {
                    DUMPND(state);
                    BASIC_ASSERT(0);
                }
                if (LLIST_IS_NOT_EMPTY(USED_LIST))
                {
                    printf("Error : timer list should be empty ...\n");
                    BASIC_ASSERT(0);
                }
                return 1;
            }
        }
    }
    return 0;
}

/*
    unit : 2
    time slice : 1
    Timer A/B : one-shot
    Timer C/D : finite
    Timer E/F : infinite
    time: -500~1000
*/
#define FREE_RUN_TIMER_NUM (6)
u32 gFreeRunStartOffset = 300;
u32 gFreeRunEndClk = 300;
int gActiveFlag[FREE_RUN_TIMER_NUM] = {0};
u32 gSleepCtr[FREE_RUN_TIMER_NUM] = {0}; //2~10ms
u32 gSleepCtrDel;       //2~10ms
u32 gSleepCtrRestart;   //2~10ms
u32 gDura[FREE_RUN_TIMER_NUM] = {0};     //2~10ms
u32 gRepeat[FREE_RUN_TIMER_NUM] = {0};   //3~10, only works on C,D
u32 gRepeatCtr[FREE_RUN_TIMER_NUM] = {0};   // only works on C,D
LibU64_t gFirstEndTime[FREE_RUN_TIMER_NUM];
//u16 gCmdFlag[FREE_RUN_TIMER_NUM];

// bit 0: time's up
// bit 1: add timer         0x02
// bit 2: del timer         0x04
// bit 3: restart timer     0x08
LibVCD_WireInfo_t gTestWire8[] = {
    {4, "A", VALUE_IN_FOLLOWING, 0},
    {4, "B", VALUE_IN_FOLLOWING, 0},
    {4, "C", VALUE_IN_FOLLOWING, 0},
    {4, "D", VALUE_IN_FOLLOWING, 0},
    {4, "E", VALUE_IN_FOLLOWING, 0},
    {4, "F", VALUE_IN_FOLLOWING, 0},

    {4, "A_Du", VALUE_IN_FOLLOWING, 0}, //6
    {4, "B_Du", VALUE_IN_FOLLOWING, 0},
    {4, "C_Du", VALUE_IN_FOLLOWING, 0},
    {4, "D_Du", VALUE_IN_FOLLOWING, 0},
    {4, "E_Du", VALUE_IN_FOLLOWING, 0},
    {4, "F_Du", VALUE_IN_FOLLOWING, 0},

    {4, "C_Re", VALUE_IN_FOLLOWING, 0}, //12
    {4, "D_Re", VALUE_IN_FOLLOWING, 0},
};
#define OFFSET_DU (6)
#define OFFSET_C_RE (12)
#define OFFSET_D_RE (13)

void TestCase8_FreeRun_Test_AddTimer(u32 i)
{
    LibVCD_ValueChange(i+OFFSET_DU, 0); // reset  du
    gActiveFlag[i] = 0;
    gSleepCtr[i] = LibUtil_GetRangeRand8(2,10);
    gDura[i] = LibUtil_GetRangeRand8(2,10);
    gRepeat[i] = LibUtil_GetRangeRand8(2,10);
    gRepeatCtr[i] = 0;
}

void TestCase8_FreeRun_Test_TimesupCB(struct timer_request_node *first, LibU64_t *p_curr_clk)
{
    u32 i = (first->req.timer_id / 1111) - 1;
    //BASIC_ASSERT(index >= 0 && index <= 5);
    LibVCD_ValueChange(i, 1);
    switch (first->req.timer_id)
    {
        case 1111:
        case 2222:
            BASIC_ASSERT( A_B_ARE_EQUAL == LibU64_Compare(&(gFirstEndTime[i]), p_curr_clk) );
            TestCase8_FreeRun_Test_AddTimer(i);
            break;
        case 3333:
        case 4444:
            u32 loop;
            LibU64_t dura, currEndTime;
            currEndTime.hi = 0;
            currEndTime.lo = 0;
            dura.hi = 0;
            dura.lo = 0;
            // calc dura
            MUL48S(first->req.time_in_ms, gMsToTimerTicks, &dura);
            loop = gRepeatCtr[i];
            while (loop) {
                ADD48(&currEndTime, &dura, &currEndTime);
                loop--;
            }
            ADD48(&currEndTime, &(gFirstEndTime[i]), &currEndTime);
            BASIC_ASSERT( A_B_ARE_EQUAL == LibU64_Compare(&currEndTime, p_curr_clk) );

            gRepeatCtr[i]+=1;
            if (gRepeatCtr[i] == gRepeat[i])
            {
                TestCase8_FreeRun_Test_AddTimer(i);
            }
            break;
        case 5555:
            break;
        case 6666:
            break;
    }
}

int TestCase8_FreeRun_Test(int is_begin, LibU64_t *curr_clk)
{
/*
    unit : 2
    time slice : 1
    Timer A/B : one-shot
    Timer C/D : finite
    Timer E/F : infinite
    time: -500~1000

    A/C/E: add-dura
*/

    static int state = 0;

    if (is_begin)
    {
        switch (state)
        {
            case 0:
                state = 1;
                DUMP_TEST_START();
                gEnableMessageInDump = 0;
                gEnableTimesUpDump = 0;
                gEnableCurrClkDump = 0;
                g_pseudo_system_clock.hi = 0;
                g_pseudo_system_clock.lo = 0;
                LibU64_t decrement;
                decrement.hi = 0;
                decrement.lo = gFreeRunStartOffset;
                SUB48(&g_pseudo_system_clock, &decrement, &g_pseudo_system_clock);
                gSimTimeSlice = 1;
                gMsToTimerTicks = 2;
                printf("system clock is adjusted...\n");
                dump_curr_clk_with_log(NULL, 1);
                LibVCD_Init("example8.vcd", 1, TIME_UNIT_US, gTestWire8, sizeof(gTestWire8)/sizeof(LibVCD_WireInfo_t));

                LibUtil_InitRand();
                FOR_I(6) {
                    gActiveFlag[i] = 0;
                    gSleepCtr[i] = LibUtil_GetRangeRand8(2,10);
                    gDura[i] = LibUtil_GetRangeRand8(2,10);
                    gRepeat[i] = LibUtil_GetRangeRand8(2,10);
                    gRepeatCtr[i] = 0;
                }
                gSleepCtrDel = 11;
                gSleepCtrRestart = 13;

                gTest_TimesupCB = TestCase8_FreeRun_Test_TimesupCB;
                break;
            case 1:
                FOR_I(6) {
                    // add timer check
                    if (gSleepCtr[i] == 0 && gActiveFlag[i] == 0 && gIsThisRoundTimesUp == 0) {
                        //BASIC_ASSERT(gActiveFlag[i] == 0);
                        LibVCD_ValueChange(i, 0x02); // add of wire
                        LibVCD_ValueChange(i+OFFSET_DU, gDura[i]);
                        switch (i) {
                            case 0:
                            case 1: //Timer A/B : one-shot   A/C/E: add-dura
                                if (i == 0) {
                                    g_timer_request_buf.command_flags = ADD_TIMER_RETURN_DURATION;
                                    g_timer_request_buf.timer_id = 1111;
                                } else {
                                    g_timer_request_buf.command_flags = ADD_TIMER;
                                    g_timer_request_buf.timer_id = 2222;
                                }
                                //g_timer_request_buf.repeat_counts = 0;
                                g_timer_request_buf.time_in_ms = gDura[i];
                                break;
                            case 2:
                            case 3: //Timer C/D : finite     A/C/E: add-dura
                                if (i == 2) {
                                    g_timer_request_buf.command_flags = ADD_TIMER_RETURN_DURATION | REPEAT_FINITE;
                                    g_timer_request_buf.timer_id = 3333;
                                    LibVCD_ValueChange(OFFSET_C_RE, gRepeat[i]);
                                } else {
                                    g_timer_request_buf.command_flags = ADD_TIMER | REPEAT_FINITE;
                                    g_timer_request_buf.timer_id = 4444;
                                    LibVCD_ValueChange(OFFSET_D_RE, gRepeat[i]);
                                }
                                g_timer_request_buf.repeat_counts = gRepeat[i];
                                g_timer_request_buf.time_in_ms = gDura[i];
                                gRepeatCtr[i] = 0;
                                break;
                            case 4:
                            case 5: //Timer E/F : infinite   A/C/E: add-dura
                                if (i == 4) {
                                    g_timer_request_buf.command_flags = ADD_TIMER_RETURN_DURATION | REPEAT_INFINITE;
                                    g_timer_request_buf.timer_id = 5555;
                                } else {
                                    g_timer_request_buf.command_flags = ADD_TIMER | REPEAT_INFINITE;
                                    g_timer_request_buf.timer_id = 6666;
                                }
                                //g_timer_request_buf.repeat_counts = 0;
                                g_timer_request_buf.time_in_ms = gDura[i];
                                break;
                        }
                        Timer48_Message_In_Dump();
                        gActiveFlag[i] = 1;
                        LibU64_t end_time;
                        MUL48S(gDura[i], gMsToTimerTicks, &end_time);
                        ADD48(&end_time, curr_clk, &end_time);
                        gFirstEndTime[i] = end_time;
                    } else {
                        if (gSleepCtr[i])
                            gSleepCtr[i] -= 1;
                    }
                }
                if (gSleepCtrRestart == 0) {
                    u32 restart_idx = LibUtil_GetRangeRand8(0,4);
                    if (gActiveFlag[restart_idx] == 1 && gIsThisRoundTimesUp == 0)
                    {
                        u32 target_id = (restart_idx+1) * 1111;
                        gSleepCtrRestart = LibUtil_GetRangeRand8(4,15);
                        g_timer_request_buf.command_flags = RESTART_TIMER;
                        g_timer_request_buf.timer_id = target_id;
                        Timer48_Message_In_Dump();
                        gRepeatCtr[restart_idx] = 0;
                        LibVCD_ValueChange(restart_idx, 0x08); // restart of wire
                        LibU64_t end_time;
                        MUL48S(gDura[restart_idx], gMsToTimerTicks, &end_time);
                        ADD48(&end_time, curr_clk, &end_time);
                        gFirstEndTime[restart_idx] = end_time;
                    }
                } else {
                    if (gSleepCtrRestart)
                        gSleepCtrRestart -= 1;
                }
                // DEL_TIMER
                if (gSleepCtrDel == 0) {
                    u32 restart_idx = LibUtil_GetRangeRand8(0,4);
                    if (gActiveFlag[restart_idx] == 1 && gIsThisRoundTimesUp == 0)
                    {
                        do {
                            u32 target_id = (restart_idx+1) * 1111;
                            gSleepCtrDel = LibUtil_GetRangeRand8(6,18);
                            g_timer_request_buf.command_flags = DEL_TIMER;
                            g_timer_request_buf.timer_id = target_id;
                            Timer48_Message_In_Dump();
                            //gRepeatCtr[restart_idx] = 0;
                            LibVCD_ValueChange(restart_idx, 0x04); // del of wire
                            TestCase8_FreeRun_Test_AddTimer(restart_idx);
                        } while (0);
                    }
                } else {
                    if (gSleepCtrDel)
                        gSleepCtrDel -= 1;
                }
                break;
        }
    }
    else
    {
        LibVCD_ClockAdd(1);
        LibVCD_ValueChange(0, 0);
        LibVCD_ValueChange(1, 0);
        LibVCD_ValueChange(2, 0);
        LibVCD_ValueChange(3, 0);
        LibVCD_ValueChange(4, 0);
        LibVCD_ValueChange(5, 0);

        //test over
        {
            LibU64_t target, result;
            target.hi = 0;
            target.lo = gFreeRunEndClk;
            LIB_U64_COMPARE_t cmp = LibU48_Diff48(&target, curr_clk, &result, 0x00008000);
            if (cmp & B_IS_GTE)
            {
                DUMP_TEST_OVER();
                LibVCD_Uninit();
                gTest_TimesupCB = NULL;
                state = 0;
                return 1;
            }
        }
    }
    return 0;
}

void TestCase9_Response_Test_TimesupCB(struct timer_request_node *first, LibU64_t *p_curr_clk)
{
    switch (first->req.timer_id)
    {
        case 1111:
            BASIC_ASSERT(g_timer_response_buf.command_flags == (ADD_TIMER|TIMER_TIMES_UP));
            break;
        case 2222:
            BASIC_ASSERT(g_timer_response_buf.command_flags == (ADD_TIMER_RETURN_DURATION|TIMER_TIMES_UP));
            break;
    }
}

int TestCase9_Response_Test(int is_begin, LibU64_t *curr_clk)
{
    static int state = 0;

    if (is_begin)
    {
        switch (state)
        {
            case 0:
                state = 1;
                DUMP_TEST_START();
                gEnableMessageInDump = 0;
                gEnableTimesUpDump = 0;
                gEnableCurrClkDump = 0;
                gSimTimeSlice = 1;
                gMsToTimerTicks = 2;

                g_timer_request_buf.command_flags = ADD_TIMER;
                g_timer_request_buf.repeat_counts = 1;
                g_timer_request_buf.timer_id = 1111;
                g_timer_request_buf.time_in_ms = 3;
                Timer48_Message_In_Dump();
                g_timer_request_buf.command_flags = DEL_TIMER;
                g_timer_request_buf.timer_id = 2222;
                Timer48_Message_In_Dump();
                BASIC_ASSERT(g_timer_response_buf.command_flags == (DEL_TIMER | CAN_NOT_FIND_TIMER));
                BASIC_ASSERT(g_timer_response_buf.timer_id == 2222);
                g_timer_request_buf.command_flags = DEL_TIMER;
                g_timer_request_buf.timer_id = 1111;
                Timer48_Message_In_Dump();
                BASIC_ASSERT(g_timer_response_buf.command_flags == (DEL_TIMER));
                BASIC_ASSERT(g_timer_response_buf.timer_id == 1111);

                g_timer_request_buf.command_flags = ADD_TIMER;
                g_timer_request_buf.repeat_counts = 1;
                g_timer_request_buf.timer_id = 1111;
                g_timer_request_buf.time_in_ms = 3;
                Timer48_Message_In_Dump();
                g_timer_request_buf.command_flags = RESTART_TIMER;
                g_timer_request_buf.timer_id = 2222;
                Timer48_Message_In_Dump();
                BASIC_ASSERT(g_timer_response_buf.command_flags == (RESTART_TIMER | CAN_NOT_FIND_TIMER));
                BASIC_ASSERT(g_timer_response_buf.timer_id == 2222);
                g_timer_request_buf.command_flags = RESTART_TIMER;
                g_timer_request_buf.timer_id = 1111;
                Timer48_Message_In_Dump();
                BASIC_ASSERT(g_timer_response_buf.command_flags == (RESTART_TIMER));
                BASIC_ASSERT(g_timer_response_buf.timer_id == 1111);

                g_timer_request_buf.command_flags = ADD_TIMER_RETURN_DURATION;
                g_timer_request_buf.repeat_counts = 1;
                g_timer_request_buf.timer_id = 2222;
                g_timer_request_buf.time_in_ms = 4;
                Timer48_Message_In_Dump();

                gTest_TimesupCB = TestCase9_Response_Test_TimesupCB;
                break;
            case 1:
                break;
        }
    }
    else
    {
        //test over
        {
            LIB_U64_COMPARE_t cmp = LibU64_Compare2(0, 10, curr_clk);
            if (cmp & B_IS_GTE)
            {
                DUMP_TEST_OVER();
                gTest_TimesupCB = NULL;
                state = 0;
                return 1;
            }
        }
    }
    return 0;
}

typedef int (*tc_t)(int is_begin, LibU64_t *curr_clk);

tc_t gTest_Database[] = {
    TestCase1_AddTimer_Test,
    TestCase2_TurnAround_Test,
    TestCase3_TurnAround_DiffUnit_62500_Test,
    TestCase4_TurnAround_DiffUnit_250000_Test,
    TestCase5_Repeat_Test,
    TestCase6_DeleteTimer_GetDura_Test,
    TestCase7_RestartTimer_GetDura_Test,
    TestCase8_FreeRun_Test, // combination, add before/after turn-around, repeat, del, restart
    TestCase9_Response_Test,
};
static size_t gTestCaseIndex = 0;
static size_t gTestCaseIndex_Increment = 1;
static size_t gTestCaseIndexMax = sizeof(gTest_Database)/sizeof(tc_t);

int Timer48_TestStub(int is_begin)
{
    int do_print = 0;
    int ret = 0;
    LibU64_t result_clock;
    //LibU48_Add48To48((LibU64_t *)&g_pseudo_system_clock, (LibU64_t *)&g_elapsed_clock, (LibU64_t *)&result_clock);

    if (is_begin && gEnableCurrClkDump) {
        do_print = 1;
    }
    dump_curr_clk_with_log(&result_clock, do_print);

    if(is_begin)
    {
        gIsThisRoundTimesUp = 0;
        {
            struct timer_request_node *first = (struct timer_request_node *)LLIST_FIRST(USED_LIST);
            if (first != NULL)
            {
                LIB_U64_COMPARE_t cmp = LibU64_Compare((LibU64_t *)&g_elapsed_clock_hw, (LibU64_t *)&(first->duration_in_list));
                //DUMPNX32(g_elapsed_clock_hw.hi);
                //DUMPNX32(g_elapsed_clock_hw.lo);
                //DUMPNX32(first->duration_in_list.hi);
                //DUMPNX32(first->duration_in_list.lo);
                if (cmp & A_IS_GTE)
                {
                    g_is_times_up = 1;
                    gIsThisRoundTimesUp = 1;
                    if (cmp == A_IS_GREATER)
                    {
                        printf("timer check is not precise ...\n");
                        DUMPNX_LIBU64(g_elapsed_clock_hw);
                        DUMPNX_LIBU64(first->duration_in_list);
                    }
                }
            }
        }
        if (gEnableTimesUpDump && gIsThisRoundTimesUp)
        {
            printf(" --- Time's up dump --- S ---\n");
            dump_list3("used   ", USED_LIST);
        }
    }

    ret = (*gTest_Database[gTestCaseIndex])(is_begin, &result_clock);
    if (ret)
    {
        gTestCaseIndex += gTestCaseIndex_Increment;
        if (gTestCaseIndex >= gTestCaseIndexMax)
        {
            ret = 2;
        }
    }

    // add simulation timer
    if(is_begin)
    {
    }
    else
    {
        {
            if (gEnableTimesUpDump && gIsThisRoundTimesUp)
            {
                printf(" --- Time's up dump --- E ---\n");
                dump_list3("used   ", USED_LIST);
            }
        }
        {
            LibU64_t slice;
            slice.hi = 0;
            slice.lo = gSimTimeSlice;
            LibU48_Add48To48((LibU64_t *)&g_elapsed_clock_hw, (LibU64_t *)&slice, (LibU64_t *)&g_elapsed_clock_hw);
            //Timer48_Get_Elapsed(); only works when timer exist
            g_elapsed_clock = g_elapsed_clock_hw;
        }
    }

    return ret;
}

void Timer48_TimesupStub(struct timer_request_node *first, LibU64_t *p_curr_clk)
{
    if (gTest_TimesupCB != NULL)
    {
        (*gTest_TimesupCB)(first, p_curr_clk);
    }
}
