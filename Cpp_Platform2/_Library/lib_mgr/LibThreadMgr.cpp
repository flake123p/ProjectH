
#include "Everything_Lib_Mgr.hpp"

//int gTestData_1 = 123;
int gTestData = 5;

void *Test_Thread_1(void *arg)
{
	/*
		May print "567" or "577" or "a mass".
	*/
	printf("%s() 1 ... (%d)\n", __func__, gTestData);
	LibOs_SleepMiliSeconds(10);
	printf("%s() 2 ... (%d)\n", __func__, gTestData);
	LibOs_SleepMiliSeconds(100);
	printf("%s() 3 ... (%d)\n", __func__, gTestData);


	return 0;
}

void *Test_Thread_2(void *arg)
{
	printf("%s() 1 ... (%d)\n", __func__, gTestData);
	LibOs_SleepMiliSeconds(100);
	printf("%s() 2 ... (%d)\n", __func__, gTestData);
	LibOs_SleepMiliSeconds(400);
	
	return 0;
}

void LibThreadMgr_Demo(void)
{
	THREAD_HANDLE_t threadHdlAry[2] = {0};
	int retVal;

	LibTime_StartMicroSecondClock();
	
	retVal = LibThread_NewHandle(&threadHdlAry[0]);
	BASIC_ASSERT(retVal == 0);
	retVal = LibThread_NewHandle(&threadHdlAry[1]);
	BASIC_ASSERT(retVal == 0);
	
	retVal = LibThread_Create(threadHdlAry[0], Test_Thread_1);
	BASIC_ASSERT(retVal == 0);
	retVal = LibThread_Create(threadHdlAry[1], Test_Thread_2);
	BASIC_ASSERT(retVal == 0);

	
	printf("%s() 1\n", __func__);gTestData++;
	LibOs_SleepMiliSeconds(10);
	printf("%s() 2\n", __func__);gTestData++;
	LibOs_SleepMiliSeconds(100);
	printf("%s() 3\n", __func__);gTestData++;

	//LibThread_WaitThreads(threadHdlAry, SIZE_OF_ARRAY(threadHdlAry));
	LibThread_WaitThread(threadHdlAry[0]);
	LibThread_WaitThread(threadHdlAry[1]);
	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;

	//DUMPD((u32)threadHdlAry[0]);
	//DUMPD((u32)threadHdlAry[1]);
	
	retVal = LibThread_DestroyHandle(threadHdlAry[0]);
	BASIC_ASSERT(retVal == 0);
	retVal = LibThread_DestroyHandle(threadHdlAry[1]);
	BASIC_ASSERT(retVal == 0);

	REMOVE_UNUSED_WRANING(retVal);
}

void LibThreadMgr_DemoLite(void)
{
	THREAD_HANDLE_t threadHdl;

	if ( LibThread_NewHandle(&threadHdl) ) {
		BASIC_ASSERT(0);
	}

	if ( LibThread_Create(threadHdl, Test_Thread_1) ) {
		BASIC_ASSERT(0);
	}

	LibTime_StartMicroSecondClock();
	LibThread_WaitThread(threadHdl);
	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;

	if ( LibThread_DestroyHandle(threadHdl) ) {
		BASIC_ASSERT(0);
	}
}

EVENT_HANDLE_t gEvent_A;
EVENT_HANDLE_t gEvent_B;
void *Test_Thread_A(void *arg)
{
	LibIPC_Event_Wait(gEvent_A);
	
	for (u32 i=0; i<10; i++) {
		PRINT_FUNC;
		LibOs_SleepMiliSeconds(1);
	}

	LibIPC_Event_Set(gEvent_B);
	LibIPC_Event_Wait(gEvent_A);

	for (u32 i=0; i<10; i++) {
		PRINT_FUNC;
		LibOs_SleepMiliSeconds(1);
	}

	LibIPC_Event_Set(gEvent_B);
	
	return 0;
}

void *Test_Thread_B(void *arg)
{
	LibIPC_Event_Wait(gEvent_B);
	
	for (u32 i=0; i<10; i++) {
		PRINT_FUNC;
		LibOs_SleepMiliSeconds(1);
	}

	LibIPC_Event_Set(gEvent_A);
	LibIPC_Event_Wait(gEvent_B);

	for (u32 i=0; i<10; i++) {
		PRINT_FUNC;
		LibOs_SleepMiliSeconds(1);
	}

	return 0;
}

