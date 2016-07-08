/*
	OS Define
	
	Web Resource:
	http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
*/
#include <stdio.h>

typedef void (*pFunction)(void);

pFunction reg;

void FunctionA(void)
{
	printf("execute function A\n");
	if(reg != NULL)
		reg();
	else
		printf("do not register callback functgion\n");
}

void FunctionA_CB(void)
{
	printf("I am callback function\n");
}

void RegCB(pFunction p)
{
	reg = p;
	printf("register callback\n");
}

int main(int argc, char *argv[])
{
	RegCB(FunctionA_CB);
	FunctionA();
	
	return 0;
}
