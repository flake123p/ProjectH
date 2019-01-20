
#include "Everything_TimeSliceSim.hpp"
#include "TimeSliceSim.hpp"

EVENT_HANDLE_t  gSimAirEventAry[4];
THREAD_HANDLE_t gSimAirThreadAry[4];

u32 gGlobalTicks = 0;
//#define LINUX_SET_EVENT_WORKAROUND LibOs_SleepMiliSeconds(10)
#define LINUX_SET_EVENT_WORKAROUND

void *SimAir_Peer00(void *arg)
{
	while (1) {
		LibIPC_Event_Wait(gSimAirEventAry[0]);
		PRINT_FUNC;
		LINUX_SET_EVENT_WORKAROUND;
		LibIPC_Event_Set(gSimAirEventAry[1]);
		//if (gGlobalTicks)
		//	break;
	}

	return 0;
}

void *SimAir_Peer01(void *arg)
{
	while (1) {
		LibIPC_Event_Wait(gSimAirEventAry[1]);
		PRINT_FUNC;
		LINUX_SET_EVENT_WORKAROUND;
		LibIPC_Event_Set(gSimAirEventAry[2]);
		//if (gGlobalTicks)
		//	break;
	}

	return 0;
}

void *SimAir_Peer02(void *arg)
{
	while (1) {
		LibIPC_Event_Wait(gSimAirEventAry[2]);
		PRINT_FUNC;
		LINUX_SET_EVENT_WORKAROUND;
		LibIPC_Event_Set(gSimAirEventAry[3]);
		//LibOs_SleepMiliSeconds(10);
		//if (gGlobalTicks)
		//	break;
	}

	return 0;
}

void *SimAir_GlobalTicker(void *arg)
{
	while (1) {
		LibIPC_Event_Wait(gSimAirEventAry[3]);
		PRINT_FUNC;
		LINUX_SET_EVENT_WORKAROUND;
		LibIPC_Event_Set(gSimAirEventAry[0]);
		//if (gGlobalTicks)
		//	break;
	}

	return 0;
}

void Test(void)
{
	int retVal;

	retVal = LibThread_NewHandle(&gSimAirThreadAry[0]);
	ASSERT_IF(retVal);
	retVal = LibThread_NewHandle(&gSimAirThreadAry[1]);
	ASSERT_IF(retVal);
	retVal = LibThread_NewHandle(&gSimAirThreadAry[2]);
	ASSERT_IF(retVal);
	retVal = LibThread_NewHandle(&gSimAirThreadAry[3]);
	ASSERT_IF(retVal);

	retVal = LibIPC_Event_Create(&gSimAirEventAry[0]);
	ASSERT_IF(retVal);
	retVal = LibIPC_Event_Create(&gSimAirEventAry[1]);
	ASSERT_IF(retVal);
	retVal = LibIPC_Event_Create(&gSimAirEventAry[2]);
	ASSERT_IF(retVal);
	retVal = LibIPC_Event_Create(&gSimAirEventAry[3]);
	ASSERT_IF(retVal);

	retVal = LibThread_Create(gSimAirThreadAry[0], SimAir_Peer00);
	ASSERT_IF(retVal);
	retVal = LibThread_Create(gSimAirThreadAry[1], SimAir_Peer01);
	ASSERT_IF(retVal);
	retVal = LibThread_Create(gSimAirThreadAry[2], SimAir_Peer02);
	ASSERT_IF(retVal);
	retVal = LibThread_Create(gSimAirThreadAry[3], SimAir_GlobalTicker);
	ASSERT_IF(retVal);

	LibOs_SleepMiliSeconds(10); // For linux, prevent SetEvent() is running before WaitEvent() !!
	LibIPC_Event_Set(gSimAirEventAry[0]);
	
	LibTime_StartMicroSecondClock();
	LibThread_WaitThread(gSimAirThreadAry[0]);
	LibThread_WaitThread(gSimAirThreadAry[1]);
	LibThread_WaitThread(gSimAirThreadAry[2]);
	LibThread_WaitThread(gSimAirThreadAry[3]);
	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;

	retVal = LibIPC_Event_Destroy(gSimAirEventAry[0]);
	ASSERT_IF(retVal);
	retVal = LibIPC_Event_Destroy(gSimAirEventAry[1]);
	ASSERT_IF(retVal);
	retVal = LibIPC_Event_Destroy(gSimAirEventAry[2]);
	ASSERT_IF(retVal);
	retVal = LibIPC_Event_Destroy(gSimAirEventAry[3]);
	ASSERT_IF(retVal);

	retVal = LibThread_DestroyHandle(gSimAirThreadAry[0]);
	ASSERT_IF(retVal);
	retVal = LibThread_DestroyHandle(gSimAirThreadAry[1]);
	ASSERT_IF(retVal);
	retVal = LibThread_DestroyHandle(gSimAirThreadAry[2]);
	ASSERT_IF(retVal);
	retVal = LibThread_DestroyHandle(gSimAirThreadAry[3]);
	ASSERT_IF(retVal);
}

