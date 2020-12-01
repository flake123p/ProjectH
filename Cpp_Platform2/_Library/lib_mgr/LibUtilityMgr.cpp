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
#include "_LibMT.hpp"
#include "_LibError.hpp"
#include "LibUtility.hpp"
#include "LibU64.hpp"
#include "My_Basics.hpp"


LibMT_UtilMutex_t gLibUtilLock;
#define LIB_UTIL_LOCK    LibMT_UtilMutex_Lock(&gLibUtilLock);
#define LIB_UTIL_UNLOCK  LibMT_UtilMutex_Unlock(&gLibUtilLock);

void LibUtil_Init(void)
{
    LibMT_UtilMutex_Init(&gLibUtilLock);
}

void LibUtil_Uninit(void)
{
    LibMT_UtilMutex_Uninit(&gLibUtilLock);
}

unsigned int seed = 0;
u32 randMaxBitNum = 0xFFFFFFFF;
void LibUtil_InitRand(int do_lock/* = 1 */)
{
    if (do_lock) {
        LIB_UTIL_LOCK;
    }
    seed += time(NULL);
    srand(seed);

    LibUtil_InitRandMaxBitNum();

    if (do_lock) {
        LIB_UTIL_UNLOCK;
    }
}

u32 LibUtil_InitRandMaxBitNum(void)
{
    if (randMaxBitNum == 0xFFFFFFFF)
    {
        u32 mask = 1;
        randMaxBitNum = 0;
        while(1)
        {
            if (mask > RAND_MAX)
                break;

            mask = mask << 1;
            randMaxBitNum++;
        }
    }

    return randMaxBitNum;
}
/*
    Affected by RAND_MAX
    please use LibUtil_Print_RAND_MAX() to check
*/
int LibUtil_GetRand(void)
{
    return rand();
}

u8 LibUtil_GetRand8(void)
{
    u8 ret = ((rand()&0xFF) << 0);
    return ret;
}

u16 LibUtil_GetRand16(void)
{
    u16 ret = ((rand()&0xFF) << 8) | ((rand()&0xFF) << 0);
    return ret;
}

u32 LibUtil_GetRand32(void)
{
    u32 ret = ((rand()&0xFF) << 24) | ((rand()&0xFF) << 16) | ((rand()&0xFF) << 8) | ((rand()&0xFF) << 0);
    return ret;
}

u8 LibUtil_GetRangeRand8(u8 min, u8 max)
{
    u8 diff = max - min + 1;
    u8 ret = ((rand()&0xFF) << 0);
    ret = (ret%diff)+min;
    return ret;
}

u16 LibUtil_GetRangeRand16(u8 min, u8 max)
{
    u16 diff = max - min + 1;
    u16 ret = ((rand()&0xFF) << 8) | ((rand()&0xFF) << 0);
    ret = (ret%diff)+min;
    return ret;
}

u32 LibUtil_GetRangeRand32(u8 min, u8 max)
{
    u32 diff = max - min + 1;
    u32 ret = ((rand()&0xFF) << 24) | ((rand()&0xFF) << 16) | ((rand()&0xFF) << 8) | ((rand()&0xFF) << 0);
    ret = (ret%diff)+min;
    return ret;
}

void LibUtil_TestRand(void)
{
#define AMOUNT (20)
    LibU64_t u64Data;
    u32 average;

    LibUtil_InitRand();
    LibUtil_Print_RAND_MAX();

    u64Data.hi = 0;
    u64Data.lo  = 0;
    FOR_I(1<<AMOUNT) {
        LibU64_AddU32(&u64Data, (u32)LibUtil_GetRand());
    }
    average = (u64Data.hi << (32-AMOUNT)) | (u64Data.lo >> AMOUNT);
    printf("Average of LibUtil_GetRand()   is: %12u (0x%08X)\n", average, average);

    u64Data.hi = 0;
    u64Data.lo  = 0;
    FOR_I(1<<AMOUNT) {
        LibU64_AddU32(&u64Data, (u32)LibUtil_GetRand8());
    }
    average = (u64Data.hi << (32-AMOUNT)) | (u64Data.lo >> AMOUNT);
    printf("Average of LibUtil_GetRand8()  is: %12u (0x%08X)\n", average, average);

    u64Data.hi = 0;
    u64Data.lo  = 0;
    FOR_I(1<<AMOUNT) {
        LibU64_AddU32(&u64Data, (u32)LibUtil_GetRand16());
    }
    average = (u64Data.hi << (32-AMOUNT)) | (u64Data.lo >> AMOUNT);
    printf("Average of LibUtil_GetRand16() is: %12u (0x%08X)\n", average, average);

    u64Data.hi = 0;
    u64Data.lo  = 0;
    FOR_I(1<<AMOUNT) {
        LibU64_AddU32(&u64Data, (u32)LibUtil_GetRand32());
    }
    average = (u64Data.hi << (32-AMOUNT)) | (u64Data.lo >> AMOUNT);
    printf("Average of LibUtil_GetRand32() is: %12u (0x%08X)\n", average, average);
}

