

#ifndef _LIB_EVENT_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================

// ============================== Library: Cross-Platform (Manager) ==============================
// For minimizing the global variable uses
void LibEvent_SendSimpleEvent(int inputEvent);
int LibEvent_GetSimpleEvent(bool do_clear = true);

// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================
//From Keyboard
#define KEYBOARD_BACKSPACE  (0x08)
#define KEYBOARD_ENTER      (0x0D)
#define KEYBOARD_ESC        (0x1B)
int LibEvent_GetOneKeyBoard(void);


#define _LIB_EVENT_HPP_INCLUDED_
#endif//_LIB_EVENT_HPP_INCLUDED_

