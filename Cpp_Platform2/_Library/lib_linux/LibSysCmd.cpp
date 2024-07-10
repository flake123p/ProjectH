
#include "Everything_Lib_Linux.hpp"

void LibSysCmd_SimpleCmd(const char *cmdStr)
{
	system(cmdStr);
}

void LibSysCmd_ClearScreen(void)
{
	system("clear");
}

#include "LibEvent.hpp"
void LibSysCmd_Pause(void)
{
	//Linux don't have pause.
	printf("Press any key to continue ...\n");
	LibEvent_GetOneKeyBoard();
}
