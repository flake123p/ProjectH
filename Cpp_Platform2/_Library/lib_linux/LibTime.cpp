
#include "Everything_Lib_Linux.hpp"

// ====== Platform Library ======
#include "LibTime.hpp"

struct timeval t1, t2;
double elapsedTime;

void LibTime_StartMicroSecondClock(void)
{
	// start timer
	gettimeofday(&t1, NULL);
}

void LibTime_StopMicroSecondClock_ShowResult(void)
{
	// stop timer
	gettimeofday(&t2, NULL);

	// compute and print the elapsed time in millisec
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
	printf ("It took me %f mili-seconds.", elapsedTime);
}

