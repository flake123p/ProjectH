
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>

class abc
{
	private:
		int i;
	public:
		abc():i(9){};
		int get(){return i;}
};

abc gObj;

abc &GetVar()
{
	return gObj;
}

int main(int argc, char *argv[])
{
	std::cout << "GetVar().get() = " << GetVar().get() << std::endl; 
	return 0;
}