void LibThreadMgr_DemoEvent(void)
{
	THREAD_HANDLE_t threadHdl_A;
	THREAD_HANDLE_t threadHdl_B;

	int retVal;

	retVal = LibThread_NewHandle(&threadHdl_A);
	ASSERT_IF(retVal);
	retVal = LibThread_NewHandle(&threadHdl_B);
	ASSERT_IF(retVal);

	retVal = LibIPC_Event_Create(&gEvent_A);
	ASSERT_IF(retVal);
	retVal = LibIPC_Event_Create(&gEvent_B);
	ASSERT_IF(retVal);

	retVal = LibThread_Create(threadHdl_A, Test_Thread_A);
	ASSERT_IF(retVal);
	retVal = LibThread_Create(threadHdl_B, Test_Thread_B);
	ASSERT_IF(retVal);

	LibOs_SleepMiliSeconds(10); // For linux, prevent SetEvent() is running before WaitEvent() !!
	LibIPC_Event_Set(gEvent_A);
	
	LibTime_StartMicroSecondClock();
	LibThread_WaitThread(threadHdl_A);
	LibThread_WaitThread(threadHdl_B);
	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;

	retVal = LibIPC_Event_Destroy(gEvent_A);
	ASSERT_IF(retVal);
	retVal = LibIPC_Event_Destroy(gEvent_B);
	ASSERT_IF(retVal);

	retVal = LibThread_DestroyHandle(threadHdl_A);
	ASSERT_IF(retVal);
	retVal = LibThread_DestroyHandle(threadHdl_B);
	ASSERT_IF(retVal);
}

void *Test_Thread_A_Priority(void *arg)
{
	LibIPC_Event_Wait(gEvent_A);

	LibIPC_Event_Set(gEvent_B);

	for (u32 i=0; i<1000000000; i++) {
		;
	}
	printf("A\n");

	return 0;
}

void *Test_Thread_B_Priority(void *arg)
{
	LibIPC_Event_Wait(gEvent_B);
	
	for (u32 i=0; i<1000000000; i++) {
		;
	}
	printf("B\n");

	return 0;
}

void LibThreadMgr_DemoPriority(void)
{
	THREAD_HANDLE_t threadHdl_A;
	THREAD_HANDLE_t threadHdl_B;

	int retVal;

	retVal = LibThread_NewHandle(&threadHdl_A, TPRI_HH);
	ASSERT_IF(retVal);
	retVal = LibThread_NewHandle(&threadHdl_B, TPRI_LL);
	ASSERT_IF(retVal);

	retVal = LibIPC_Event_Create(&gEvent_A);
	ASSERT_IF(retVal);
	retVal = LibIPC_Event_Create(&gEvent_B);
	ASSERT_IF(retVal);

	retVal = LibThread_Create(threadHdl_A, Test_Thread_A_Priority);
	ASSERT_IF(retVal);
	retVal = LibThread_Create(threadHdl_B, Test_Thread_B_Priority);
	ASSERT_IF(retVal);

	LibOs_SleepMiliSeconds(10); // For linux, prevent SetEvent() is running before WaitEvent() !!
	LibIPC_Event_Set(gEvent_A);
	
	LibTime_StartMicroSecondClock();
	LibThread_WaitThread(threadHdl_A);
	LibThread_WaitThread(threadHdl_B);
	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;

	retVal = LibIPC_Event_Destroy(gEvent_A);
	ASSERT_IF(retVal);
	retVal = LibIPC_Event_Destroy(gEvent_B);
	ASSERT_IF(retVal);

	retVal = LibThread_DestroyHandle(threadHdl_A);
	ASSERT_IF(retVal);
	retVal = LibThread_DestroyHandle(threadHdl_B);
	ASSERT_IF(retVal);
}

MUTEX_HANDLE_t gTextMutexHdl;

void Test_Mutex_Print(int base)
{
// if no mutex, print result is chaos.
#define MUTEX_ENABLE ( 1 )

#if MUTEX_ENABLE
	LibIPC_Mutex_Lock(gTextMutexHdl);
#endif

	for (int i = base; i < base+10; i++ ) {
		printf("%s() %d\n", __func__, i);
		LibOs_SleepMicroSeconds(1); // For linux, prevent printing isn't chaos!!
	}

#if MUTEX_ENABLE
	int retVal;
	ASSERT_CHK( retVal, LibIPC_Mutex_Unlock(gTextMutexHdl) );
#endif
}

