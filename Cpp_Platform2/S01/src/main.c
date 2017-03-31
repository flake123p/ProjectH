
#include "Everything_App.hpp"


int main(int argc, char *argv[])
{
	int myArgc = 3;
	char *myArgv[4];
	char argv0[] = "dummy";
	char argv1[] = "-x";
	char argv2[] = "zAssembly.txt";
	char argv3[] = "dummy";
	myArgv[0]=argv0;
	myArgv[1]=argv1;
	myArgv[2]=argv2;
	myArgv[3]=argv3;

	std::string fileName;
	fileName = "";
	DUMPS(fileName.c_str());
	Assembler_Main(myArgc, myArgv);
	
	//LibFileIO_TestNextLine_InWinAndLinux("zAssembly.txt");
	return 0;
}
