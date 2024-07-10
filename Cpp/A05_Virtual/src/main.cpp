
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>

// Abstract class = there is virtual function inside
class abc
{
	public:
		virtual void show() = 0;
};

class v1:public abc
{
	public:
		// Must implement show()
		void show() { std::cout << "this is v1" << std::endl; }
};

class v2:public abc
{
	public:
		// Must implement show()
		void show() { std::cout << "this is v2" << std::endl; }
};

int main(int argc, char *argv[])
{
	abc *p;
	v1 obj1;
	v2 obj2;
	
	p = &obj1;
	p->show();
	
	p = &obj2;
	p->show();
	
	return 0;
}