void *Test_Thread_C(void *arg)
{
	Test_Mutex_Print(0);
	
	return 0;
}

void *Test_Thread_D(void *arg)
{
	Test_Mutex_Print(100);
	
	return 0;
}

void LibThreadMgr_DemoMutex(void)
{
	THREAD_HANDLE_t threadHdl_C;
	THREAD_HANDLE_t threadHdl_D;

	int retVal;

	ASSERT_CHK( retVal, LibThread_NewHandle(&threadHdl_C) );
	ASSERT_CHK( retVal, LibThread_NewHandle(&threadHdl_D) );

	ASSERT_CHK( retVal, LibIPC_Mutex_Create(&gTextMutexHdl) );
	
	ASSERT_CHK( retVal, LibThread_Create(threadHdl_C, Test_Thread_C) );
	ASSERT_CHK( retVal, LibThread_Create(threadHdl_D, Test_Thread_D) );

	LibOs_SleepMiliSeconds(10); // For linux, prevent SetEvent() is running before WaitEvent() !!

	LibTime_StartMicroSecondClock();
	LibThread_WaitThread(threadHdl_C);
	LibThread_WaitThread(threadHdl_D);
	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;

	LibIPC_Mutex_Lock(gTextMutexHdl);
	LibIPC_Mutex_Unlock(gTextMutexHdl);
	ASSERT_CHK( retVal, LibIPC_Mutex_Destroy(gTextMutexHdl) );

	ASSERT_CHK( retVal, LibThread_DestroyHandle(threadHdl_C) );
	ASSERT_CHK( retVal, LibThread_DestroyHandle(threadHdl_D) );
}

THREAD_HANDLE_t gThreadHdl_WatchDog;
MUTEX_HANDLE_t gMutexHdl_WatchDog;
//EVENT_HANDLE_t gEvent_CountDownTimer;
typedef enum {
	WATCH_DOG_NULL,
	WATCH_DOG_START,
	WATCH_DOG_TOUCH,
	WATCH_DOG_FREEZE,
	WATCH_DOG_UNINIT,
}WATCH_DOG_t;
WATCH_DOG_t gCountDownState = WATCH_DOG_NULL;
u32 gSleepMiliSec = 0;
WatchDogTimeOutFunc gTimeOutCB = NULL;
static void _WatchDog_SetState(WATCH_DOG_t state)
{
	int retVal;
	
	LibIPC_Mutex_Lock(gMutexHdl_WatchDog);

	gCountDownState = state;

	ASSERT_CHK( retVal, LibIPC_Mutex_Unlock(gMutexHdl_WatchDog) );
}

static WATCH_DOG_t _WatchDog_GetState(void)
{
	int retVal;
	WATCH_DOG_t state;
	
	LibIPC_Mutex_Lock(gMutexHdl_WatchDog);

	state = gCountDownState;

	ASSERT_CHK( retVal, LibIPC_Mutex_Unlock(gMutexHdl_WatchDog) );

	return state;
}

void *WatchDog_Thread(void *arg)
{
#define SLEEP_DURATION_MS (100)

	u32 ctrMax = gSleepMiliSec / SLEEP_DURATION_MS;
	u32 ctr = 0;
	while (1) {
		LibOs_SleepMiliSeconds(SLEEP_DURATION_MS);
		switch (_WatchDog_GetState()) {
			case WATCH_DOG_START: {
				ctr++;
				if (ctr == ctrMax) {
					ctr = 0;
					if (gTimeOutCB != NULL) {
						(*gTimeOutCB)();
						return 0;
					} else {
						return 0;
					}
				}
			} break;

			case WATCH_DOG_TOUCH: {
				ctr = 0;
				_WatchDog_SetState(WATCH_DOG_START);
			} break;

			case WATCH_DOG_FREEZE: {
			} break;

			case WATCH_DOG_UNINIT: {
				return 0;
			} break;

			default:
				BASIC_ASSERT(0);
				break;
		}
	}
	
	return 0;
}
int LibThreadMgr_Init_WatchDog(u32 sleepMiliSec, WatchDogTimeOutFunc cb /* = NULL */)
{
	int retVal;

	gSleepMiliSec = sleepMiliSec;
	gTimeOutCB = cb;

	ASSERT_CHK( retVal, LibIPC_Mutex_Create(&gMutexHdl_WatchDog) );

	_WatchDog_SetState(WATCH_DOG_START);
	
	ASSERT_CHK( retVal, LibThread_NewHandle(&gThreadHdl_WatchDog) );
	ASSERT_CHK( retVal, LibThread_Create(gThreadHdl_WatchDog, WatchDog_Thread) );
	
	return 0;
}