int TimeSliceSim_Demo(void)
{
	printf("This is %s()\n", __func__);

	Test();

	return 0;
}

static Time_Slice_Descriptor *g_descriptor_table = NULL;
static u32 g_table_number = 0;
static u32 gTimeStamp1 = 0;
static u32 gTimeStamp2 = 0;
static void TimeSliceSime_UpdateTimeStamp(u32 inTimeStamp)
{
    //if overflow
    if (gTimeStamp2 + inTimeStamp < gTimeStamp2) {
        gTimeStamp1++;
    }
    else if (gTimeStamp2 + inTimeStamp < inTimeStamp) {
        gTimeStamp1++;
    }

    gTimeStamp2 += inTimeStamp;
}

void TimeSliceSime_GetTimeStamp(u32 *outTimeStamp1, u32 *outTimeStamp2)
{
    *outTimeStamp1 = gTimeStamp1;
    *outTimeStamp2 = gTimeStamp2;
}

void TimeSliceSime_SetTimeStamp(u32 inTimeStamp1, u32 inTimeStamp2)
{
    gTimeStamp1 = inTimeStamp1;
    gTimeStamp2 = inTimeStamp2;
}

int TimeSliceSim_Init(Time_Slice_Descriptor *descriptor_table, u32 table_number)
{
    BASIC_ASSERT(descriptor_table != NULL);
    BASIC_ASSERT(table_number != 0);

    g_descriptor_table = descriptor_table;
    g_table_number = table_number;

    return 0;
}

int TimeSliceSim_Start(void)
{
#define CURR_REMAIN_TIME (*(g_descriptor_table[i].p_remain_time))
#define CURR_CALLBACK (g_descriptor_table[i].times_up_cb)

    int ret;

    BASIC_ASSERT(g_descriptor_table != NULL);
    BASIC_ASSERT(g_table_number != 0);

    //Check if there are callbacks need execute immediatly.
    for (u32 i = 0; i<g_table_number; i++)
    {
        if (CURR_REMAIN_TIME == 0)
        {
            if(CURR_CALLBACK == NULL)
            {
                printf("i = %d\n", i);
                BASIC_ASSERT(0);
            }
            else
            {
                ret = (*CURR_CALLBACK)();
            }
        }
    }

    u32 is_there_undone;
    u32 min_remain_time;
    do {
        is_there_undone = 0;
        
        //Find minimun remain time
        min_remain_time = 0xFFFFFFFF;
        for (u32 i = 0; i<g_table_number; i++)
        {
            if (CURR_REMAIN_TIME != 0)
            {
                if (CURR_REMAIN_TIME < min_remain_time)
                {
                    is_there_undone = 1;
                    min_remain_time = CURR_REMAIN_TIME;
                }

                if (CURR_REMAIN_TIME == 0xFFFFFFFF) {
                    is_there_undone = 1;
                }
            }
        }

        if (is_there_undone)
        {
            TimeSliceSime_UpdateTimeStamp(min_remain_time);
            for (u32 i = 0; i<g_table_number; i++)
            {
                if (CURR_REMAIN_TIME != 0)
                {
                    if (CURR_REMAIN_TIME == min_remain_time)
                    {
                        //Reset remain time before execute callback
                        CURR_REMAIN_TIME -= min_remain_time; //must be zero

                        if(CURR_CALLBACK == NULL)
                        {
                            printf("i = %d\n", i);
                            BASIC_ASSERT(0);
                        }
                        else
                        {
                            ret = (*CURR_CALLBACK)();
                        }
                    }
                    else
                    {
                        // for those who doesn't time's up
                        CURR_REMAIN_TIME -= min_remain_time;
                    }
                }
            }
        }
    } while(is_there_undone);

    return ret;
#undef CURR_REMAIN_TIME
#undef CURR_CALLBACK
}

