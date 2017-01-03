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
#include "My_Basics.hpp"
#include "LibUtility.hpp"

unsigned int seed;
void Util_InitRand(void)
{
	seed += time(NULL);
	srand(seed);
}

int Util_GetRand(void)
{
	return rand();
}

void Util_Print_RAND_MAX(void)
{
	printf("RAND_MAX = %d\n", RAND_MAX);
}