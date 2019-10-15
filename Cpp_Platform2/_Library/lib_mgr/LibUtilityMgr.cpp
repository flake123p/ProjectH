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
#include "_LibError.hpp"
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

int LibUtil_BytesToInt32(u32 &dst, u8 *src, bool isSrcBigEndian)
{
	if (IS_LITTLE_ENDIAN()) {
		if (isSrcBigEndian) {
			//SWAP
			dst = SHIFT_OR_4(src[0], src[1], src[2], src[3]);
		} else {
			//NO SWAP
			dst = SHIFT_OR_4(src[3], src[2], src[1], src[0]);
		}
	} else {
		// BIG ENDIAN
		if (isSrcBigEndian) {
			//NO SWAP
			dst = SHIFT_OR_4(src[3], src[2], src[1], src[0]);
		} else {
			//SWAP
			dst = SHIFT_OR_4(src[0], src[1], src[2], src[3]);
		}
	}

	return 0;
}

int LibUtil_Int32ToBytes(u8 *dst, u32 &src, bool isDstBigEndian)
{
	if (IS_LITTLE_ENDIAN()) {
		if (isDstBigEndian) {
			dst[0] = src>>24;
			dst[1] = src>>16;
			dst[2] = src>>8;
			dst[3] = src;
		} else {
			dst[3] = src>>24;
			dst[2] = src>>16;
			dst[1] = src>>8;
			dst[0] = src;
		}
	} else {
		// BIG ENDIAN
		if (isDstBigEndian) {
			dst[3] = src>>24;
			dst[2] = src>>16;
			dst[1] = src>>8;
			dst[0] = src;
		} else {
			dst[0] = src>>24;
			dst[1] = src>>16;
			dst[2] = src>>8;
			dst[3] = src;
		}
	}
	return 0;
}

int LibUtil_BytesToInt16(u16 &dst, u8 *src, bool isSrcBigEndian)
{
	if (IS_LITTLE_ENDIAN()) {
		if (isSrcBigEndian) {
			//SWAP
			dst = SHIFT_OR_2(src[0], src[1]);
		} else {
			//NO SWAP
			dst = SHIFT_OR_2(src[1], src[0]);
		}
	} else {
		// BIG ENDIAN
		if (isSrcBigEndian) {
			//NO SWAP
			dst = SHIFT_OR_2(src[1], src[0]);
		} else {
			//SWAP
			dst = SHIFT_OR_2(src[0], src[1]);
		}
	}

	return 0;
}

int LibUtil_Int16ToBytes(u8 *dst, u16 &src, bool isDstBigEndian)
{
	if (IS_LITTLE_ENDIAN()) {
		if (isDstBigEndian) {
			dst[0] = src>>8;
			dst[1] = src;
		} else {
			dst[1] = src>>8;
			dst[0] = src;
		}
	} else {
		// BIG ENDIAN
		if (isDstBigEndian) {
			dst[1] = src>>8;
			dst[0] = src;
		} else {
			dst[0] = src>>8;
			dst[1] = src;
		}
	}
	return 0;
}

// Support Macro: DO_ENDIAN_SWAP / NO_ENDIAN_SWAP
int LibUtil_IntSwapCopy(u8 *dst, u8 *src, u32 len, bool swap)
{
	switch(len) {
		case 1: {
			dst[0] = src[0];
		} break;

		case 2: {
			if (swap) {
				dst[0] = src[1];
				dst[1] = src[0];
			} else {
				dst[0] = src[0];
				dst[1] = src[1];
			}
		} break;

		case 4: {
			if (swap) {
				dst[0] = src[3];
				dst[1] = src[2];
				dst[2] = src[1];
				dst[3] = src[0];
			} else {
				dst[0] = src[0];
				dst[1] = src[1];
				dst[2] = src[2];
				dst[3] = src[3];
			}
		} break;
		
		default: {
			BASIC_ASSERT(0);
		} break;
	}
	return 0;
}

int LibUtil_BytesAssembleU32(u32 &dst, u8 *src, u32 bytesNum, bool isSrcBigEndian)
{
	bool doSwap;

	BASIC_ASSERT(bytesNum >= 1 && bytesNum <= 4);

	if (IS_LITTLE_ENDIAN()) {
		if (isSrcBigEndian) {
			doSwap = true;
		} else {
			doSwap = false;
		}
	} else {
		// BIG ENDIAN
		if (isSrcBigEndian) {
			doSwap = false;
		} else {
			doSwap = true;
		}
	}

	if (doSwap) {
		switch (bytesNum) {
			case 1: dst = src[0]; break;
			case 2: dst = SHIFT_OR_2(src[0], src[1]); break;
			case 3: dst = SHIFT_OR_3(src[0], src[1], src[2]); break;
			case 4: dst = SHIFT_OR_4(src[0], src[1], src[2], src[3]); break;
		}
	} else {
		// no swap
		switch (bytesNum) {
			case 1: dst = src[0]; break;
			case 2: dst = SHIFT_OR_2(src[1], src[0]); break;
			case 3: dst = SHIFT_OR_3(src[2], src[1], src[0]); break;
			case 4: dst = SHIFT_OR_4(src[3], src[2], src[1], src[0]); break;
		}
	}

	return 0;
}

