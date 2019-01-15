
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

