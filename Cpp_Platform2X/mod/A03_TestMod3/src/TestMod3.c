
#include "Everything_TestMod3.hpp"

int TestMod3_Demo(void)
{
	printf("This is %s()\n", __func__);

	TestMod2_Demo();
	TestMod2_DemoX();

	return 0;
}

