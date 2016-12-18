//#include "stdafx.h" //For porting to "Visual Studio"
#include <stdio.h>
#include <time.h>
#include "libTime.hpp"

clock_t gClock = 0;
void Time_StartClock(void)
{
	gClock = clock();
}
void Time_StopClock_ShowResult(void)
{
	gClock = clock() - gClock;
	printf ("It took me %d clicks (%f seconds).\n", (int)gClock, ((double)gClock)/CLOCKS_PER_SEC);
}

void Time_DemoClockTimer(void)
{
	Time_StartClock();

	int i;
	for (i = 0; i<0x0FFFFFFF; i++) ;

	Time_StopClock_ShowResult();
}

