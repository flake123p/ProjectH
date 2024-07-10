
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>
#include <stdlib.h>     /* atoi */

#define USAGE "Usage:\n" \
              "ao [h/x/d] string\n"

int main(int argc, char *argv[])
{
	bool isHexMode = false;
	
	if (argc != 3) {
		printf(USAGE);
		return -1;
	}
	
	if(argv[1][0] == 'x' || argv[1][0] == 'X' || argv[1][0] == 'h' || argv[1][0] == 'H') {
		isHexMode = true;
	} 
	else if(argv[1][0] == 'd' || argv[1][0] == 'D') {
		isHexMode = false;
	} 
	else {
		printf(USAGE);
		return -1;
	}
	
	int argvCtr = 0;
	if (isHexMode) {
		printf("Hexadecimal Result:");
	}
	else {
		printf("Decimal Result:");
	}
	while (argv[2][argvCtr]) {
		if (isHexMode) {
			printf("%X ", (unsigned int)argv[2][argvCtr]);
		}
		else {
			printf("%d ", (unsigned int)argv[2][argvCtr]);
		}
		argvCtr++;
	}
	printf("\n");
	
	return 0;
}
