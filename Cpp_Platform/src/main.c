
#include <stdio.h>
#include <string.h>
#include "LibUart.hpp"
#include "LibTime.hpp"

int main(void)
{
	//Time_StartClock();
	//LibUartMgr_DemoTxRx();
	//Time_StopClock_ShowResult();
	Time_StartClock();
	Time_StartMicroSecondClock();
	Time_PrintLocalTime();
	Time_StopClock_ShowResult();
	Time_StopMicroSecondClock_ShowResult();
}
