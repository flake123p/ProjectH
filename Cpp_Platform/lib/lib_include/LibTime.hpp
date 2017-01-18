

#ifndef _LIB_TIME_HPP_INCLUDED_

#include <time.h>

// ============================== Manager ==============================
void LibTime_StartClock(void);
clock_t LibTime_StopClock(void);
double LibTime_CalculateClock(void);
void LibTime_StopClock_ShowResult(void);

void LibTime_DemoClockTimer(void);

// ============================== Library: Cross-Platform ==============================
void LibTime_PrintUtcTime(void);
void LibTime_PrintLocalTime(void);

// ============================== Library: Platform Dependant ==============================
void LibTime_StartMicroSecondClock(void);
void LibTime_StopMicroSecondClock_ShowResult(void);


#define _LIB_TIME_HPP_INCLUDED_
#endif//_LIB_TIME_HPP_INCLUDED_

