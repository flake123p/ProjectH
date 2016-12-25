
#include <stdio.h>
#include <string.h>

#include "LibUart.hpp"
#include "LibTime.hpp"
#include "LibFileIO.hpp"

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
	
	Time_PrintLocalTime();

	//Demo_Output_A_File_Cpp_Lite();
}
