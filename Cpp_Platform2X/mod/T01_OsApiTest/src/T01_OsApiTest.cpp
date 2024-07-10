
#include "Everything_Lib_Mgr.hpp"
#include "T01_OsApiTest.hpp"

EVENT_HANDLE_t  gEventAry[4];
THREAD_HANDLE_t gThreadAry[4];

u32 gGlobalTicks = 0;
//#define LINUX_SET_EVENT_WORKAROUND LibOs_SleepMiliSeconds(1)
#define LINUX_SET_EVENT_WORKAROUND

int gT01_01_Stop = 0;
void *T01_01_TestThread00(void *arg)
{
	while (1) {
		LibIPC_Event_Wait(gEventAry[0]);
		PRINT_FUNC;
		LINUX_SET_EVENT_WORKAROUND;
		LibIPC_Event_Set(gEventAry[1]);
		if (gT01_01_Stop)
			break;
	}

	return 0;
}

void *T01_01_TestThread01(void *arg)
{
	while (1) {
		LibIPC_Event_Wait(gEventAry[1]);
		PRINT_FUNC;
		LINUX_SET_EVENT_WORKAROUND;
		LibIPC_Event_Set(gEventAry[2]);
		if (gT01_01_Stop)
			break;
	}

	return 0;
}

void *T01_01_TestThread02(void *arg)
{
	while (1) {
		LibIPC_Event_Wait(gEventAry[2]);
		PRINT_FUNC;
		LINUX_SET_EVENT_WORKAROUND;
		LibIPC_Event_Set(gEventAry[3]);
		if (gT01_01_Stop)
			break;
	}

	return 0;
}

void *T01_01_TestThread03(void *arg)
{
	while (1) {
		LibIPC_Event_Wait(gEventAry[3]);
		PRINT_FUNC;
		if (gT01_01_Stop)
			break;
		{
		    static int ctr = 0;
		    ctr++;
		    if (ctr == 3)
		        gT01_01_Stop = 1;
		}
		LINUX_SET_EVENT_WORKAROUND;
		LibIPC_Event_Set(gEventAry[0]);
	}

	return 0;
}

void T01_01_LinuxEventTest(void)
{
	int retVal;


	retVal = LibIPC_Event_BatchCreate(gEventAry, GET_ARRAY_LEN(gEventAry));
	ASSERT_IF(retVal);


    ThreadEntryFunc funcAry[] = {
        T01_01_TestThread00,
        T01_01_TestThread01,
        T01_01_TestThread02,
        T01_01_TestThread03,
    };
    LibThreadMgr_BatchCreate(gThreadAry, funcAry, GET_ARRAY_LEN(gThreadAry));

	LibOs_SleepMiliSeconds(10); // For linux, prevent SetEvent() is running before WaitEvent() !!
	LibIPC_Event_Set(gEventAry[0]);

	LibTime_StartMicroSecondClock();
	retVal = LibThreadMgr_BatchWait(gThreadAry, GET_ARRAY_LEN(gThreadAry));
	ASSERT_IF(retVal);
	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;

	retVal = LibIPC_Event_BatchDestroy(gEventAry, GET_ARRAY_LEN(gEventAry));
	ASSERT_IF(retVal);

	retVal = LibThreadMgr_BatchDestroy(gThreadAry, GET_ARRAY_LEN(gThreadAry));
	ASSERT_IF(retVal);

	printf("%s() ... [Success]\n", __func__);
}

int T01_OsApiTest(void)
{
	printf("This is %s()\n", __func__);

	T01_01_LinuxEventTest();

	return 0;
}