int LibUtil_DemoEndian(void)
{
	{
		u32 a;
		u8 b[4];
		b[0]=0x11; b[1]=0x22; b[2]=0x33; b[3]=0x44;
		LibUtil_BytesToInt32(a, b, true);
		printf("a = 0x%08X\n", a);
		ARRAYDUMPX2(b, 4);

		b[0]=0x11; b[1]=0x22; b[2]=0x33; b[3]=0x44;
		LibUtil_BytesToInt32(a, b, false);
		printf("a = 0x%08X\n", a);
		ARRAYDUMPX2(b, 4);

		a = 0xAABBCCDD;
		LibUtil_Int32ToBytes(b, a, true);
		printf("a = 0x%08X\n", a);
		ARRAYDUMPX2(b, 4);

		a = 0xAABBCCDD;
		LibUtil_Int32ToBytes(b, a, false);
		printf("a = 0x%08X\n", a);
		ARRAYDUMPX2(b, 4);
	}

	{
		u16 a;
		u8 b[4];
		b[0]=0x99; b[1]=0xAA;
		LibUtil_BytesToInt16(a, b, true);
		printf("a = 0x%04X\n", a);
		ARRAYDUMPX2(b, 2);

		b[0]=0x99; b[1]=0xAA;
		LibUtil_BytesToInt16(a, b, false);
		printf("a = 0x%04X\n", a);
		ARRAYDUMPX2(b, 2);

		a = 0x5566;
		LibUtil_Int16ToBytes(b, a, true);
		printf("a = 0x%04X\n", a);
		ARRAYDUMPX2(b, 2);

		a = 0x5566;
		LibUtil_Int16ToBytes(b, a, false);
		printf("a = 0x%04X\n", a);
		ARRAYDUMPX2(b, 2);
	}
	return 0;
}

s32 gLibUtil_Max = 0;
s32 gLibUtil_Min = 0;
int LibUtil_MaxMinMgr_Init(s32 initVal /* = 0 */)
{
	gLibUtil_Max = initVal;
	gLibUtil_Min = initVal;
	return 0;
}

int LibUtil_MaxMinMgr_Input(s32 inVal)
{
	if (inVal > gLibUtil_Max)
		gLibUtil_Max = inVal;

	if (inVal < gLibUtil_Min)
		gLibUtil_Min = inVal;

	return 0;
}

s32 LibUtil_MaxMinMgr_GetMax(void)
{
	return gLibUtil_Max;
}

s32 LibUtil_MaxMinMgr_GetMin(void)
{
	return gLibUtil_Min;
}

int LibUtil_GetTrueBitIndexOfU4(u8 in)
{
    BASIC_ASSERT((in & 0xF0) == 0);

    // 1101
    // 3210
    switch (in)
    {
        case  0: return -1;
        case  1: return 0;
        case  2: return 1;
        case  3: return 0;
        case  4: return 2;
        case  5: return 0;
        case  6: return 1;
        case  7: return 0;
        case  8: return 3;
        case  9: return 0;
        case 10: return 1;
        case 11: return 0;
        case 12: return 2;
        case 13: return 0;
        case 14: return 1;
        case 15: return 0;
    }
    return -2;
}

int LibUtil_GetFalseBitIndexOfU4(u8 in)
{
    BASIC_ASSERT((in & 0xF0) == 0);

    // 1101
    // 3210
    switch (in)
    {
        case  0: return 0;
        case  1: return 1;
        case  2: return 0;
        case  3: return 2;
        case  4: return 0;
        case  5: return 1;
        case  6: return 0;
        case  7: return 3;
        case  8: return 0;
        case  9: return 1;
        case 10: return 0;
        case 11: return 2;
        case 12: return 0;
        case 13: return 1;
        case 14: return 0;
        case 15: return -1;
    }
    return -2;
}

int LibUtil_GetTrueBitIndexOfU8(u8 in)
{
    int ret;

    ret = LibUtil_GetTrueBitIndexOfU4(in & 0x0F);
    if(ret < 0) {
        ret = LibUtil_GetTrueBitIndexOfU4(in >> 4);
        if(ret < 0)
            return ret;
        else
            ret = ret + 4;
    }

    return ret;
}

int LibUtil_GetFalseBitIndexOfU8(u8 in)
{
    int ret;

    ret = LibUtil_GetFalseBitIndexOfU4(in & 0x0F);
    if(ret < 0) {
        ret = LibUtil_GetFalseBitIndexOfU4(in >> 4);
        if(ret < 0)
            return ret;
        else
            ret = ret + 4;
    }

    return ret;
}