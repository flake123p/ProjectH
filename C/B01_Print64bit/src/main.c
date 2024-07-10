
/*
	Web Resource:
	http://stackoverflow.com/questions/2844/how-do-you-printf-an-unsigned-long-long-intthe-format-specifier-for-unsigned-lo
*/

#include <stdio.h>
#include <stdint.h>

//
// Flake: It's better using these macro (Print64/Print64f)
//
#if defined(_WIN32) || defined(_WIN64)
	#define Print64(a)          "%I64"#a
	#define Print64f(format,a)  "%"#format"I64"#a
#elif defined(__unix__) || defined(__linux) || defined(__linux__)
	#define Print64(a)          "%ll"#a
	#define Print64f(format,a)  "%"#format"ll"#a
#endif

int main(int argc, char *argv[])
{
	int64_t a = 0xA987654321; //728121033505
	int64_t b = 0x123456789A; //78187493530
	
#if defined(_WIN32) || defined(_WIN64)
	printf("a = %I64d, b = %I64d\n", a, b);
	printf("a = 0x%I64X, b = 0x%I64X\n", a, b);
#endif

#if defined(__unix__) || defined(__linux) || defined(__linux__)
	printf("a = %lld, b = %lld\n", a, b);
	printf("a = 0x%llX, b = 0x%llX\n", a, b);
#endif

	printf("Unified >> a = " Print64(d) ", b = " Print64(d) "\n", a, b);
	printf("Unified >> a = " Print64(x) ", b = " Print64(x) "\n", a, b);
	// Demo "format" usage
	printf("Unified >> a = " Print64f(15,x) ", b = " Print64f(15,x) "\n", a, b);
	printf("Unified >> a = " Print64f(-15,x) ", b = " Print64f(-15,x) "\n", a, b);
	
	return 316227766;
}
