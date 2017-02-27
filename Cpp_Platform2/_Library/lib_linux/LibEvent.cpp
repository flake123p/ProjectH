
#include <termios.h>
#include <stdio.h>

/*
 *   Reference:
 *     1.) http://stackoverflow.com/questions/7469139/what-is-equivalent-to-getch-getche-in-linux
 *     2.) http://stackoverflow.com/questions/3276546/how-to-implement-getch-function-of-c-in-linux
 */

struct termios gOld, gNew;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
	tcgetattr(0, &gOld); /* grab old terminal i/o settings */
	gNew = gOld; /* make new settings same as old settings */
	gNew.c_lflag &= ~ICANON; /* disable buffered i/o */
	gNew.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	tcsetattr(0, TCSANOW, &gNew); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
	tcsetattr(0, TCSANOW, &gOld);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
	return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
	return getch_(1);
}

int LibEvent_GetOneKeyBoard(void)
{
	return getch();
}

