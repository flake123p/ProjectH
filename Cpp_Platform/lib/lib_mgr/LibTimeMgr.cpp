//#include "stdafx.h" //For porting to "Visual Studio"
#include <stdio.h>
#include <time.h>

#include "LibTime.hpp"
#include "My_Basics.hpp"

clock_t gClock = 0;
void LibTime_StartClock(void)
{
	gClock = clock();
}

clock_t LibTime_StopClock(void)
{
	gClock = clock() - gClock;
	return gClock;
}

double LibTime_CalculateClock(void)
{
	return ((double)gClock)/CLOCKS_PER_SEC;
}

void LibTime_StopClock_ShowResult(void)
{
	gClock = clock() - gClock;
	printf ("It took me %d clicks (%f seconds).", (int)gClock, ((double)gClock)/CLOCKS_PER_SEC);
}

void LibTime_DemoClockTimer(void)
{
	LibTime_StartClock();

	int i;
	for (i = 0; i<0x1FFFFFFF; i++) ;

	LibTime_StopClock_ShowResult();
	printf("\n");
}


void LibTime_PrintUtcTime(void)
{
    time_t t = time(NULL);
	
    printf("%s", asctime(gmtime(&t)));
}

void LibTime_PrintLocalTime(void)
{
    time_t t = time(NULL);

    printf("%s", asctime(localtime(&t)));
}
