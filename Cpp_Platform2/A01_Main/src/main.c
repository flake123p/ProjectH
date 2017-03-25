
#include "Everything_App.hpp"


int main(int argc, char *argv[])
{
	//LibMemory_Demo3();
	LibThreadMgr_DemoLite();

	void *ptr = (void *)LibThreadMgr_DemoLite;
	DUMPP(ptr);

	//printf("aaa  0x%08X\n", POINTER_TO_U32(ptr));
	DUMPA(ptr);
	return 0;
}
