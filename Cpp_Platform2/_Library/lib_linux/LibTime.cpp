
#include "Everything_Lib_Linux.hpp"

// ====== Platform Library ======
#include "LibTime.hpp"
#include "_LibMT.hpp"

static int gLibTimeLinux_IsStarted = 0;

struct timeval t1, t2;
double elapsedTime;

void LibTime_StartMicroSecondClock(void)
{
    MUTEX_LIB_TIME_LOCK;
    BASIC_ASSERT(gLibTimeLinux_IsStarted == 0);
    gLibTimeLinux_IsStarted = 1;

    // start timer
    gettimeofday(&t1, NULL);
    MUTEX_LIB_TIME_UNLOCK;
}

void LibTime_StopMicroSecondClock_ShowResult(int do_print_nextline /* = 0 */)
{
    MUTEX_LIB_TIME_LOCK;
    BASIC_ASSERT(gLibTimeLinux_IsStarted == 1);
    gLibTimeLinux_IsStarted = 0;

    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to us
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

    if (do_print_nextline) {
        printf ("It took me %f mili-seconds.\n", elapsedTime);
    } else {
        printf ("It took me %f mili-seconds.", elapsedTime);
    }
    MUTEX_LIB_TIME_UNLOCK;
}

u32 LibTime_StopMicroSecondClock(void)
{
    u32 microSecond;

    MUTEX_LIB_TIME_LOCK;
    BASIC_ASSERT(gLibTimeLinux_IsStarted == 1);
    gLibTimeLinux_IsStarted = 0;

    // stop timer
    gettimeofday(&t2, NULL);

    if (t2.tv_usec >= t1.tv_usec) {
        microSecond = ((t2.tv_sec - t1.tv_sec) * 1000000) + ((t2.tv_usec - t1.tv_usec));
    } else {
        microSecond = ((t2.tv_sec - t1.tv_sec) * 1000000) + ((t1.tv_usec - t2.tv_usec)) - 1000000;
    }

    {
        t1 = t2;
    }
    MUTEX_LIB_TIME_UNLOCK;
    return microSecond;
}