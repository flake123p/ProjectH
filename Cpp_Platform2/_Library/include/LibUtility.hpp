
#ifndef _LIB_UTILITY_HPP_INCLUDED_

#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================

// also used in power of 2 test
#define REMOVE_RIGHTMOST_1(x) ((x)&((x)-1))
// isolate  the  rightmost 1-bit, (e.g., 01011000 -> 00001000)
#define ISOLATE_RIGHTMOST_1(x) ((x)&(-(x)))
/*
	Useful function from <stdlib.h>
*/
void LibUtil_Init(void);
void LibUtil_Uninit(void);

void LibUtil_InitRand(int do_lock = 1);
u32 LibUtil_InitRandMaxBitNum(void);
/*
    Affected by RAND_MAX
    please use LibUtil_Print_RAND_MAX() to check
*/
int LibUtil_GetRand(void);
u8  LibUtil_GetRand8(void);
u16 LibUtil_GetRand16(void);
u32 LibUtil_GetRand32(void);
u8 LibUtil_GetRangeRand8(u8 min, u8 max);
u16 LibUtil_GetRangeRand16(u8 min, u8 max);
u32 LibUtil_GetRangeRand32(u8 min, u8 max);
void LibUtil_TestRand(void);

void LibUtil_Print_RAND_MAX(void);

void LibUtil_PrintBinary(u8 *ary, u32 len, int startFromHighAddress = 1);

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
int LibUtil_BytesAssembleU32(u32 &dst, u8 *src, u32 bytesNum, bool isSrcBigEndian);
int LibUtil_DemoEndian(void);

#include "My_Macros.h"
#define INT_TO_ARRAY(dst,src,currOffset,swap) LibUtil_IntSwapCopy(ADDRX(dst,currOffset),(u8 *)&(src),sizeof(src),swap);currOffset+=sizeof(src)
#define ARRAY_TO_INT(dst,src,currOffset,swap) LibUtil_IntSwapCopy((u8 *)&(dst),ADDRX(src,currOffset),sizeof(dst),swap);currOffset+=sizeof(dst)

int LibUtil_MaxMinMgr_Init(s32 initVal = 0);
int LibUtil_MaxMinMgr_Input(s32 inVal);
s32 LibUtil_MaxMinMgr_GetMax(void);
s32 LibUtil_MaxMinMgr_GetMin(void);



int LibUtil_GetTrueBitIndexOfU4(u8 in);
int LibUtil_GetFalseBitIndexOfU4(u8 in);
int LibUtil_GetTrueBitIndexOfU8(u8 in);
int LibUtil_GetFalseBitIndexOfU8(u8 in);
int LibUtil_GetTrueBitIndexOfU16(u16 in);
int LibUtil_GetFalseBitIndexOfU16(u16 in);

int LibUtil_GetBitPosition16(u16 in);
int LibUtil_GetBitPosition32(u32 in);

typedef struct {
    u16 unique_id;
    void *handle;
} LibUtil_UniqueID_Cell_t;
typedef struct LibUtil_UniqueID_Info_t {
    int recycle_ctr;
    u32 flag;
    u32 flag2;
    LibUtil_UniqueID_Cell_t *start_ptr[4];
} LibUtil_UniqueID_Info_t;
int LibUtil_UniqueID_Init(LibUtil_UniqueID_Info_t *info);
int LibUtil_UniqueID_Uninit(LibUtil_UniqueID_Info_t *info);
u16 LibUtil_UniqueID_GetID(LibUtil_UniqueID_Info_t *info, void *handle);
void *LibUtil_UniqueID_GetHandle(LibUtil_UniqueID_Info_t *info, u16 id);
int LibUtil_UniqueID_ReleaseID(LibUtil_UniqueID_Info_t *info, u16 id);
void LibUtil_UniqueID_Dump(LibUtil_UniqueID_Info_t *info);



u32 LibUtil_GetUniqueU32(void);
u16 LibUtil_GetUniqueU16(void);



void LibUtil_Demo(void);
void LibUtil_Demo2(void);


int LibUtil_AddInU64_TwoU32(u32 *hi, u32 *lo, u32 increment);

void LibUtil_SimpleDynamicMemDemo(void);

#define _LIB_UTILITY_HPP_INCLUDED_
#endif//_LIB_UTILITY_HPP_INCLUDED_
