
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>

/*
	static variable:
		...
	static function:
		...
	protected:
		...
*/

class singleX {
	public:
		static singleX* Instance();

		//existing intervace goes here
	protected:
		singleX(){};
		
	private:
		static singleX* _instance;
};

singleX* singleX::_instance = 0;

singleX* singleX::Instance()
{
	if (_instance == 0) {
		printf("init only one instance\n");
		_instance = new singleX;
	} else {
		printf("return existed only one instance\n");
	}
	return _instance;
}

int main(int argc, char *argv[])
{
	singleX *pa;
	printf("line:%d\n", __LINE__);
	pa = pa->Instance();
	
	singleX *pb;
	printf("line:%d\n", __LINE__);
	pb = pb->Instance();
	
	return 0;
}