int LibThreadMgr_Uninit_WatchDog(void)
{
	int retVal;
	
	//LibOs_SleepMiliSeconds(10); // For linux, prevent SetEvent() is running before WaitEvent() !!

	_WatchDog_SetState(WATCH_DOG_UNINIT);
	
	LibThread_WaitThread(gThreadHdl_WatchDog);

	LibIPC_Mutex_Lock(gMutexHdl_WatchDog);
	LibIPC_Mutex_Unlock(gMutexHdl_WatchDog);
	ASSERT_CHK( retVal, LibIPC_Mutex_Destroy(gMutexHdl_WatchDog) );

	ASSERT_CHK( retVal, LibThread_DestroyHandle(gThreadHdl_WatchDog) );
	
	return 0;
}

int LibThreadMgr_Touch_WatchDog(void)
{
	_WatchDog_SetState(WATCH_DOG_TOUCH);
	
	return 0;
}

int LibThreadMgr_Freeze_WatchDog(void)
{
	_WatchDog_SetState(WATCH_DOG_FREEZE);
	
	return 0;
}

int LibThreadMgr_Unfreeze_WatchDog(void)
{
	_WatchDog_SetState(WATCH_DOG_TOUCH);
	
	return 0;
}

void LibThreadMgr_Demo_WatchDog_CB(void)
{
	printf("Watch Dog Times Out!!\n");
	EXIT_LOC_IF(1);
}

void LibThreadMgr_Demo_WatchDog(void)
{
	LibTime_StartMicroSecondClock();
	LibThreadMgr_Init_WatchDog(500, LibThreadMgr_Demo_WatchDog_CB);
	for (u32 i=0; i<30; i++) {
		LibOs_SleepMiliSeconds(100);
		LibThreadMgr_Touch_WatchDog();
	}
	
	LibThreadMgr_Uninit_WatchDog();
	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;
}

//
//
//
int LibThreadMgr_BatchCreate(THREAD_HANDLE_t *hdlAry, ThreadEntryFunc *funcAry, u32 len)
{
    int retVal;
    u32 i = 0;
    for (i = 0; i < len; i++) {
        retVal = LibThread_NewHandle(&hdlAry[i]);
        RETURN_IF(retVal);
    }

    for (i = 0; i < len; i++) {
        retVal = LibThread_Create(hdlAry[i], funcAry[i]);
        RETURN_IF(retVal);
    }

    return 0;
}

int LibThreadMgr_BatchWait(THREAD_HANDLE_t *hdlAry, u32 len)
{
    int retVal;
    u32 i = 0;
    for (i = 0; i < len; i++) {
        retVal = LibThread_WaitThread(hdlAry[i]);
        RETURN_IF(retVal);
    }

    return 0;
}

int LibThreadMgr_BatchDestroy(THREAD_HANDLE_t *hdlAry, u32 len)
{
    int retVal;
    u32 i = 0;
    for (i = 0; i < len; i++) {
        retVal = LibThread_DestroyHandle(hdlAry[i]);
        RETURN_IF(retVal);
    }

    return 0;
}

int LibIPC_Event_BatchCreate(EVENT_HANDLE_t *eventHdlAry, u32 len)
{
    int retVal;
    u32 i = 0;
    for (i = 0; i < len; i++) {
        retVal = LibIPC_Event_Create(&eventHdlAry[i]);
        RETURN_IF(retVal);
    }

    return 0;
}

int LibIPC_Event_BatchDestroy(EVENT_HANDLE_t *eventHdlAry, u32 len)
{
    int retVal;
    u32 i = 0;
    for (i = 0; i < len; i++) {
        retVal = LibIPC_Event_Destroy(eventHdlAry[i]);
        RETURN_IF(retVal);
    }

    return 0;
}

