
#ifndef _LIB_U64_HPP_INCLUDED_

#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================

typedef struct {
    u32 hi;
    u32 lo;
} LibU64_t;

typedef enum {
    A_IS_GREATER = 0x01,
    B_IS_GREATER = 0x02,
    A_B_ARE_EQUAL = 0x04,
} LIB_U64_COMPARE_t;
//GTE stands for Greater Than or Equal
#define A_IS_GTE (A_IS_GREATER|A_B_ARE_EQUAL)
#define B_IS_GTE (B_IS_GREATER|A_B_ARE_EQUAL)

int LibU64_AddU32(LibU64_t *p_u64Data, u32 increment); //return 1 if u64 overflow
int LibU64_AddU64(LibU64_t *p_u64Data, LibU64_t *p_increment); //return 1 if u64 overflow

void LibU64_Demo(void);
int LibU64_Mul32To64(u32 a, u32 x, LibU64_t *pU64result); //return 1 if u64 overflow
int LibU64_Div32To64(LibU64_t *dividend64, u32 divisor32, LibU64_t *quotient, LibU64_t *remainder);

LIB_U64_COMPARE_t LibU64_Compare(LibU64_t *a, LibU64_t *b);
LIB_U64_COMPARE_t LibU64_Compare2(u32 aHi, u32 aLo, LibU64_t *b);
LIB_U64_COMPARE_t LibU64_Compare3(u32 aHi, u32 aLo, u32 bHi, u32 bLo);

int LibU48_Add48To48(LibU64_t *from, LibU64_t *increment, LibU64_t *result); //return 1 if u64 overflow
int LibU48_Sub48To48(LibU64_t *from, LibU64_t *decrement, LibU64_t *result); //return 1 if u64 overflow
int LibU48_Mul32To48(u32 a, u32 x, LibU64_t *result);
int LibU48_Div32To48(LibU64_t *dividend64, u32 divisor32, LibU64_t *quotient, u32 *remainder);
LIB_U64_COMPARE_t LibU48_Diff48(LibU64_t *a, LibU64_t *b, LibU64_t *result, u32 turnaround_mask_63_32);
#define ADD48(from,increment,result) LibU48_Add48To48((LibU64_t *)from, (LibU64_t *)increment, (LibU64_t *)result)
#define SUB48(from,decrement,result) LibU48_Sub48To48((LibU64_t *)from, (LibU64_t *)decrement, (LibU64_t *)result)
#define MUL48S(a,x,result)           LibU48_Mul32To48(a, x, (LibU64_t *)result)
#define DIV48S(dividend64,divisor32,quotient,p_remainder) LibU48_Div32To48((LibU64_t *)dividend64, divisor32, (LibU64_t *)quotient, p_remainder)
#define DIFF48(a,b,result,mask)      LibU48_Diff48((LibU64_t *)a, (LibU64_t *)b, (LibU64_t *)result, mask)

int LibU64_TestAll(void);
int LibU48_TestAll(void);

#define _LIB_U64_HPP_INCLUDED_
#endif//_LIB_U64_HPP_INCLUDED_