static int Time_Slice_CB_Demo1(void);
static int Time_Slice_CB_Demo2(void);
static int Time_Slice_CB_Demo3(void);
static u32 gDemoRemainTime1  = 3125;
static u32 gDemoRemainTime2  = 0;
static u32 gDemoRemainTime3  = 6250;
Time_Slice_Descriptor gDemoTable[] = 
{
    {&gDemoRemainTime1, Time_Slice_CB_Demo1},
    {&gDemoRemainTime2, Time_Slice_CB_Demo2},
    {&gDemoRemainTime3, Time_Slice_CB_Demo3},
};

static int Time_Slice_CB_Demo1(void)
{
    printf("This is %s()\n", __func__);

    u32 timeStamp1, timeStamp2;
    TimeSliceSime_GetTimeStamp(&timeStamp1, &timeStamp2);
    DUMPD(timeStamp1);
    DUMPD(timeStamp2);

    return 0;
}

static int Time_Slice_CB_Demo2(void)
{
    printf("This is %s()\n", __func__);

    u32 timeStamp1, timeStamp2;
    TimeSliceSime_GetTimeStamp(&timeStamp1, &timeStamp2);
    DUMPD(timeStamp1);
    DUMPD(timeStamp2);

    static int i = 0;
    if (i < 3) {
        i++;
        gDemoRemainTime2 = 7000;
    }

    return 0;
}

static int Time_Slice_CB_Demo3(void)
{
    printf("This is %s()\n", __func__);

    u32 timeStamp1, timeStamp2;
    TimeSliceSime_GetTimeStamp(&timeStamp1, &timeStamp2);
    DUMPD(timeStamp1);
    DUMPD(timeStamp2);

    static int i = 0;
    if (i < 3) {
        i++;
        gDemoRemainTime3 = 0xFFFFFFFF;
    }

    return 0;
}

int TimeSliceSim_Demo2(void)
{
    printf("This is %s()\n", __func__);

    TimeSliceSim_Init(ARRAY_AND_SIZE(gDemoTable));
    TimeSliceSim_Start();
    //Test();

    return 0;
}

Time_Slice_Descriptor2 *g_descriptor2_list = NULL;
Time_Slice_Descriptor2_Mgr g_descriptor2_mgr = {
    .pre_cb = NULL,
    .post_cb = NULL};


int TimeSliceSim_Init2(Time_Slice_Descriptor2 *descriptor_list)
{
    BASIC_ASSERT(descriptor_list != NULL);
    g_descriptor2_list = descriptor_list;
    return 0;
}

int TimeSliceSim_Mgr_Init2(Time_Slice_CB in_pre_cb, Time_Slice_CB in_post_cb)
{
    g_descriptor2_mgr.pre_cb = in_pre_cb;
    g_descriptor2_mgr.post_cb = in_post_cb;
    return 0;
}

