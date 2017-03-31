

#ifndef _LIB_OS_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================

// ============================== Library: Cross-Platform (Manager) ==============================
void LibOsMgr_DemoSleep(void);

// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================
void LibOs_SleepSeconds(unsigned int seconds);
void LibOs_SleepMiliSeconds(unsigned int miliSeconds);
void LibOs_SleepMicroSeconds(unsigned int microSeconds);



#define _LIB_OS_HPP_INCLUDED_
#endif//_LIB_OS_HPP_INCLUDED_

