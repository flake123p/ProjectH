
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>

#define USE_NAME_SPACE 0

#if USE_NAME_SPACE
using namespace std;
#endif

int main(int argc, char *argv[])
{
#if USE_NAME_SPACE
	cout << "using namespace" << endl;
#else
	std::cout << "unusing namespace" << std::endl;
#endif	
	return 0;
}
