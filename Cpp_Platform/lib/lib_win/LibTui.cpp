#include <stdio.h>
#include <stdlib.h>

#include <curses.h>

#include "LibEvent.hpp"
#include "LibTui.hpp"

#define C_WHITE_ON_BLUE (1)
#define C_BLACK_ON_CYAN (2)
#define C_BLACK_ON_WHITE (3)

int gX=1;
int gY=1;

void LibTui_DumpDebugString(const char *string_to_dump, unsigned int str_len, int x, int y)
{
	unsigned int i;

	#if 0
	move(y,x);
	for (i=0; i<20; i++) {
		addch(' ');
	}
	#endif
	
	move(y,x);
	for (i=0; i<str_len; i++) {
		addch(string_to_dump[i]);
	}
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

int LibTuiMgr_DemoSimple(void)
{
	char str[] = "LibTuiMgr_DemoSimple() ... PRESS ANY KEY TO LEAVE";
	
	LibTui_Init();

	LibTui_DumpDebugString(str, sizeof(str)-1, 10, 20);
	LibTui_UpdateScreen();
	LibEvent_GetOneKeyBoard();

	LibTui_Uninit();
	return 0;
}