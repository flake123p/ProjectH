
#include "Everything_SimTimeSlice.hpp"
#include "SimTimeSlice.hpp"
#include <vector> //for ver2

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

static void Test(void)
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

int SimTimeSlice_Demo_Old(void)
{
	printf("This is %s()\n", __func__);

	Test();

	return 0;
}

SimTimeSlice_TimeAdd_CB_t gSimTimeSlice_TimeAdd_CB = NULL;

static Time_Slice_Descriptor *g_descriptor_table = NULL;
static u32 g_table_number = 0;
static u32 gTimeStampHigh = 0;
static u32 gTimeStampLow = 0;
static void SimTimeSlice_UpdateTimeStamp(u32 inTimeStamp)
{
    //if overflow
    if (gTimeStampLow + inTimeStamp < gTimeStampLow) {
        gTimeStampHigh++;
    }
    else if (gTimeStampLow + inTimeStamp < inTimeStamp) {
        gTimeStampHigh++;
    }

    gTimeStampLow += inTimeStamp;
    if (gSimTimeSlice_TimeAdd_CB != NULL) {
        (*gSimTimeSlice_TimeAdd_CB)(inTimeStamp);
    }
}

u32 SimTimeSlice_TimeStamp_Low_Get(void)
{
    return gTimeStampLow;
}

u32 SimTimeSlice_TimeStamp_High_Get(void)
{
    return gTimeStampHigh;
}

void SimTimeSlice_TimeStampGet(u32 *outTimeStamp1, u32 *outTimeStamp2)
{
    *outTimeStamp1 = gTimeStampHigh;
    *outTimeStamp2 = gTimeStampLow;
}

void SimTimeSlice_TimeStampSet(u32 inTimeStamp1, u32 inTimeStamp2)
{
    gTimeStampHigh = inTimeStamp1;
    gTimeStampLow = inTimeStamp2;
}

int SimTimeSlice_Init(Time_Slice_Descriptor *descriptor_table, u32 table_number)
{
    BASIC_ASSERT(descriptor_table != NULL);
    BASIC_ASSERT(table_number != 0);

    g_descriptor_table = descriptor_table;
    g_table_number = table_number;

    return 0;
}

int SimTimeSlice_RegisterTimeAddCallback(SimTimeSlice_TimeAdd_CB_t timeAdd_CB)
{
    gSimTimeSlice_TimeAdd_CB = timeAdd_CB;
    return 0;
}

int SimTimeSlice_Start(void)
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
            SimTimeSlice_UpdateTimeStamp(min_remain_time);
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

static int TimeSlice_TestCB_1(void);
static int TimeSlice_TestCB_2(void);
static int TimeSlice_TestCB_3(void);
static u32 gDemoRemainTime1  = 3125;
static u32 gDemoRemainTime2  = 0;
static u32 gDemoRemainTime3  = 6250;
Time_Slice_Descriptor gDemoTable[] =
{
    {&gDemoRemainTime1, TimeSlice_TestCB_1},
    {&gDemoRemainTime2, TimeSlice_TestCB_2},
    {&gDemoRemainTime3, TimeSlice_TestCB_3},
};

static int TimeSlice_TestCB_1(void)
{
    printf("This is %s()\n", __func__);

    u32 timeStamp1, timeStamp2;
    SimTimeSlice_TimeStampGet(&timeStamp1, &timeStamp2);
    DUMPD(timeStamp1);
    DUMPD(timeStamp2);

    return 0;
}

static int TimeSlice_TestCB_2(void)
{
    printf("This is %s()\n", __func__);

    u32 timeStamp1, timeStamp2;
    SimTimeSlice_TimeStampGet(&timeStamp1, &timeStamp2);
    DUMPD(timeStamp1);
    DUMPD(timeStamp2);

    static int i = 0;
    if (i < 3) {
        i++;
        gDemoRemainTime2 = 7000;
    }

    return 0;
}

static int TimeSlice_TestCB_3(void)
{
    printf("This is %s()\n", __func__);

    u32 timeStamp1, timeStamp2;
    SimTimeSlice_TimeStampGet(&timeStamp1, &timeStamp2);
    DUMPD(timeStamp1);
    DUMPD(timeStamp2);

    static int i = 0;
    if (i < 3) {
        i++;
        gDemoRemainTime3 = 0xFFFFFFFF;
    }

    return 0;
}

int SimTimeSlice1_Demo(void)
{
    printf("This is %s()\n", __func__);

    SimTimeSlice_Init(ARRAY_AND_SIZE(gDemoTable));
    SimTimeSlice_Start();
    //Test();

    return 0;
}

static std::vector<Time_Slice_Descriptor2 *> gDescriptor2_vector;
static Time_Slice_Descriptor2 gLastStandaloneDes = {
    .remain_time = 0,
    .state = TIME_SLICE_CB_DONE,
    .times_up_cb = NULL,
    .hdl_to_cb = NULL,
};
static Time_Slice_Descriptor2_Ext g_descriptor2_ext = {
    .pre_cb = NULL,
    .post_cb = NULL
};

