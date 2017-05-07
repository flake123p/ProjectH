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
	if (isSrcBigEndian) {
		dst = SHIFT_OR_4(src[0], src[1], src[2], src[3]);
	} else {
		dst = SHIFT_OR_4(src[3], src[2], src[1], src[0]);
	}
	return 0;
}

int LibUtil_Int32ToBytes(u8 *dst, u32 &src, bool isDstBigEndian)
{
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
	return 0;
}

int LibUtil_BytesToInt16(u16 &dst, u8 *src, bool isSrcBigEndian)
{
	if (isSrcBigEndian) {
		dst = SHIFT_OR_2(src[0], src[1]);
	} else {
		dst = SHIFT_OR_2(src[1], src[0]);
	}
	return 0;
}

int LibUtil_Int16ToBytes(u8 *dst, u16 &src, bool isDstBigEndian)
{
	if (isDstBigEndian) {
		dst[0] = src>>8;
		dst[1] = src;
	} else {
		dst[1] = src>>8;
		dst[0] = src;
	}
	return 0;
}

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