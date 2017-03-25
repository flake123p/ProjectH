
#ifdef BUILD_TUI_LIBRARY

#include "Everything_Lib_Mgr.hpp"

#include <curses.h>

#include "LibTui.hpp"

#define C_WHITE_ON_BLUE  (1)
#define C_BLACK_ON_CYAN  (2)
#define C_BLACK_ON_WHITE (3)

int gX=1;
int gY=1;

void LibTui_DrawString(const char *str, int x, int y, bool do_reverse /* = false */, int color /* = 0 */)
{
	if (do_reverse)
		attron(A_REVERSE);
	if (color)
		attron(COLOR_PAIR(color));
		
	mvaddstr(y, x, str);
	
	if (do_reverse)
		attroff(A_REVERSE);
	if (color)
		attroff(COLOR_PAIR(color));
}

void LibTui_DrawDecimal(int val, int x, int y, bool do_reverse /* = false */, int color /* = 0 */)
{
	char str[20];
	sprintf(str, "%d", val);

	LibTui_DrawString(str, x, y, do_reverse, color);
}

void LibTui_Init(void)
{
	initscr();
	start_color();
	init_pair(C_WHITE_ON_BLUE, COLOR_WHITE, COLOR_BLUE);
	init_pair(C_BLACK_ON_CYAN, COLOR_BLACK, COLOR_CYAN);
	init_pair(C_BLACK_ON_WHITE, COLOR_BLACK, COLOR_WHITE);

	cbreak(); /* don't buffer keyboard (Old curses: crmode())*/
	//nocbreak(); /* do buffer keyboard (Old curses: nocrmode())*/

	nonl();
	noecho();
	intrflush(stdscr,FALSE);
	keypad(stdscr,TRUE);
}

void LibTui_Uninit(void)
{
	endwin();
}

int LibTui_GetCh(void)
{
	return getch();
}

void LibTui_GetMaxXY(OUT int *x, OUT int *y)
{
	*x = getmaxx(stdscr);
	*y = getmaxy(stdscr);
}

void LibTui_MoveCursor(int x, int y)
{
	move(y,x);
}

void LibTui_ClearArea(int x, int y, int x_len, int y_len, int ch)
{
	//int oriX = x;
	//int oriY = y;
	move(y, x);

	for (int i = 0; i < y_len; i++) {
		move(y+i,x);
		for (int j = 0; j < x_len; j++) {
			addch(ch);
		}
	}

	//move(oriX, oriY);
}

int LibTui_Command(TUI_COMMAND cmd, TUI_COMMAND_HDL_t *cmdHdl /* = NULL */)
{
	switch (cmd) {
	case TUI_CLEAR_SCREEN:
		wclear(stdscr);
	break;

	case TUI_REFRESH_SCREEN:
		refresh();
	break;

	case TUI_MOVE_CURSOR:
		move(cmdHdl->y, cmdHdl->x);
	break;
	}

	return 0;
}

int LibTuiMgr_DemoDumpAttr(void)
{
	int ch;
	char str[50];
	LibTui_Init();

	LibTui_DrawString("PRESS [ESC] TO LEAVE:", 0, 0);
	while (1) {
		ch = LibTui_GetCh();

		//ESC
		if (ch == 27) {
			break;
		}

		sprintf(str, "0x%04X", ch);
		LibTui_DrawString("          ", 0, 1);
		LibTui_DrawString(str, 0, 1);
		LibTui_Command(TUI_REFRESH_SCREEN);
	}

	LibTui_Uninit();
	return 0;
}

int LibTuiMgr_DemoSimple(void)
{
	char str[] = "LibTuiMgr_DemoSimple() ... PRESS ANY KEY TO LEAVE";
	
	LibTui_Init();

	LibTui_DrawString(str, 10, 20);
	LibTui_Command(TUI_REFRESH_SCREEN);
	LibTui_GetCh();

	LibTui_Uninit();
	return 0;
}

int LibTuiMgr_DemoDumpMem(char *memory_address)
{
	int ch;
	char str[80];
	char lineStr[17];
	u8 *mem = (u8 *)memory_address;
	LibTui_Init();

	LibTui_DrawString("PRESS [ESC] TO LEAVE:", 0, 0);
	while (1) {
		mem = (u8 *)memory_address;
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				if (LibString_IsCharPrintable(mem[j])) {
					lineStr[j] = mem[j];
				} else {
					lineStr[j] = '-';
				}
			}
			lineStr[16] = 0;
			
			sprintf(str, 
			        "[%p] %02X %02X %02X %02X %02X %02X %02X %02X - %02X %02X %02X %02X %02X %02X %02X %02X  **%s**", 
			        mem,
			        mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7], mem[8], mem[9], mem[10], mem[11], mem[12], mem[13], mem[14], mem[15],
			        lineStr
			        );
			LibTui_DrawString(str, 0, i+2);
			mem+=16;
		}
		
		LibTui_Command(TUI_REFRESH_SCREEN);

		ch = LibTui_GetCh();

		//ESC
		if (ch == 27) {
			break;
		}
	}

	LibTui_Uninit();
	return 0;
}

#endif //#ifdef BUILD_TUI_LIBRARY