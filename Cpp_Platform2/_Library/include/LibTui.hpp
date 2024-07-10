


#ifndef _LIB_TUI_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================

// ============================== Library: Cross-Platform (Manager) ==============================
#define TUI_KEY_BREAK     0x101  /* Not on PC KBD */
#define TUI_KEY_DOWN      0x102  /* Down arrow key */
#define TUI_KEY_UP        0x103  /* Up arrow key */
#define TUI_KEY_LEFT      0x104  /* Left arrow key */
#define TUI_KEY_RIGHT     0x105  /* Right arrow key */
#define TUI_KEY_HOME      0x106  /* home key */
#define TUI_KEY_BACKSPACE 0x107  /* not on pc */
#define TUI_KEY_F0        0x108  /* function keys; 64 reserved */
#define TUI_KEY_F1        0x109
#define TUI_KEY_F2        0x10A
#define TUI_KEY_F3        0x10B
#define TUI_KEY_F4        0x10C
#define TUI_KEY_F5        0x10D
#define TUI_KEY_F6        0x10E
#define TUI_KEY_F7        0x10F
#define TUI_KEY_F8        0x110
#define TUI_KEY_F9        0x111
#define TUI_KEY_F10       0x112
#define TUI_KEY_F11       0x113
#define TUI_KEY_F12       0x114

#define TUI_KEY_NPAGE     0x152  /* next page */
#define TUI_KEY_PPAGE     0x153  /* previous page */

#define TUI_PADENTER      0x1cb  /* enter on keypad */

void LibTui_DrawString(const char *str, int x, int y, bool do_reverse = false, int color = 0);
void LibTui_DrawDecimal(int val, int x, int y, bool do_reverse = false, int color = 0);
void LibTui_Init(void);
void LibTui_Uninit(void);
int LibTui_GetCh(void);
void LibTui_GetMaxXY(OUT int *x, OUT int *y);
void LibTui_MoveCursor(int x, int y);
void LibTui_ClearArea(int x, int y, int x_len, int y_len, int ch);

typedef enum {
	TUI_CLEAR_SCREEN,
	TUI_REFRESH_SCREEN,
	TUI_MOVE_CURSOR,
} TUI_COMMAND;
typedef struct {
	int x;
	int y;
} TUI_COMMAND_HDL_t;
int LibTui_Command(TUI_COMMAND cmd, TUI_COMMAND_HDL_t *cmdHdl = NULL);

int LibTuiMgr_DemoDumpAttr(void);
int LibTuiMgr_DemoSimple(void);
int LibTuiMgr_DemoDumpMem(char *memory_address);

// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================




#define _LIB_TUI_HPP_INCLUDED_
#endif//_LIB_TUI_HPP_INCLUDED_


