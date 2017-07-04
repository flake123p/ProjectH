
#include "Everything_App.hpp"

#define DEV_MODE ( 1 )

int main(int argc, char *argv[])
{
#if DEV_MODE
	int argcX = 2;
	char argvX0[] = "xxx";
	char argvX1[] = "__STATES__.ini";
	char *argvX[2] = {
		argvX0,
		argvX1,
	};
	StateMachineMaker(argcX, argvX);
#else
	StateMachineMaker(argc, argv);
#endif
	return 0;
}

