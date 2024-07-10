//#include "stdafx.h" //For porting to "Visual Studio"
#include <windows.h>
#include <stdio.h>
#include <time.h>

#include "LibTime.hpp"
#include "My_Basics.hpp"

LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
LARGE_INTEGER Frequency;

void LibTime_StartMicroSecondClock(void)
{
	QueryPerformanceFrequency(&Frequency); 
	QueryPerformanceCounter(&StartingTime);
}

void LibTime_StopMicroSecondClock_ShowResult(void)
{
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

	//
	// We now have the elapsed number of ticks, along with the
	// number of ticks-per-second. We use these values
	// to convert to the number of elapsed microseconds.
	// To guard against loss-of-precision, we convert
	// to microseconds *before* dividing by ticks-per-second.
	//

	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	printf ("It took me %I64u micro-seconds.", ElapsedMicroseconds.QuadPart);
}
