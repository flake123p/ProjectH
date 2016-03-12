
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>

class abc {
	private:
		int a;
	public:
		int b;
		abc()
		{
			a = 1;
			b = 2;
		};
		//If mark these line, func() won't be able to access private member variable a.
		friend void func(void);
		friend class def;
};

abc obj = abc();

void func(void)
{
	obj.b = obj.a + obj.b;
}

class def {
	public:
		void func_b(void)
		{
			obj.b += obj.a;
		}
};

int main(int argc, char *argv[])
{
	//func doesn't belong to abc class, even it is a friend function
	//obj.func();
	//abc::func();
	
	func();
	
	def obj2 = def();
	obj2.func_b();
	
	std::cout << "obj.b = " << obj.b << std::endl;
	return 0;
}
