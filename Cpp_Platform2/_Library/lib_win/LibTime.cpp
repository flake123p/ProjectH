//#include "stdafx.h" //For porting to "Visual Studio"
#include <windows.h>
#include <stdio.h>
#include <time.h>

#include "_LibMT.hpp"
#include "LibTime.hpp"
#include "My_Basics.hpp"

static int gLibTimeLinux_IsStarted = 0;

LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
LARGE_INTEGER Frequency;

void LibTime_StartMicroSecondClock(void)
{
    MUTEX_LIB_TIME_LOCK;
    BASIC_ASSERT(gLibTimeLinux_IsStarted == 0);
    gLibTimeLinux_IsStarted = 1;

    QueryPerformanceFrequency(&Frequency); 
    QueryPerformanceCounter(&StartingTime);
    MUTEX_LIB_TIME_UNLOCK;
}

void LibTime_StopMicroSecondClock_ShowResult(int do_print_nextline /* = 0 */)
{
    MUTEX_LIB_TIME_LOCK;
    BASIC_ASSERT(gLibTimeLinux_IsStarted == 1);
    gLibTimeLinux_IsStarted = 0;

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

    if (do_print_nextline) {
        printf ("It took me %I64u micro-seconds.\n", ElapsedMicroseconds.QuadPart);
    } else {
        printf ("It took me %I64u micro-seconds.", ElapsedMicroseconds.QuadPart);
    }
    MUTEX_LIB_TIME_UNLOCK;
}

u32 LibTime_StopMicroSecondClock(void)
{
    u32 microSecond;

    MUTEX_LIB_TIME_LOCK;
    BASIC_ASSERT(gLibTimeLinux_IsStarted == 1);
    gLibTimeLinux_IsStarted = 0;

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

    microSecond = (u32)ElapsedMicroseconds.QuadPart;
    {
        gLibTimeLinux_IsStarted = 1;
        StartingTime = EndingTime;
    }
    MUTEX_LIB_TIME_UNLOCK;
    return microSecond;
}

