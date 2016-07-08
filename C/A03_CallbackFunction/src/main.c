/*
	Purpose:
			Register callback function to OtherProcess()
*/
#include <stdio.h>

typedef void (*pFunction)(void);

pFunction gCallback = NULL;

void OtherProcess(void)
{
	printf("execute OtherProcess\n");
	if(gCallback != NULL)
	{
		printf("Excute externel callback:\n");
		(*gCallback)();
	}
	else
		printf("Have no callback function\n");
}

void OtherProcess_RegCB(pFunction p)
{
	gCallback = p;
	printf("register callback\n");
}



void MyCallback(void)
{
	printf("I am callback function from main()\n");
}

int main(int argc, char *argv[])
{
	OtherProcess_RegCB(MyCallback);
	OtherProcess();
	
	return 0;
}
