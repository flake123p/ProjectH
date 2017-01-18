/*
	Useful function from <stdlib.h>
*/

// ====== Standard C/Cpp Library ======
#include <stdio.h>
#include <string.h>
#include <stdint.h> // for uint32_t ...
#include <stdlib.h>
#include <time.h>

// ====== Platform Library ======
#include "LibUtility.hpp"
#include "My_Basics.hpp"

unsigned int seed;
void LibUtil_InitRand(void)
{
	seed += time(NULL);
	srand(seed);
}

int LibUtil_GetRand(void)
{
	return rand();
}

void LibUtil_Print_RAND_MAX(void)
{
	printf("RAND_MAX = %d\n", RAND_MAX);
}