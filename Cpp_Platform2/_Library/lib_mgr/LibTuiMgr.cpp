
#include "Everything_Lib_Mgr.hpp"

#include <curses.h>

#include "LibTui.hpp"

#define C_WHITE_ON_BLUE  (1)
#define C_BLACK_ON_CYAN  (2)
#define C_BLACK_ON_WHITE (3)

int gX=1;
int gY=1;

void LibTui_DumpDebugString(const char *string_to_dump, int x, int y)
{
	attron(A_REVERSE);
	mvaddstr(y, x, string_to_dump);
	attroff(A_REVERSE);

	move(gY,gX);
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

void LibTui_UpdateScreen(void)
{
	refresh();
}

int LibTui_GetCh(void)
{
	return getch();
}

int LibTuiMgr_DumpAttr(void)
{
	int ch;
	char str[50];
	LibTui_Init();

	LibTui_DumpDebugString("PRESS [ESC] TO LEAVE:", 0, 0);
	while (1) {
		ch = LibTui_GetCh();

		//ESC
		if (ch == 27) {
			break;
		}

		sprintf(str, "0x%04X", ch);
		LibTui_DumpDebugString("          ", 0, 1);
		LibTui_DumpDebugString(str, 0, 1);
		LibTui_UpdateScreen();
	}

	LibTui_Uninit();
	return 0;
}

int LibTuiMgr_DemoSimple(void)
{
	char str[] = "LibTuiMgr_DemoSimple() ... PRESS ANY KEY TO LEAVE";
	
	LibTui_Init();

	LibTui_DumpDebugString(str, 10, 20);
	LibTui_UpdateScreen();
	LibTui_GetCh();

	LibTui_Uninit();
	return 0;
}



