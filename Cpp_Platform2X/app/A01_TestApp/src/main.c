
#include "Everything_App.hpp"

int main(int argc, char *argv[])
{
	TestMod2_DemoX();
	
	TestMod3_Demo();

#ifdef DFS_TEST
	printf("This is %s() ... DFS_TEST is defined!!\n", __func__);
#else
	printf("This is %s() ... DFS_TEST is not defined!!\n", __func__);
#endif

#ifdef DFS_TEST2
	printf("This is %s() ... DFS_TEST2 is defined!!\n", __func__);
#else
	printf("This is %s() ... DFS_TEST2 is not defined!!\n", __func__);
#endif

	return 0;
}

