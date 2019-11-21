
#include "Everything_App.hpp"


int main(int argc, char *argv[])
{
	int  myArgc = 2;
	char *myArgv[4];
	char argv0[] = "dummy";
	char argv1[] = "..\\S01_CpuX_Assembler\\zAssembly_obj.txt";
	myArgv[0]=argv0;
	myArgv[1]=argv1;

	CPU_X_Main(myArgc-1, &myArgv[1]);

	return 0;
}
