
#include "Everything_App.hpp"


int main(int argc, char *argv[])
{
	LibTime_StartMicroSecondClock();
	
	Lib51HexReader reader;

	if (argc == 1)
		reader.ReadFile("D:\\abc.hex");
	else
		reader.ReadFile(argv[1]);

	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;

	LibTime_StartMicroSecondClock();
	
	reader.virMem.DumpVirMemContent_ToFile("Lib51DEMO.txt");

	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;
	
	return 0;
}

