
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>

template<typename T1, typename T2>
void MyMemcpy(T1 *dst, T2 *src, size_t loopMax, T2 carryCompensate)
{
	for (size_t loopCtr = 0; loopCtr < loopMax; loopCtr++)
	{
		dst[loopCtr] = (T1)(src[loopCtr] + carryCompensate);
	}
}

int main(int argc, char *argv[])
{
	int i[3];
	double j[3] = {0.0, 0.8, 1.2};
	
	//carryCompensate for rounding
	MyMemcpy<int, double>(i, j, 3, 0.5);
	
	for (int k = 0; k < 3; k++)
		std::cout << i[k] << std::endl;
	return 0;
}
