#include <stdio.h>

int myfunc(int argu1, int argu2);

int main(int argc, char *argv[])
{
	printf("This sample will call ASM procedure(function).\n");
	
	int out = myfunc(20, 81);
	
	printf("ADD FUNC --->  myfunc(20, 81) = [ %d ]\n", out);
	
	int out2 = myfunc2(30, 5);
	
	printf("SUB FUNC --->  myfunc2(30, 5) = [ %d ]\n", out2);
	return 0;
}

