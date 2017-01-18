
#include <stdio.h>
#include <string.h>

#include "LibUart.hpp"
#include "LibTime.hpp"
#include "LibFileIO.hpp"
#include "LibUtility.hpp"
#include "LibOS.hpp"
#include "LibTui.hpp"
#include "LibSysCmd.hpp"
#include "My_Basics.hpp"

int main(void)
{
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
}
