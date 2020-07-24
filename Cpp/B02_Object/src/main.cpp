
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>
#include <string.h>

using namespace std;

class libaaa {
public:
    libaaa(void){printf("AAA\n");};
    ~libaaa(void){printf("aaa\n");};
};
class libbbb : public libaaa {
public:
    libbbb(void){printf("BBB\n");};
    ~libbbb(void){printf("bbb\n");};
};

int main(int argc, char *argv[])
{
	libbbb *b = new(libbbb);
	int x = (int)b;
	printf("123 0x%08X\n", x);
	unsigned char ary[20];
	memcpy(ary, &b, sizeof(void *));
	printf("123 0x%02X 0x%02X 0x%02X 0x%02X\n", ary[0], ary[1], ary[2], ary[3]);
	delete(*((libbbb**)ary));
	printf("456\n");
	
	return 0;
}
