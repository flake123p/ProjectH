


#ifndef _LIB_TUI_HPP_INCLUDED_



// ============================== Debug ==============================

// ============================== Library: Cross-Platform (Manager) ==============================
void LibTui_DumpDebugString(const char *string_to_dump, unsigned int str_len, int x, int y);
void LibTui_Init(void);
void LibTui_Uninit(void);
void LibTui_UpdateScreen(void);

int LibTuiMgr_DumpAttr(void);
int LibTuiMgr_DemoSimple(void);

// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================




#define _LIB_TUI_HPP_INCLUDED_
#endif//_LIB_TUI_HPP_INCLUDED_


