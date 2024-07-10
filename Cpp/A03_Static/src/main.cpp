
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>

class abc {
	public:
		static int a;
		static void func(int x);
};

int abc::a;

void abc::func(int x)
{
	a = x;
}

int main(int argc, char *argv[])
{
	//call static function directly, doesn't init object
	abc::func(2);
	
	std::cout << "abc::a = " << abc::a << std::endl;
	
	return 0;
}
