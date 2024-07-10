
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>
#include <stdlib.h>     /* atoi */

#define USAGE "Usage:\n" \
              "ai [h/x/d] number number number ...\n"

int main(int argc, char *argv[])
{
	bool isHexMode = false;
	
	if (argc == 1 || argc == 2) {
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
	
	if (isHexMode) {
		printf("Hexadecimal Result:");
	}
	else {
		printf("Decimal Result:");
	}
	
	int argcCtr = 2;
	int curArgu;
	if (isHexMode) {
		while (argcCtr < argc) {
			sscanf(argv[argcCtr], "%x", &curArgu);
			printf("%c", (char)curArgu);
			argcCtr++;
		}
	}
	else {
		while (argcCtr < argc) {
			curArgu = atoi(argv[argcCtr]);
			printf("%c", (char)curArgu);
			argcCtr++;
		}
	}
	printf("\n");
	
	return 0;
}