void LibUtil_Print_RAND_MAX(void)
{
	printf("RAND_MAX = %d (0x%X), randMaxBitNum = %d\n", RAND_MAX, RAND_MAX, LibUtil_InitRandMaxBitNum());
}

void LibUtil_PrintBinary(u8 *ary, u32 len, int startFromHighAddress /*= 1*/)
{
    u32 i,j;
    u32 end;
    u8 curr;
    int bit;
    BASIC_ASSERT(len>=1);
    if (startFromHighAddress) {
        i = len-1;
        end = 0xFFFFFFFF;
    } else {
        i = 0;
        end = len;
    }
    while (i != end) {
        curr = ary[i];
        //printf("[%X]",curr);
        for(j = 0; j < 8; j++) {
            if (curr&0x80)
                bit = 1;
            else
                bit = 0;
            printf("%d", bit);
            curr = curr << 1;
        }
        printf(",");
        if (startFromHighAddress) {
            i--;
        } else {
            i++;
        }
    }
    printf("\n");
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

int LibUtil_GetTrueBitIndexOfU8_2(u8 in)
{
    int ret;

    ret = LibUtil_GetTrueBitIndexOfU4(in >> 4);
    if(ret < 0) {
        ret = LibUtil_GetTrueBitIndexOfU4(in & 0x0F);
        return ret;
    }

    return ret = ret + 4;;
}

int LibUtil_GetTrueBitIndexOfU16(u16 in)
{
    int ret;

    ret = LibUtil_GetTrueBitIndexOfU8(in & 0x00FF);
    if(ret < 0) {
        ret = LibUtil_GetTrueBitIndexOfU8(in >> 8);
        if(ret < 0)
            return ret;
        else
            ret = ret + 8;
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

int LibUtil_GetFalseBitIndexOfU16(u16 in)
{
    int ret;

    ret = LibUtil_GetFalseBitIndexOfU8(in & 0x00FF);
    if(ret < 0) {
        ret = LibUtil_GetFalseBitIndexOfU8(in >> 8);
        if(ret < 0)
            return ret;
        else
            ret = ret + 8;
    }

    return ret;
}

int LibUtil_GetBitPosition16(u16 in)
{
    switch (in)
    {
        case 0x1: return 0;
        case 0x2: return 1;
        case 0x4: return 2;
        case 0x8: return 3;
        case 0x10: return 4;
        case 0x20: return 5;
        case 0x40: return 6;
        case 0x80: return 7;
        case 0x100: return 8;
        case 0x200: return 9;
        case 0x400: return 10;
        case 0x800: return 11;
        case 0x1000: return 12;
        case 0x2000: return 13;
        case 0x4000: return 14;
        case 0x8000: return 15;
    }
    return -1;
}

int LibUtil_GetBitPosition32(u32 in)
{
    switch (in)
    {
        case 0x1: return 0;
        case 0x2: return 1;
        case 0x4: return 2;
        case 0x8: return 3;
        case 0x10: return 4;
        case 0x20: return 5;
        case 0x40: return 6;
        case 0x80: return 7;
        case 0x100: return 8;
        case 0x200: return 9;
        case 0x400: return 10;
        case 0x800: return 11;
        case 0x1000: return 12;
        case 0x2000: return 13;
        case 0x4000: return 14;
        case 0x8000: return 15;
        case 0x10000: return 16;
        case 0x20000: return 17;
        case 0x40000: return 18;
        case 0x80000: return 19;
        case 0x100000: return 20;
        case 0x200000: return 21;
        case 0x400000: return 22;
        case 0x800000: return 23;
        case 0x1000000: return 24;
        case 0x2000000: return 25;
        case 0x4000000: return 26;
        case 0x8000000: return 27;
        case 0x10000000: return 28;
        case 0x20000000: return 29;
        case 0x40000000: return 30;
        case 0x80000000: return 31;
    }
    return -1;
}

int LibUtil_UniqueID_Init(LibUtil_UniqueID_Info_t *info)
{
    info->recycle_ctr = 0;
    info->flag = 0xFFFFFFFF;
    info->flag2 = 0xFFFFFFFF;
    info->start_ptr[0] = (LibUtil_UniqueID_Cell_t *)malloc(16 * sizeof(LibUtil_UniqueID_Cell_t));
    if (NULL == info->start_ptr[0]) {
        return 1;
    }
    FOREACH_I(16)
    {
        info->start_ptr[0][i].unique_id = 0x8EFF; //0~0x0EFF
    }
    info->start_ptr[1] = NULL;
    info->start_ptr[2] = NULL;
    info->start_ptr[3] = NULL;
    return 0;
}

int LibUtil_UniqueID_Uninit(LibUtil_UniqueID_Info_t *info)
{
    FOREACH_I(4)
    {
        if (NULL != info->start_ptr[i]) {
            free(info->start_ptr[i]);
        }
    }
    return 0;
}

u16 LibUtil_UniqueID_GetID(LibUtil_UniqueID_Info_t *info, void *handle)
{
    int ary_index;
    int bitmap_index;
    u16 hash_index;
    u16 counter_index;

    //1.check flags & index
    do {
        if (info->flag & 0x0000FFFF) {
            bitmap_index = LibUtil_GetTrueBitIndexOfU16((u16)info->flag);
            ary_index = 0;
            CLEAR_BIT(info->flag, bitmap_index);
            break;
        }
        if (info->flag & 0xFFFF0000) {
            bitmap_index = LibUtil_GetTrueBitIndexOfU16((u16)(info->flag>>16));
            ary_index = 1;
            CLEAR_BIT(info->flag, bitmap_index+16);
            break;
        }
        if (info->flag2 & 0x0000FFFF) {
            bitmap_index = LibUtil_GetTrueBitIndexOfU16((u16)info->flag2);
            ary_index = 2;
            CLEAR_BIT(info->flag2, bitmap_index);
            break;
        }
        if (info->flag2 & 0xFFFF0000) {
            bitmap_index = LibUtil_GetTrueBitIndexOfU16((u16)(info->flag2>>16));
            ary_index = 3;
            CLEAR_BIT(info->flag2, bitmap_index+16);
            break;
        }
        return 0xFFF0;
    } while(0);

    if (info->start_ptr[ary_index] == NULL)
    {
        info->start_ptr[ary_index] = (LibUtil_UniqueID_Cell_t *)malloc(16 * sizeof(LibUtil_UniqueID_Cell_t));
        if (NULL == info->start_ptr[ary_index]) {
            return 0xFFF1;
        }
        FOREACH_I(16)
        {
            info->start_ptr[ary_index][i].unique_id = 0x8EFF; //0~0x0EFF
        }
    }

    hash_index = bitmap_index + (ary_index << 4);
    counter_index = info->start_ptr[ary_index][bitmap_index].unique_id & 0x0FC0;
    if (counter_index == 0x0EC0) {
        counter_index = 0;
    } else {
        counter_index += 0x40;
    }

    info->start_ptr[ary_index][bitmap_index].unique_id = counter_index | hash_index;
    info->start_ptr[ary_index][bitmap_index].handle = handle;

    return info->start_ptr[ary_index][bitmap_index].unique_id;
}

void *LibUtil_UniqueID_GetHandle(LibUtil_UniqueID_Info_t *info, u16 id)
{
    u16 ary_index = (id & 0x0030 ) >> 4;
    u16 cell_index = id & 0x000F;

    if (id == info->start_ptr[ary_index][cell_index].unique_id) {
        return info->start_ptr[ary_index][cell_index].handle;
    } else {
        return NULL;
    }
}

int LibUtil_UniqueID_ReleaseID(LibUtil_UniqueID_Info_t *info, u16 id)
{
    u16 ary_index = (id & 0x0030 ) >> 4;
    u16 cell_index = id & 0x000F;

    if (id != info->start_ptr[ary_index][cell_index].unique_id) {
        return 1;
    }

    info->start_ptr[ary_index][cell_index].unique_id |= 0x8000;

    switch (ary_index)
    {
        case 0: SET_BIT(info->flag, cell_index); break;
        case 1: SET_BIT(info->flag, cell_index + 16); break;
        case 2: SET_BIT(info->flag2, cell_index); break;
        case 3: SET_BIT(info->flag2, cell_index + 16); break;
    }

    info->recycle_ctr++;
    if (info->recycle_ctr == 10)
    {
        info->recycle_ctr = 0;
        if(info->flag & 0x0000FFFF)
        {
            if((info->flag & 0xFFFF0000) == 0xFFFF0000 && info->start_ptr[1] != NULL) {
                free(info->start_ptr[1]);
                info->start_ptr[1] = NULL;
            }
            if((info->flag2 & 0x0000FFFF) == 0x0000FFFF && info->start_ptr[2] != NULL) {
                free(info->start_ptr[2]);
                info->start_ptr[2] = NULL;
            }
            if((info->flag2 & 0xFFFF0000) == 0xFFFF0000 && info->start_ptr[3] != NULL) {
                free(info->start_ptr[3]);
                info->start_ptr[3] = NULL;
            }
        }
    }

    return 0;
}

void LibUtil_UniqueID_Dump(LibUtil_UniqueID_Info_t *info)
{
    printf("flag = 0x%08X, flag2 = 0x%08X\n", info->flag, info->flag2);
    FOREACH_I(16)
    {
        DUMPNX(info->start_ptr[0][i].unique_id);
    }
    if (info->start_ptr[1] != NULL)
    {
        FOREACH_I(16)
        {
            DUMPNX(info->start_ptr[1][i].unique_id);
        }
    }
    if (info->start_ptr[2] != NULL)
    {
        FOREACH_I(16)
        {
            DUMPNX(info->start_ptr[2][i].unique_id);
        }
    }
    if (info->start_ptr[3] != NULL)
    {
        FOREACH_I(16)
        {
            DUMPNX(info->start_ptr[3][i].unique_id);
        }
    }
}

static int gLibUtil_GetUniqueU32_Inited = 0;
static u32 gLibUtil_GetUniqueU32_Base = 0;
static u32 gLibUtil_GetUniqueU32_Increment = 0;
u32 LibUtil_GetUniqueU32(void)
{
    LIB_UTIL_LOCK;
    if (0 == gLibUtil_GetUniqueU32_Inited)
    {
        gLibUtil_GetUniqueU32_Inited = 1;

        LibUtil_InitRand(0);
        gLibUtil_GetUniqueU32_Base = LibUtil_GetRand32();
        gLibUtil_GetUniqueU32_Increment = LibUtil_GetRand32();
        gLibUtil_GetUniqueU32_Increment |= 0x00000001;
    }

    gLibUtil_GetUniqueU32_Base += gLibUtil_GetUniqueU32_Increment;
    if (gLibUtil_GetUniqueU32_Base == 0)
        gLibUtil_GetUniqueU32_Base += gLibUtil_GetUniqueU32_Increment;

    LIB_UTIL_UNLOCK;
    return gLibUtil_GetUniqueU32_Base;
}

static int gLibUtil_GetUniqueU16_Inited = 0;
static u16 gLibUtil_GetUniqueU16_Base = 0;
static u16 gLibUtil_GetUniqueU16_Increment = 0;
u16 LibUtil_GetUniqueU16(void)
{
    LIB_UTIL_LOCK;
    if (0 == gLibUtil_GetUniqueU16_Inited)
    {
        gLibUtil_GetUniqueU16_Inited = 1;

        LibUtil_InitRand(0);
        gLibUtil_GetUniqueU16_Base = LibUtil_GetRand16();
        gLibUtil_GetUniqueU16_Increment = LibUtil_GetRand16();
        gLibUtil_GetUniqueU16_Increment |= 0x0001;
    }

    gLibUtil_GetUniqueU16_Base += gLibUtil_GetUniqueU16_Increment;
    if (gLibUtil_GetUniqueU16_Base == 0)
        gLibUtil_GetUniqueU16_Base += gLibUtil_GetUniqueU16_Increment;

    LIB_UTIL_UNLOCK;
    return gLibUtil_GetUniqueU16_Base;
}

void LibUtil_Demo(void)
{
    int ret;
    u8 map = 0xFF;

    FOREACH_I(9)
    {
        ret = LibUtil_GetTrueBitIndexOfU8_2(map);
        DUMPD(ret);DUMPND(i);
        if (ret >= 0) {
            CLEAR_BIT(map, ret);
        } else {
            printf("FULL\n");
            break;
        }
    }
    map = 0x10;
    FOREACH_I(9)
    {
        ret = LibUtil_GetTrueBitIndexOfU8_2(map);
        DUMPD(ret);DUMPND(i);
        if (ret >= 0) {
            CLEAR_BIT(map, ret);
        } else {
            printf("FULL\n");
            break;
        }
    }
}

void LibUtil_Demo2(void)
{
    LibUtil_UniqueID_Info_t demo_uniqueID_info;
    int ret = 0;
    u16 id = 0;
    LibUtil_UniqueID_Init(&demo_uniqueID_info);

    //id = LibUtil_UniqueID_GetID(&demo_uniqueID_info, NULL);
    FOREACH_I(0x11) {
        //ret = LibUtil_UniqueID_ReleaseID(&demo_uniqueID_info, id);
        //BASIC_ASSERT(ret == 0);
        id = LibUtil_UniqueID_GetID(&demo_uniqueID_info, ((u8 *)0) + i + 1);
    }
    ret = LibUtil_UniqueID_ReleaseID(&demo_uniqueID_info, 0x0010);
    BASIC_ASSERT(ret == 0);

    FOREACH_I(0x09) {
        LibUtil_UniqueID_ReleaseID(&demo_uniqueID_info, i);
    }

    LibUtil_UniqueID_Dump(&demo_uniqueID_info);

    //u8 *x = (u8 *)LibUtil_UniqueID_GetHandle(&demo_uniqueID_info, 0x0020);
    //DUMPNA(x);
    LibUtil_UniqueID_Uninit(&demo_uniqueID_info);

    id=id;ret=ret;
}

int LibUtil_AddInU64_TwoU32(u32 *hi, u32 *lo, u32 increment)
{
    int isCarryHappened = 0;
    *lo += increment;

    if (*lo < increment) {
        isCarryHappened = 1;
    }

    if (isCarryHappened) {
        *hi += 1;
        if (*hi == 0) {
            return 1;
        }
    }

    return 0;
}

// return index
int LibUtil_BitmapGet(u32 *bitmap32, u32 *isolatedBitmap)
{
    if (*bitmap32 == 0)
        return -1;

    u32 isolatedBit = ISOLATE_RIGHTMOST_1(*bitmap32);
    int index = LibUtil_GetBitPosition32(isolatedBit);
    BASIC_ASSERT(index != -1);

    //clear bitmap
    //*bitmap32 = (*bitmap32) & (~isolatedBit);
    *isolatedBitmap = isolatedBit;
    return index;
}

int LibUtil_BitmapRelease(u32 *bitmap32, int index)
{
    u32 isolatedBit = 1 << index;
    *bitmap32 |= isolatedBit;
    return 0;
}

int LibUtil_2TiersBitmapGet(u32 *tier1, u32 *tier2, OUT int *index1, OUT int *index2)
{
    u32 iso1, iso2;
    int idx1 = LibUtil_BitmapGet(tier1, &iso1);
    int idx2 = LibUtil_BitmapGet(tier2+idx1, &iso2);

    tier2[idx1] &= (~iso2);
    if (tier2[idx1] == 0) {
        *tier1 &= (~iso1);
    }

    *index1 = idx1;
    *index2 = idx2;
    return idx1;
}

int LibUtil_2TiersBitmapRelease(u32 *tier1, u32 *tier2, int index1, int index2)
{
    LibUtil_BitmapRelease(tier2+index1, index2);
    LibUtil_BitmapRelease(tier1, index1);
    return 0;
}

void LibUtil_SimpleDynamicMemDemo(void)
{
    // 4 x 4, 2 tiers
    u32 tier1 = 0x0F;
    u32 tier2[] = {0x01, 0x0F, 0x0F, 0x0F};

    int index1, index2;

    LibUtil_2TiersBitmapGet(&tier1, tier2, &index1, &index2);
    printf(" index1=%d, index2=%d\n", index1, index2);
    DUMPNX(tier1);
    ARRAYDUMPX2(tier2, 4);

    LibUtil_2TiersBitmapGet(&tier1, tier2, &index1, &index2);
    LibUtil_2TiersBitmapGet(&tier1, tier2, &index1, &index2);
    LibUtil_2TiersBitmapGet(&tier1, tier2, &index1, &index2);
    LibUtil_2TiersBitmapGet(&tier1, tier2, &index1, &index2);
    LibUtil_2TiersBitmapRelease(&tier1, tier2, 0, 31);
    printf(" index1=%d, index2=%d\n", index1, index2);
    DUMPNX(tier1);
    ARRAYDUMPX2(tier2, 4);
}