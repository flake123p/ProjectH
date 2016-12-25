//#include "stdafx.h" //For porting to "Visual Studio"
#include <stdio.h>
#include <time.h>
#include "LibTime.hpp"

clock_t gClock = 0;
void Time_StartClock(void)
{
	gClock = clock();
}

clock_t Time_StopClock(void)
{
	gClock = clock() - gClock;
	return gClock;
}

double Time_CalculateClock(void)
{
	return ((double)gClock)/CLOCKS_PER_SEC;
}

void Time_StopClock_ShowResult(void)
{
	gClock = clock() - gClock;
	printf ("It took me %d clicks (%f seconds).", (int)gClock, ((double)gClock)/CLOCKS_PER_SEC);
}

void Time_DemoClockTimer(void)
{
	Time_StartClock();

	int i;
	for (i = 0; i<0x1FFFFFFF; i++) ;

	Time_StopClock_ShowResult();
	printf("\n");
}


void Time_PrintUtcTime(void)
{
    time_t t = time(NULL);
	
    printf("%s", asctime(gmtime(&t)));
}

void Time_PrintLocalTime(void)
{
    time_t t = time(NULL);

    printf("%s", asctime(localtime(&t)));
}
