
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>
//need include header
#include <vector>

int main(int argc, char *argv[])
{
	std::vector<int> v(10);
	int i;
	
	std::cout << "size = " << v.size() << std::endl;

	for(i=0; i<v.size(); i++)
	{
		v[i] = i;
	}
	
	for(i=0; i<v.size(); i++)
	{
		std::cout << "v[" << i << "] = " << v[i] << std::endl;
	}
	
	return 0;
}
