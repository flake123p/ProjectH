

#ifndef _LIB_TIME_HPP_INCLUDED_

#include <time.h>

// ============================== Manager ==============================
void Time_StartClock(void);
clock_t Time_StopClock(void);
double Time_CalculateClock(void);
void Time_StopClock_ShowResult(void);

void Time_DemoClockTimer(void);

// ============================== Library: Cross-Platform ==============================
void Time_PrintUtcTime(void);
void Time_PrintLocalTime(void);

// ============================== Library: Platform Dependant ==============================
void Time_StartMicroSecondClock(void);
void Time_StopMicroSecondClock_ShowResult(void);


#define _LIB_TIME_HPP_INCLUDED_
#endif//_LIB_TIME_HPP_INCLUDED_