Time_Slice_Descriptor2 *SimTimeSlice2_GetLastStandaloneDes(void)
{
    return &gLastStandaloneDes;
}

int SimTimeSlice2_Init_AddDescriptor(Time_Slice_Descriptor2 *p_descriptor)
{
    gDescriptor2_vector.push_back(p_descriptor);
    return 0;
}

int SimTimeSlice2_Init_PrePostCB(Simple_CB_t in_pre_cb, Simple_CB_t in_post_cb)
{
    g_descriptor2_ext.pre_cb = in_pre_cb;
    g_descriptor2_ext.post_cb = in_post_cb;
    return 0;
}

int SimTimeSlice2_Start(void)
{
#define CURR_REMAIN_TIME (pCurrDescriptor->remain_time)
#define CURR_CALLBACK (pCurrDescriptor->times_up_cb)

    int is_there_remain_waitings;
    int is_pre_cb_executed;
    int need_post_cb;
    u32 descriptor_vector_size = gDescriptor2_vector.size();
    int ret;
    u32 min_remain_time;
    Time_Slice_Descriptor2 *pCurrDescriptor;

    do {
        //i = 0; //for debug dump
        is_pre_cb_executed = 0;
        need_post_cb = 0;
        for (u32 i=0; i<descriptor_vector_size; i++)
        {
            pCurrDescriptor = gDescriptor2_vector[i];
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
                        if (g_descriptor2_ext.pre_cb != NULL) {
                            ret = (*(g_descriptor2_ext.pre_cb))();
                        }
                    }
                    need_post_cb = 1;
                    do {
                        pCurrDescriptor->state = TIME_SLICE_CB_DONE;
                        ret = (*CURR_CALLBACK)(pCurrDescriptor->hdl_to_cb);
                    } while (CURR_REMAIN_TIME == 0 && pCurrDescriptor->state == TIME_SLICE_CB_WAITING); //if "zero remain time" been setting & setting
                }
            }
        }
        //New feature start: last standalone descriptor for application
        if (gLastStandaloneDes.remain_time == 0 && gLastStandaloneDes.state == TIME_SLICE_CB_WAITING)
        {
            if (gLastStandaloneDes.times_up_cb != NULL)
            {
                if (is_pre_cb_executed == 0) {
                    is_pre_cb_executed = 1;
                    if (g_descriptor2_ext.pre_cb != NULL) {
                        ret = (*(g_descriptor2_ext.pre_cb))();
                    }
                }
                need_post_cb = 1;
                do {
                    gLastStandaloneDes.state = TIME_SLICE_CB_DONE;
                    ret = (*(gLastStandaloneDes.times_up_cb))(gLastStandaloneDes.hdl_to_cb);
                } while (gLastStandaloneDes.remain_time == 0 && gLastStandaloneDes.state == TIME_SLICE_CB_WAITING); //if "zero remain time" been setting & setting
            }
            else
            {
                BASIC_ASSERT(0);
            }
        }
        //New feature end
        if (need_post_cb) {
            need_post_cb = 0;
            if (g_descriptor2_ext.post_cb != NULL) {
                ret = (*(g_descriptor2_ext.post_cb))();
            }
        }

        is_there_remain_waitings = 0;
        min_remain_time = 0xFFFFFFFF; //Find minimun remain time
        for (u32 i=0; i<descriptor_vector_size; i++)
        {
            pCurrDescriptor = gDescriptor2_vector[i];

            if (/*CURR_REMAIN_TIME != 0 && */pCurrDescriptor->state == TIME_SLICE_CB_WAITING)
            {
                is_there_remain_waitings = 1;
                if (CURR_REMAIN_TIME < min_remain_time)
                {
                    min_remain_time = CURR_REMAIN_TIME;
                }
            }
        }
        //New feature start: last standalone descriptor for application
        if (gLastStandaloneDes.state == TIME_SLICE_CB_WAITING)
        {
            is_there_remain_waitings = 1;
            if (gLastStandaloneDes.remain_time < min_remain_time)
            {
                min_remain_time = gLastStandaloneDes.remain_time;
            }
            gLastStandaloneDes.remain_time -= min_remain_time;
        }
        //New feature end

        if (is_there_remain_waitings) //update all remain time in vector
        {
            for (u32 i=0; i<descriptor_vector_size; i++)
            {
                pCurrDescriptor = gDescriptor2_vector[i];

                if (pCurrDescriptor->state == TIME_SLICE_CB_WAITING) {
                    CURR_REMAIN_TIME -= min_remain_time;
                }
            }
            SimTimeSlice_UpdateTimeStamp(min_remain_time);
        }
    } while(is_there_remain_waitings);

    return ret;
#undef CURR_REMAIN_TIME
#undef CURR_CALLBACK
}

int SimTimeSlice2_Uninit(void)
{
    SimTimeSlice_TimeStampSet(0, 0);

    SimTimeSlice2_Init_PrePostCB(NULL, NULL);

    gLastStandaloneDes.remain_time = 0;
    gLastStandaloneDes.state = TIME_SLICE_CB_DONE;
    gLastStandaloneDes.times_up_cb = NULL;
    gLastStandaloneDes.hdl_to_cb = NULL;

    gDescriptor2_vector.clear();
    return 0;
}

