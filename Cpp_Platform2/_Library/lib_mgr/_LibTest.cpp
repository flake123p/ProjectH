
#include "Everything_Lib_Mgr.hpp"

void LibTest_DumpMacro(void)
{
	int i;

	void *ptr = (void *)&i;
	DUMPP(ptr);

	//printf("aaa  0x%08X\n", POINTER_TO_U32(ptr));
	DUMPA(ptr);
}

