
#include "Everything_App.hpp"


int main(int argc, char *argv[])
{
#if 0
	int  myArgc = 4;
	char *myArgv[4];
	char argv0[] = "dummy";
	char argv1[] = "zAssembly.txt";
	char argv2[] = "-o";
	char argv3[] = "zAssembly_obj.txt";
	myArgv[0]=argv0;
	myArgv[1]=argv1;
	myArgv[2]=argv2;
	myArgv[3]=argv3;
#else
	int  myArgc = 2;
	char *myArgv[4];
	char argv0[] = "dummy";
	char argv1[] = "zAssembly.txt";
	myArgv[0]=argv0;
	myArgv[1]=argv1;
#endif

	Assembler_Main(myArgc-1, &myArgv[1]);
	
	//LibFileIO_TestNextLine_InWinAndLinux("zAssembly.txt");
	return 0;
}
