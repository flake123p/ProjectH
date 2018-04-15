
#include "Everything_App.hpp"


int main(int argc, char *argv[])
{
	SimAir_Init();

	SimNode0_Init();
	SimNode1_Init();
	SimNode2_Init();

	SimAir_Run();

	return 0;
}

