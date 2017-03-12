
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

	//LibTuiMgr_DemoDumpAttr();
	//_LibMenu_Demo();
	//LibBuffer_DemoBasic();

	return 1;
	u8 var = 0x0;
	if(var&0x1 == 0x0) {
		printf("TRUE\n");
	} else {
		printf("FALSE\n");
	}

	if(FLG_CHK(var, 0x1) == 0x0) {
		printf("TRUE\n");
	} else {
		printf("FALSE\n");
	}

	if(NOT(CHECK_BIT(var, 0))) {
		printf("TRUE\n");
	} else {
		printf("FALSE\n");
	}

	if(NOT(CHECK_BIT(var, BIT_POSITION_0))) {
		printf("TRUE\n");
	} else {
		printf("FALSE\n");
	}

	// NBB_Reg.AAAAAA.Gbyte.LLbyte = 0x03;
	// NBB_Reg.AAAAAA.Gbyte.LLbyte = (BIT0 | BIT1);
	// FLG_ADD(NBB_Reg.AAAAAA.Gbyte.LLbyte, BIT0 | BIT1);
	
	//LibThreadMgr_Demo();
	
	//LibScreenLogic_Demo();
	return 0;//LibSharedMemory_Demo_Server();
	//return LibSharedMemory_Demo_Client();
}
