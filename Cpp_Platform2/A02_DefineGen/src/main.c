
#include "Everything_App.hpp"

int main(int argc, char *argv[])
{
	char str[30];
	for (u32 i = 0; i < 32; i++) {
		sprintf(str, "BIT_POSITION_%d", i);
		printf("#define %-15s (%2d)\n", str, i);
	}
	#if 0
	for (u32 i = 0; i < 32; i++) {
		sprintf(str, "BIT_MASK_%d", i);
		printf("#define %-15s (1<<%2d) //0x%08X\n", str, i, 1<<i);
	}
	#endif

	return 0;
}
