
#include <Windows.h>

void LibSysCmd_SimpleCmd(const char *cmdStr)
{
	system(cmdStr);
}

void LibSysCmd_ClearScreen(void)
{
	system("CLS");
}

void LibSysCmd_Pause(void)
{
	system("PAUSE");
}
