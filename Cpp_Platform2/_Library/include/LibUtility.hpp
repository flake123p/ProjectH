
#ifndef _LIB_UTILITY_HPP_INCLUDED_

#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================

/*
	Useful function from <stdlib.h>
*/

void LibUtil_InitRand(void);
int LibUtil_GetRand(void);
void LibUtil_Print_RAND_MAX(void);

template<typename T1>
bool LibUtil_FindInArray(IN T1 val, IN T1 *array, IN u32 len)
{
	for (u32 i=0; i<len; i++) {
		if (val == array[i])
			return true;
	}
	return false;
}

#define _LIB_UTILITY_HPP_INCLUDED_
#endif//_LIB_UTILITY_HPP_INCLUDED_