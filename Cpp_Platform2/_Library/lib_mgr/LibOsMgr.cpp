
#include "Everything_Lib_Mgr.hpp"

void LibOsMgr_DemoSleep(void)
{
	LibTime_StartMicroSecondClock();
	LibOs_SleepSeconds(1);
	LibTime_StopMicroSecondClock_ShowResult(); PRINT_NEXT_LINE;

	LibTime_StartMicroSecondClock();
	LibOs_SleepMiliSeconds(200);
	LibTime_StopMicroSecondClock_ShowResult(); PRINT_NEXT_LINE;

	LibTime_StartMicroSecondClock();
	LibOs_SleepMicroSeconds(500);
	LibTime_StopMicroSecondClock_ShowResult(); PRINT_NEXT_LINE;
}
