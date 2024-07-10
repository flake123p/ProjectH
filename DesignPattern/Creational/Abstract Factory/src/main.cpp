
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>

class L1 {
public:
	int i;
	const char *_from;
	L1(const char *from="NONE", int in_i=0){_from=from;i=in_i;printf("construct %s\n",_from);};
	~L1(){printf("destruct %s\n",_from);};
};
class B: virtual public L1 {
public:
	B(int in_i = 0) : L1("B",in_i)
	{
	};
	~B(){};
};
class C: virtual public L1 {
public:
	C(int in_i = 0) : L1("C",in_i)
	{
	};
	~C(){};
};
class D: public B, public C {
public:
	D(int in_i = 0) : L1("D",in_i)
	{
	};
	~D(){};
};

class D d = D(1);
int main(int argc, char *argv[])
{
	
	class C c;
	
	class B *b = new(B);
	
	//d.i = 0;
	
	printf("d.i=%d\n", d.i);
	printf("%s\n", d._from);
	
	printf("c.i=%d\n", c.i);
	printf("%s\n", c._from);
	
	printf("b->i=%d\n", b->i);
	printf("%s\n", b->_from);
	
	//memory leak, *b is not been deleted
	//delete(b);
	
	return 0;
}
