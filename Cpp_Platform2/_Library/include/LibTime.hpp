

#ifndef _LIB_TIME_HPP_INCLUDED_

#include <time.h>
#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================

// ============================== Manager ==============================
void LibTime_StartClock(void);
clock_t LibTime_StopClock(void);
double LibTime_CalculateClock(void);
void LibTime_StopClock_ShowResult(void);

void LibTime_DemoClockTimer(void);
void LibTime_DemoClockTimer2(void);

void LibTime_PrintUtcTime(void);
void LibTime_PrintLocalTime(void);

// ============================== Library: Platform Dependant ==============================
void LibTime_StartMicroSecondClock(void);
void LibTime_StopMicroSecondClock_ShowResult(int do_print_nextline = 0);
u32 LibTime_StopMicroSecondClock(void);

#define _LIB_TIME_HPP_INCLUDED_
#endif//_LIB_TIME_HPP_INCLUDED_

