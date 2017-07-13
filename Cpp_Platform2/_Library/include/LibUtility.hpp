
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

template<typename T1_Container, typename T2_Value>
s32 LibUtil_FindInContainer_ReturnIndex(T1_Container &container, T2_Value &value)
{
	for (s32 i=0; i<container.size(); i++) {
		if (value == container[i])
			return i;
	}
	return -1;
}

int LibUtil_BytesToInt32(u32 &dst, u8 *src, bool isSrcBigEndian);
int LibUtil_Int32ToBytes(u8 *dst, u32 &src, bool isDstBigEndian);
int LibUtil_BytesToInt16(u16 &dst, u8 *src, bool isSrcBigEndian);
int LibUtil_Int16ToBytes(u8 *dst, u16 &src, bool isDstBigEndian);
int LibUtil_IntSwapCopy(u8 *dst, u8 *src, u32 len, bool swap);
int LibUtil_DemoEndian(void);

#include "My_Macros.h"
#define INT_TO_ARRAY(dst,src,currOffset,swap) LibUtil_IntSwapCopy(ADDRX(dst,currOffset),(u8 *)&(src),sizeof(src),swap);currOffset+=sizeof(src)
#define ARRAY_TO_INT(dst,src,currOffset,swap) LibUtil_IntSwapCopy((u8 *)&(dst),ADDRX(src,currOffset),sizeof(dst),swap);currOffset+=sizeof(dst)

int LibUtil_MaxMinMgr_Init(s32 initVal = 0);
int LibUtil_MaxMinMgr_Input(s32 inVal);
s32 LibUtil_MaxMinMgr_GetMax(void);
s32 LibUtil_MaxMinMgr_GetMin(void);

#define _LIB_UTILITY_HPP_INCLUDED_
#endif//_LIB_UTILITY_HPP_INCLUDED_