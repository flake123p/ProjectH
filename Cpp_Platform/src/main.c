
#include <stdio.h>
#include <string.h>
#include "LibUart.h"
#include "libTime.hpp"

int main(void)
{
	Time_StartClock();
	LibUartMgr_DemoTxRx();
	Time_StopClock_ShowResult();
	Time_DemoClockTimer();
}