int TimeSliceSim_Start2(void)
{
#define CURR_REMAIN_TIME (pCurrDescriptor->remain_time)
#define CURR_CALLBACK (pCurrDescriptor->times_up_cb)

    int is_there_remain_waitings;
    int is_pre_cb_executed;
    int need_post_cb;
    int i;//for debug dump
    int ret;
    u32 min_remain_time;
    Time_Slice_Descriptor2 *pCurrDescriptor;

    BASIC_ASSERT(g_descriptor2_list != NULL);

    do {
        i = 0; //for debug dump
        is_pre_cb_executed = 0;
        need_post_cb = 0;
        for (pCurrDescriptor = g_descriptor2_list; pCurrDescriptor != NULL; pCurrDescriptor = (Time_Slice_Descriptor2 *)pCurrDescriptor->next)
        {
            //Check if there are callbacks need execute immediatly.
            if (CURR_REMAIN_TIME == 0 && pCurrDescriptor->state == TIME_SLICE_CB_WAITING)
            {
                if(CURR_CALLBACK == NULL)
                {
                    printf("i = %d\n", i);
                    BASIC_ASSERT(0);
                }
                else
                {
                    if (is_pre_cb_executed == 0) {
                        is_pre_cb_executed = 1;
                        if (g_descriptor2_mgr.pre_cb != NULL) {
                            ret = (*(g_descriptor2_mgr.pre_cb))();
                        }
                    }
                    need_post_cb = 1;
                    do {
                        pCurrDescriptor->state = TIME_SLICE_CB_DONE;
                        ret = (*CURR_CALLBACK)();
                    } while (CURR_REMAIN_TIME == 0 && pCurrDescriptor->state == TIME_SLICE_CB_WAITING); //if zero remain been setting & setting
                }
            }
            i++; //for debug dump
        }
        if (need_post_cb) {
            need_post_cb = 0;
            if (g_descriptor2_mgr.post_cb != NULL) {
                ret = (*(g_descriptor2_mgr.post_cb))();
            }
        }

        is_there_remain_waitings = 0;
        min_remain_time = 0xFFFFFFFF; //Find minimun remain time
        for (pCurrDescriptor = g_descriptor2_list; pCurrDescriptor != NULL; pCurrDescriptor = (Time_Slice_Descriptor2 *)pCurrDescriptor->next)
        {
            if (/*CURR_REMAIN_TIME != 0 && */pCurrDescriptor->state == TIME_SLICE_CB_WAITING)
            {
                is_there_remain_waitings = 1;
                if (CURR_REMAIN_TIME < min_remain_time)
                {
                    min_remain_time = CURR_REMAIN_TIME;
                }
            }
        }

        if (is_there_remain_waitings) //update all remain time in list
        {
            for (pCurrDescriptor = g_descriptor2_list; pCurrDescriptor != NULL; pCurrDescriptor = (Time_Slice_Descriptor2 *)pCurrDescriptor->next)
            {
                if (pCurrDescriptor->state == TIME_SLICE_CB_WAITING) {
                    CURR_REMAIN_TIME -= min_remain_time;
                }
            }
            TimeSliceSime_UpdateTimeStamp(min_remain_time);
        }
    } while(is_there_remain_waitings);

    return ret;
#undef CURR_REMAIN_TIME
#undef CURR_CALLBACK
}

static int Time_Slice_CB_DemoA(void);
static int Time_Slice_CB_DemoB(void);
static Time_Slice_Descriptor2 gDescriptor2_A = {0, Time_Slice_CB_DemoA, TIME_SLICE_CB_INVALID, NULL};
static Time_Slice_Descriptor2 gDescriptor2_B = {0, Time_Slice_CB_DemoB, TIME_SLICE_CB_INVALID, NULL};

static int Time_Slice_CB_DemoA(void)
{
    printf("This is %s()\n", __func__);

    u32 timeStamp1, timeStamp2;
    TimeSliceSime_GetTimeStamp(&timeStamp1, &timeStamp2);
    DUMPD(timeStamp1);
    DUMPD(timeStamp2);

    static int i = 0;
    if (i < 3) {
        i++;
        gDescriptor2_A.remain_time = 0;
        gDescriptor2_A.state = TIME_SLICE_CB_WAITING;
    }
    else if (i < 5) {
        i++;
        gDescriptor2_A.remain_time = 100;
        gDescriptor2_A.state = TIME_SLICE_CB_WAITING;
    }

    return 0;
}

static int Time_Slice_CB_DemoB(void)
{
    printf("This is %s()\n", __func__);

    u32 timeStamp1, timeStamp2;
    TimeSliceSime_GetTimeStamp(&timeStamp1, &timeStamp2);
    DUMPD(timeStamp1);
    DUMPD(timeStamp2);

    static int i = 0;
    if (i < 6) {
        i++;
        gDescriptor2_B.remain_time = 50;
        gDescriptor2_B.state = TIME_SLICE_CB_WAITING;
    }

    return 0;
}

static int Time_Slice_CB_PRE(void)
{
    printf("This is %s()\n", __func__);
    return 0;
}

static int Time_Slice_CB_POST(void)
{
    printf("This is %s()\n", __func__);
    return 0;
}

int TimeSliceSim_Demo3(void)
{
    printf("\nThis is %s()\n", __func__);
    TimeSliceSime_SetTimeStamp(0, 0);

    gDescriptor2_A.next = (void *)&gDescriptor2_B;
    gDescriptor2_A.remain_time = 200;
    gDescriptor2_A.state = TIME_SLICE_CB_WAITING;
    gDescriptor2_B.remain_time = 0;
    gDescriptor2_B.state = TIME_SLICE_CB_WAITING;

    TimeSliceSim_Init2(&gDescriptor2_A);
    TimeSliceSim_Mgr_Init2(Time_Slice_CB_PRE, Time_Slice_CB_POST);
    TimeSliceSim_Start2();

    return 0;
}

