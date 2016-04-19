
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>

int main(int argc, char *argv[])
{
	const int mixNum = 5;
	const int maxNum = 20;
	int setNum = 22;
	
	try
	{
		if(setNum > maxNum)
			throw 0;
		if(setNum < mixNum)
			throw 1;
		std::cout << "setNum = " << setNum << std::endl;
	}
	catch(int err)
	{
		switch(err)
		{
			case 0:
				std::cout << "value too big" << std::endl;
				break;
			case 1:
				std::cout << "value too small" << std::endl;
				break;
			default:
				std::cout << "unknow value" << std::endl;
				break;
		}
	}
	
	return 0;
}