#define ___DEMO___________________________
#define ___DEMO__________________________
#define ___DEMO_________________________

static u32 gCounter[3] = {0};//0 for A, 1 for B, 2 for all
static Time_Slice_Descriptor2 gDescriptor2_A = {0, TIME_SLICE_CB_INVALID, NULL, gCounter};
static Time_Slice_Descriptor2 gDescriptor2_B = {0, TIME_SLICE_CB_INVALID, NULL, gCounter};

static int TimeSlice_TestCB_A(Handle_t hdl)
{
    //printf("%s()\n", __func__);

    u32 timeStamp1, timeStamp2;
    SimTimeSlice_TimeStampGet(&timeStamp1, &timeStamp2);

    u32 *p_ctr = (u32 *)hdl;
    p_ctr[0]++;
    p_ctr[2]++;
    printf("====== AAA Time Stamp:%d/%-5d -- %d/%d/%d\n", timeStamp1, timeStamp2, p_ctr[0], p_ctr[1], p_ctr[2]);


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

static int TimeSlice_TestCB_B(Handle_t hdl)
{
    //printf("%s()\n", __func__);

    u32 timeStamp1, timeStamp2;
    SimTimeSlice_TimeStampGet(&timeStamp1, &timeStamp2);

    u32 *p_ctr = (u32 *)hdl;
    p_ctr[1]++;
    p_ctr[2]++;
    printf("====== BBB Time Stamp:%d/%-5d -- %d/%d/%d\n", timeStamp1, timeStamp2, p_ctr[0], p_ctr[1], p_ctr[2]);


    static int i = 0;
    if (i < 6) {
        i++;
        gDescriptor2_B.remain_time = 50;
        gDescriptor2_B.state = TIME_SLICE_CB_WAITING;
    }

    return 0;
}

static int TimeSlice_TestCB_PRE(void)
{
    printf("%s()\n", __func__);
    return 0;
}

static int TimeSlice_TestCB_POST(void)
{
    printf("%s() -- %d/%d/%d\n\n", __func__, gCounter[0], gCounter[1], gCounter[2]);
    return 0;
}

static Time_Slice_Descriptor2 *gDescriptor2_Last;

static int TimeSlice_TestCB_Last(Handle_t hdl)
{
    u32 timeStamp1, timeStamp2;
    SimTimeSlice_TimeStampGet(&timeStamp1, &timeStamp2);
    printf("====== Last Time Stamp:%d/%-5d\n", timeStamp1, timeStamp2);
    static int i = 0;
    if (i < 6) {
        i++;
        gDescriptor2_Last->remain_time = 50;
        gDescriptor2_Last->state = TIME_SLICE_CB_WAITING;
    }
    return 0;
}

int SimTimeSlice2_Demo(void)
{
    printf("\nThis is %s()\n", __func__);
    SimTimeSlice_TimeStampSet(0, 0);

    gDescriptor2_A.remain_time = 200;
    gDescriptor2_A.state = TIME_SLICE_CB_WAITING;
    gDescriptor2_A.times_up_cb = TimeSlice_TestCB_A;
    gDescriptor2_B.remain_time = 0;
    gDescriptor2_B.state = TIME_SLICE_CB_WAITING;
    gDescriptor2_B.times_up_cb = TimeSlice_TestCB_B;

    gDescriptor2_Last = SimTimeSlice2_GetLastStandaloneDes();
    gDescriptor2_Last->remain_time = 50;
    gDescriptor2_Last->state = TIME_SLICE_CB_WAITING;
    gDescriptor2_Last->times_up_cb = TimeSlice_TestCB_Last;

    SimTimeSlice2_Init_AddDescriptor(&gDescriptor2_A);
    SimTimeSlice2_Init_AddDescriptor(&gDescriptor2_B);
    SimTimeSlice2_Init_PrePostCB(TimeSlice_TestCB_PRE, TimeSlice_TestCB_POST);
    SimTimeSlice2_Start();

    SimTimeSlice2_Uninit();

    return 0;
}

void SimTimeSlice2_Dump(void)
{
    printf("\n=== %s() ===\n", __func__);

    u32 size_of_gDescriptor2_vector = gDescriptor2_vector.size();
    DUMPU(size_of_gDescriptor2_vector);

    for (u32 i=0; i<size_of_gDescriptor2_vector; i++)
    {
        printf("remain_time:%6d, state:%d, times_up_cb:0x%08X, hdl_to_cb:0x%08X\n",
            gDescriptor2_vector[i]->remain_time,
            gDescriptor2_vector[i]->state,
            POINTER_TO_U32(gDescriptor2_vector[i]->times_up_cb),
            POINTER_TO_U32(gDescriptor2_vector[i]->hdl_to_cb));
    }

    for (u32 i=0; i<size_of_gDescriptor2_vector; i++)
    {
        printf("time slice des:0x%08X\n",
            POINTER_TO_U32(gDescriptor2_vector[i]));
    }
}
