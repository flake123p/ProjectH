
#include <stdio.h>
#include <string.h>

#include "My_Basics.hpp"

#include "LibUart.hpp"
#include "LibTime.hpp"
#include "LibFileIO.hpp"
#include "LibUtility.hpp"

int main(void)
{
	//Time_StartClock();
	//LibUartMgr_DemoTxRx();
	//Time_StopClock_ShowResult();
	Time_StartClock();
	Time_StartMicroSecondClock();
	
	Time_StopClock_ShowResult();
	printf("\n");
	Time_StopMicroSecondClock_ShowResult();
	printf("\n");
	

	DUMPD(Util_GetRand());
	Util_Print_RAND_MAX();
	//Demo_Output_A_File_Cpp_Lite();
}
