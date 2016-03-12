
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>

class abc {
	private:
		int a;
	protected:  // Like private in local class
		int b;
	public:
		int c;
		abc()
		{
			a = 1;
			b = 2;
			c = 3;
		};
};

class def_public : public abc 
{
	public:
		def_public(int x)
		{
			//a = x;   // Build test: NG
			//b = x;   // Build test: OK
			//c = x;   // Build test: OK
		};
};


int main(int argc, char *argv[])
{
	def_public obj1 = def_public(10);
	
	obj1 = obj1; //Bypass warning message
	
	obj1.c = 1;
	//obj1.b = 1;  // Build test: NG
	
	return 0;
}
