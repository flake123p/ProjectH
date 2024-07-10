
#include "Everything_App.hpp"

int main(int argc, char *argv[])
{
	#if 0
	//LibTime_StartClock();
	//LibUartMgr_DemoTxRx();
	//LibTime_StopClock_ShowResult();
	LibTime_StartClock();
	LibTime_StartMicroSecondClock();
	//LibOs_SleepMiliSeconds(500);
	//LibOs_SleepSeconds(1);

	LibTuiMgr_DemoSimple();
	printf("press any key to next run\n");
	printf("press any key to next run\n");
	LibSysCmd_Pause();
	LibTuiMgr_DemoSimple();
	
	LibTime_StopClock_ShowResult();
	printf("\n");
	LibTime_StopMicroSecondClock_ShowResult();
	printf("\n");
	

	//DUMPD(LibUtil_GetRand());
	//LibUtil_Print_RAND_MAX();
	//Demo_Output_A_File_Cpp_Lite();
	#else
		#if 0
		u8 ch = 0x6F;
		int i = CHECK_BIT_FIELD(ch, BIT_0|BIT_1, SIZE_2, OFFSET_5);
		DUMPD(i);
		i = CLEAR_BIT_FIELD(ch, SIZE_2, OFFSET_5);
		DUMPD(i);
		i = SET_BIT_FIELD(ch, BIT_0|BIT_1, SIZE_2, OFFSET_6);
		DUMPX(i);
		ch = 0x80;
		i = GET_MSB(ch, u8);
		DUMPX(i);
		i = BIT_0|BIT_1;
		DUMPX(i);
		i = BIT_0|BIT_1<<1;
		DUMPX(i);
		#endif
	#endif

	//LibScreenLogic_Demo();
	return LibSharedMemory_Demo_Server();
	//return LibSharedMemory_Demo_Client();
}
