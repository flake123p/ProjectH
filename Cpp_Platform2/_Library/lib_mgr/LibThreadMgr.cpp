
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
	LibOs_SleepMiliSeconds(10);
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

	LibThread_WaitThreads(threadHdlAry, SIZE_OF_ARRAY(threadHdlAry));
	//LibThread_WaitThread(threadHdlAry[0]);
	//LibThread_WaitThread(threadHdlAry[1]);
	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;

	//DUMPD((u32)threadHdlAry[0]);
	//DUMPD((u32)threadHdlAry[1]);
	
	retVal = LibThread_ReleaseHandle(threadHdlAry[0]);
	BASIC_ASSERT(retVal == 0);
	retVal = LibThread_ReleaseHandle(threadHdlAry[1]);
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

	if ( LibThread_ReleaseHandle(threadHdl) ) {
		BASIC_ASSERT(0);
	}
}