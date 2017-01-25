

#ifndef _LIB_EVENY_HPP_INCLUDED_

// ============================== Manager ==============================

// ============================== Library: Cross-Platform ==============================
// For minimizing the global variable uses
void LibEvent_SendSimpleEvent(int inputEvent);
int LibEvent_GetSimpleEvent(bool do_clear = true);

// ============================== Library: Platform Dependant ==============================
int LibEvent_GetOneKeyBoard(void);


#define _LIB_EVENY_HPP_INCLUDED_
#endif//_LIB_EVENY_HPP_INCLUDED_

