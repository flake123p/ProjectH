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

#define ENABLE_64_BIT_TEST ( 1 )

int LibU64_Div32_LongDivision(u32 dividend, u32 divisor32, u32 *quotient, u32 *remainder)
{
    //u32 q63_32 = dividend64->hi / divisor32;
    //u32 r63_32 = dividend64->hi % divisor32;
    u32 q31_0 = 0;
    //u32 divisorComp = ~divisor32;
    //u32 dividend64_lo = dividend64->lo;

    //int carry = 0;
    u32 temp = 0;
    //starting shifting remainder with lo 32 to get q31_0
    for (int i = 0; i < 32; i++)
    {
        //if (temp & 0x80000000)
        //    carry = 1;
        //else
        //    carry = 0;

        // update temp
        temp = (temp << 1) + ((dividend & 0x80000000) ? 1 : 0);
        dividend = dividend << 1;
        //if (carry)
        //{
        //    temp += divisorComp + 1;
        //    q31_0 = (q31_0 << 1) | 1;
        //}
        //else
        {
            if (temp >= divisor32)
            {
                temp -= divisor32;
                q31_0 = (q31_0 << 1) | 1;
            }
            else
            {
                q31_0 = (q31_0 << 1) | 0;
            }
        }
    }
    *remainder = temp;
    *quotient = q31_0;

    return 0;
}

int LibU64_AddU32(LibU64_t *p_u64Data, u32 increment) //return 1 if u64 overflow
{
    int isCarryHappened = 0;
    p_u64Data->lo+= increment;

    if (p_u64Data->lo < increment) {
        isCarryHappened = 1;
    }

    if (isCarryHappened) {
        p_u64Data->hi += 1;
        if (p_u64Data->hi == 0) {
            return 1;
        }
    }

    return 0;
}

int LibU64_AddU64(LibU64_t *p_u64Data, LibU64_t *p_increment) //return 1 if u64 overflow
{
    int ret = 0;
    u32 isCarryHappened = 0;
    p_u64Data->lo += p_increment->lo;

    if (p_u64Data->lo < p_increment->lo) {
        isCarryHappened = 1;
    }

    p_u64Data->hi += p_increment->hi + isCarryHappened;
    if (p_u64Data->hi < p_increment->hi) {
        ret = 1;
    }

    return ret;
}

void LibU64_Demo(void)
{
    {
        LibU64_t u64Data = {
            .hi = 0x00000010,
            .lo  = 0xFFFFFFFE
        };
        DUMPNX(u64Data.hi);
        DUMPNX(u64Data.lo);

        printf("add 1\n");
        LibU64_AddU32(&u64Data, 1);
        DUMPNX(u64Data.hi);
        DUMPNX(u64Data.lo);

        printf("add 1\n");
        LibU64_AddU32(&u64Data, 1);
        DUMPNX(u64Data.hi);
        DUMPNX(u64Data.lo);

        printf("add 0xFFFFFFFF\n");
        LibU64_AddU32(&u64Data, 0xFFFFFFFF);
        DUMPNX(u64Data.hi);
        DUMPNX(u64Data.lo);

        printf("add 0xFFFFFFFF\n");
        LibU64_AddU32(&u64Data, 0xFFFFFFFF);
        DUMPNX(u64Data.hi);
        DUMPNX(u64Data.lo);
        printf("////////////\n");
    }
    {
        LibU64_t u64DataA = {
            .hi = 0x00000010,
            .lo  = 0xFFFFFFFE
        };
        LibU64_t u64DataB = {
            .hi = 0x00000010,
            .lo  = 0x00000001
        };
        int ret = 7;
        DUMPD(ret);
        DUMPX(u64DataA.hi);
        DUMPNX(u64DataA.lo);
        ret = LibU64_AddU64(&u64DataA, &u64DataB);
        DUMPD(ret);
        DUMPX(u64DataA.hi);
        DUMPNX(u64DataA.lo);
        ret = LibU64_AddU64(&u64DataA, &u64DataB);
        DUMPD(ret);
        DUMPX(u64DataA.hi);
        DUMPNX(u64DataA.lo);
        printf("////////////\n");
    }
    {
        LibU64_t u64DataA = {
            .hi = 0xFFFFFFFF,
            .lo  = 0xFFFFFFFE
        };
        LibU64_t u64DataB = {
            .hi = 0x00000000,
            .lo  = 0x00000001
        };
        int ret = 7;
        DUMPD(ret);
        DUMPX(u64DataA.hi);
        DUMPNX(u64DataA.lo);
        ret = LibU64_AddU64(&u64DataA, &u64DataB);
        DUMPD(ret);
        DUMPX(u64DataA.hi);
        DUMPNX(u64DataA.lo);
        ret = LibU64_AddU64(&u64DataA, &u64DataB);
        DUMPD(ret);
        DUMPX(u64DataA.hi);
        DUMPNX(u64DataA.lo);
        printf("////////////\n");
    }
    {
        LibU64_t u64DataA = {
            .hi = 0xFFFFFFFE,
            .lo  = 0xFFFFFFFF
        };
        LibU64_t u64DataB = {
            .hi = 0x00000001,
            .lo  = 0x00000000
        };
        int ret = 7;
        DUMPD(ret);
        DUMPX(u64DataA.hi);
        DUMPNX(u64DataA.lo);
        ret = LibU64_AddU64(&u64DataA, &u64DataB);
        DUMPD(ret);
        DUMPX(u64DataA.hi);
        DUMPNX(u64DataA.lo);
        ret = LibU64_AddU64(&u64DataA, &u64DataB);
        DUMPD(ret);
        DUMPX(u64DataA.hi);
        DUMPNX(u64DataA.lo);
        printf("////////////\n");
    }
    {
        LibU64_t u64DataA = {
            .hi = 0xFFFFFFFE,
            .lo  = 0xFFFFFFFF
        };
        LibU64_t u64DataB = {
            .hi = 0x00000001,
            .lo  = 0x00000001
        };
        int ret = 7;
        DUMPD(ret);
        DUMPX(u64DataA.hi);
        DUMPNX(u64DataA.lo);
        ret = LibU64_AddU64(&u64DataA, &u64DataB);
        DUMPD(ret);
        DUMPX(u64DataA.hi);
        DUMPNX(u64DataA.lo);
        ret = LibU64_AddU64(&u64DataA, &u64DataB);
        DUMPD(ret);
        DUMPX(u64DataA.hi);
        DUMPNX(u64DataA.lo);
        printf("////////////\n");
    }
}

int LibU64_Add64To64(LibU64_t *from, LibU64_t *increment, LibU64_t *result) //return 1 if u64 overflow
{
    int ret = 0;
    u32 isCarryHappened = 0;
    u32 temp;
    temp = from->lo + increment->lo;

    if (temp < from->lo) {
        isCarryHappened = 1;
    }
    result->lo = temp;

    temp = from->hi + increment->hi + isCarryHappened;
    if (temp < from->hi) {
        ret = 1;
    }
    result->hi = temp;

    return ret;
}

int LibU64_Sub64To64(LibU64_t *from, LibU64_t *decrement, LibU64_t *result) //return 1 if u64 overflow
{
    int isCarryHappened = 0;

    if(from->lo < decrement->lo) {
        isCarryHappened = 1;
        result->lo = 0xFFFFFFFF - decrement->lo + from->lo + 1;
    } else {
        result->lo = from->lo - decrement->lo;
    }

    if (isCarryHappened) {
        if (from->hi) {
            result->hi = from->hi - 1;
            isCarryHappened = 0;
        } else {
            result->hi = 0xFFFFFFFF;
            isCarryHappened = 1;
        }
    } else {
        result->hi = from->hi;
    }

    if(result->hi < decrement->hi) {
        isCarryHappened = 1;
        result->hi = 0xFFFFFFFF - decrement->hi + result->hi + 1;
    } else {
        result->hi = result->hi - decrement->hi;
    }

    return isCarryHappened;
}

int LibU64_Mul32To64(u32 a, u32 x, LibU64_t *pU64result)
{
    uint32_t ah = a >> 16;
    uint32_t al = a & 0xFFFF;
    uint32_t xh = x >> 16;
    uint32_t xl = x & 0xFFFF;
    uint32_t alxl = al * xl;
    uint32_t alxh = al * xh;
    uint32_t ahxl = ah * xl;
    uint32_t ahxh = ah * xh;
    uint32_t temp;

    //calculte result[31:16] = temp[15:0], temp[31:16] is carry 
    temp = (alxl>>16) + (alxh&0xFFFF) + (ahxl&0xFFFF);
    pU64result->lo = (temp<<16)|(0xFFFF&alxl);
    //calculate result[63:32] = temp[31:0]
    temp = (temp>>16)/*carry*/ + (alxh>>16) + (ahxl>>16) + ahxh;
    pU64result->hi = temp;

    return 0;
}

int LibU64_Div32To64(LibU64_t *dividend64, u32 divisor32, LibU64_t *quotient, u32 *remainder)
{
    u32 q63_32 = dividend64->hi / divisor32;
    u32 r63_32 = dividend64->hi % divisor32;
    u32 q31_0 = 0;
    u32 divisorComp = ~divisor32;
    u32 dividend64_lo = dividend64->lo;

    int carry = 0;
    u32 temp = r63_32;
    //starting shifting remainder with lo 32 to get q31_0
    for (int i = 0; i < 32; i++)
    {
        if (temp & 0x80000000)
            carry = 1;
        else
            carry = 0;
        // update temp
        temp = (temp << 1) + ((dividend64_lo & 0x80000000) ? 1 : 0);
        dividend64_lo = dividend64_lo << 1;
        if (carry)
        {
            temp += divisorComp + 1;
            q31_0 = (q31_0 << 1) | 1;
        }
        else
        {
            if (temp >= divisor32)
            {
                temp -= divisor32;
                q31_0 = (q31_0 << 1) | 1;
            }
            else
            {
                q31_0 = (q31_0 << 1) | 0;
            }
        }
    }
    *remainder = temp;
    quotient->hi = q63_32;
    quotient->lo = q31_0;

    return 0;
}

LIB_U64_COMPARE_t LibU64_Diff64(LibU64_t *a, LibU64_t *b, LibU64_t *result, u32 turnaround_mask_63_32)
{
    LIB_U64_COMPARE_t ret;
    if (a->hi > b->hi) {
        ret = A_IS_GREATER;
    } else if (a->hi < b->hi) {
        ret = B_IS_GREATER;
    } else {
        if (a->lo > b->lo) {
        ret = A_IS_GREATER;
        } else if (a->lo < b->lo) {
            ret = B_IS_GREATER;
        } else {
            ret = A_B_ARE_EQUAL;
        }
    }

    if (ret == A_IS_GREATER) {
        LibU64_Sub64To64(a, b, result);
    } else {
        LibU64_Sub64To64(b, a, result);
    }

    // result of diff
    if (result->hi >= turnaround_mask_63_32)
    {
        // do revert
        result->hi = 0xFFFFFFFF - result->hi;
        result->lo = 0xFFFFFFFF - result->lo;
        result->lo += 1;
        if (result->lo == 0)
            result->hi += 1;
        if (ret == A_IS_GREATER)
            ret = B_IS_GREATER;
        else if (ret == B_IS_GREATER)
            ret = A_IS_GREATER;
    }
    return ret;
}

LIB_U64_COMPARE_t LibU64_Compare(LibU64_t *a, LibU64_t *b)
{
    LIB_U64_COMPARE_t ret;
    if (a->hi > b->hi) {
        ret = A_IS_GREATER;
    } else if (a->hi < b->hi) {
        ret = B_IS_GREATER;
    } else {
        if (a->lo > b->lo) {
        ret = A_IS_GREATER;
        } else if (a->lo < b->lo) {
            ret = B_IS_GREATER;
        } else {
            ret = A_B_ARE_EQUAL;
        }
    }
    return ret;
}

LIB_U64_COMPARE_t LibU64_Compare2(u32 aHi, u32 aLo, LibU64_t *b)
{
    LIB_U64_COMPARE_t ret;
    if (aHi > b->hi) {
        ret = A_IS_GREATER;
    } else if (aHi < b->hi) {
        ret = B_IS_GREATER;
    } else {
        if (aLo > b->lo) {
        ret = A_IS_GREATER;
        } else if (aLo < b->lo) {
            ret = B_IS_GREATER;
        } else {
            ret = A_B_ARE_EQUAL;
        }
    }
    return ret;
}

LIB_U64_COMPARE_t LibU64_Compare3(u32 aHi, u32 aLo, u32 bHi, u32 bLo)
{
    LIB_U64_COMPARE_t ret;
    if (aHi > bHi) {
        ret = A_IS_GREATER;
    } else if (aHi < bHi) {
        ret = B_IS_GREATER;
    } else {
        if (aLo > bLo) {
        ret = A_IS_GREATER;
        } else if (aLo < bLo) {
            ret = B_IS_GREATER;
        } else {
            ret = A_B_ARE_EQUAL;
        }
    }
    return ret;
}

int LibU64_TestAdd(u32 testCount)
{
    int isError = 0;
#if ENABLE_64_BIT_TEST
    int ret, of;
    LibU64_t from, increment, result;
    uint64_t from64, increment64, result64;
    
    u32 ctr = 0;
    while (1)
    {
        isError = 0;
        if(ctr==testCount)
            break;
        ctr++;
        from.hi = LibUtil_GetRand32();
        from.lo = LibUtil_GetRand32();
        increment.hi = LibUtil_GetRand32();
        increment.lo = LibUtil_GetRand32();
        from64 = (((uint64_t)(from.hi)) << 32) | from.lo;
        increment64 =  (((uint64_t)(increment.hi)) << 32) | increment.lo;
        ret = LibU64_Add64To64(&from, &increment, &result);
        result64 = from64 + increment64;
        if (result64 < from64)
            of = 1;
        else
            of = 0;

        if (result.hi != (uint32_t)(result64>>32))
        {
            printf("result64.hi error\n");
            isError = 1;
        }
        if (result.lo != (uint32_t)(result64))
        {
            printf("quotient.lo error\n");
            isError = 1;
        }
        if (of != ret)
        {
            printf("overflow error\n");
            isError = 1;
        }
        if (isError)
        {
            DUMPNX32(from.hi);
            DUMPNX32(from.lo);
            DUMPNX32(increment.hi);
            DUMPNX32(increment.lo);
            DUMPNX32(result.hi);
            DUMPNX32(result.lo);
            DUMPNX32((uint32_t)(from64>>32));
            DUMPNX32((uint32_t)(from64));
            DUMPNX32((uint32_t)(increment64>>32));
            DUMPNX32((uint32_t)(increment64));
            DUMPNX32((uint32_t)(result64>>32));
            DUMPNX32((uint32_t)(result64));
            break;
        }
        printf("%s() PASSED...\n", __func__);
    }
#else
    printf("ENABLE_64_BIT_TEST is not enabled!! (%d)\n", ENABLE_64_BIT_TEST);
    isError= 2;
#endif
    printf("%s() done...\n", __func__);
    return isError;
}

int LibU64_TestSub(u32 testCount)
{
    int isError = 0;
#if ENABLE_64_BIT_TEST
    int ret, of;
    LibU64_t from, decrement, result;
    uint64_t from64, decrement64, result64;
    
    u32 ctr = 0;
    while (1)
    {
        isError = 0;
        if(ctr==testCount)
            break;
        ctr++;
        from.hi = LibUtil_GetRand32();
        from.lo = LibUtil_GetRand32();
        decrement.hi = LibUtil_GetRand32();
        decrement.lo = LibUtil_GetRand32();
        from64 = (((uint64_t)(from.hi)) << 32) | from.lo;
        decrement64 =  (((uint64_t)(decrement.hi)) << 32) | decrement.lo;
        ret = LibU64_Sub64To64(&from, &decrement, &result);
        result64 = from64 - decrement64;
        if (from64 < decrement64)
            of = 1;
        else
            of = 0;

        if (result.hi != (uint32_t)(result64>>32))
        {
            printf("result.hi error\n");
            isError = 1;
        }
        if (result.lo != (uint32_t)(result64))
        {
            printf("result.lo error\n");
            isError = 1;
        }
        if (of != ret)
        {
            printf("overflow error\n");
            isError = 1;
        }
        if (isError)
        {
            DUMPNX32(from.hi);
            DUMPNX32(from.lo);
            DUMPNX32(decrement.hi);
            DUMPNX32(decrement.lo);
            DUMPNX32(result.hi);
            DUMPNX32(result.lo);
            DUMPNX32((uint32_t)(from64>>32));
            DUMPNX32((uint32_t)(from64));
            DUMPNX32((uint32_t)(decrement64>>32));
            DUMPNX32((uint32_t)(decrement64));
            DUMPNX32((uint32_t)(result64>>32));
            DUMPNX32((uint32_t)(result64));
            break;
        }
        printf("%s() PASSED...\n", __func__);
    }
#else
    printf("ENABLE_64_BIT_TEST is not enabled!! (%d)\n", ENABLE_64_BIT_TEST);
    isError= 2;
#endif
    printf("%s() done...\n", __func__);
    return isError;
}

int LibU64_TestMul(u32 testCount)
{
    int isError = 0;
#if ENABLE_64_BIT_TEST
    u32 a, x;
    LibU64_t u64result;
    uint64_t a64, x64;
    
    u32 ctr = 0;
    while (1)
    {
        if(ctr==testCount)
            break;
        ctr++;
        a = LibUtil_GetRand32();
        x = LibUtil_GetRand32();
        LibU64_Mul32To64(a, x, &u64result);
        a64 = a;
        x64 = x;
        a64 = a64 * x64;
        if (u64result.hi != (uint32_t)(a64>>32))
        {
            printf("hi number error ans=0x%08X, result=0x%08X, a=0x%08X, x=0x%08X\n", (uint32_t)(a64>>32), u64result.hi, a, x);
            isError = 1;
            break;
        }
        if (u64result.lo != (uint32_t)(a64))
        {
            printf("lo number error ans=0x%08X, result=0x%08X, a=0x%08X, x=0x%08X\n", (uint32_t)(a64), u64result.lo, a, x);
            isError = 1;
            break;
        }
        printf("PASSED: hi=0x%08X, lo=0x%08X, a=0x%08X, x=0x%08X\n", u64result.hi, u64result.lo, a, x);
    }
#else
    printf("ENABLE_64_BIT_TEST is not enabled!! (%d)\n", ENABLE_64_BIT_TEST);
    isError= 2;
#endif
    printf("%s() done...\n", __func__);
    return isError;
}

int LibU64_TestDiv(u32 testCount)
{
    int isError = 0;
#if ENABLE_64_BIT_TEST
    u32 divisor, remainder;
    LibU64_t dividend, quotient;
    uint64_t dividend64, quotient64, remainder64;
    
    u32 ctr = 0;
    while (1)
    {
        if(ctr==testCount)
            break;
        ctr++;
        dividend.hi = LibUtil_GetRand32();
        dividend.lo = LibUtil_GetRand32();
        do {
            divisor = LibUtil_GetRand32();
        } while(divisor < 2);
        dividend64 = dividend.hi;
        dividend64 = dividend64 << 32 | dividend.lo;
        quotient64 = dividend64 / divisor;
        remainder64 = dividend64 % divisor;
        LibU64_Div32To64(&dividend, divisor, &quotient, &remainder);

        if (quotient.hi != (uint32_t)(quotient64>>32))
        {
            printf("quotient.hi error\n");
            printf("dividend64.hi=0x%08X  dividend64.lo=0x%08X  divisor=0x%08X\n", (uint32_t)(dividend64>>32), (uint32_t)dividend64, divisor);
            printf("quotient.hi=0x%08X  quotient.lo=0x%08X  remainder=0x%08X\n", quotient.hi, quotient.lo, remainder);
            printf("quotient64.hi=0x%08X  quotient64.lo=0x%08X  remainder64=0x%08X\n", (uint32_t)(quotient64>>32), (uint32_t)quotient64, (uint32_t)remainder64);
            isError = 1;
            break;
        }
        if (quotient.lo != (uint32_t)(quotient64))
        {
            printf("quotient.lo error\n");
            printf("dividend64.hi=0x%08X  dividend64.lo=0x%08X  divisor=0x%08X\n", (uint32_t)(dividend64>>32), (uint32_t)dividend64, divisor);
            printf("quotient.hi=0x%08X  quotient.lo=0x%08X  remainder=0x%08X\n", quotient.hi, quotient.lo, remainder);
            printf("quotient64.hi=0x%08X  quotient64.lo=0x%08X  remainder64=0x%08X\n", (uint32_t)(quotient64>>32), (uint32_t)quotient64, (uint32_t)remainder64);
            isError = 1;
            break;
        }
        if (remainder != (uint32_t)(remainder64))
        {
            printf("remainder error\n");
            printf("dividend64.hi=0x%08X  dividend64.lo=0x%08X  divisor=0x%08X\n", (uint32_t)(dividend64>>32), (uint32_t)dividend64, divisor);
            printf("quotient.hi=0x%08X  quotient.lo=0x%08X  remainder=0x%08X\n", quotient.hi, quotient.lo, remainder);
            printf("quotient64.hi=0x%08X  quotient64.lo=0x%08X  remainder64=0x%08X\n", (uint32_t)(quotient64>>32), (uint32_t)quotient64, (uint32_t)remainder64);
            isError = 1;
            break;
        }
        printf("%s() PASSED...\n", __func__);
    }
#else
    printf("ENABLE_64_BIT_TEST is not enabled!! (%d)\n", ENABLE_64_BIT_TEST);
    isError= 2;
#endif
    printf("%s() done...\n", __func__);
    return isError;
}

int LibU64_TestDiff_unit_test(u32 ah, u32 al, u32 bh, u32 bl, u32 turnaround_mask_63_32)
{
    int isError = 0;
#if ENABLE_64_BIT_TEST
    LIB_U64_COMPARE_t ret, ret64;
    LibU64_t a, b, result;
    uint64_t a64, b64, result64;

    a.hi = ah;
    a.lo = al;
    b.hi = bh;
    b.lo = bl;
    ret = LibU64_Diff64(&a, &b, &result, turnaround_mask_63_32);
    a64 = (((uint64_t)a.hi)<<32) | a.lo;
    b64 = (((uint64_t)b.hi)<<32) | b.lo;

    if (a64 > b64) {
        ret64 = A_IS_GREATER;
    } else if (a64 < b64) {
        ret64 = B_IS_GREATER;
    } else {
        ret64 = A_B_ARE_EQUAL;
    }

    if (ret64 == A_IS_GREATER) {
        result64 = a64 - b64;
    } else {
        result64 = b64 - a64;
    }
    // result of diff
    if (((uint32_t)(result64>>32)) >= turnaround_mask_63_32)
    {
        // do revert
        result64 = (~result64) + 1;
        if (ret64 == A_IS_GREATER)
            ret64 = B_IS_GREATER;
        else if (ret64 == B_IS_GREATER)
            ret64 = A_IS_GREATER;
    }
    //compare result
    if (result.hi != (uint32_t)(result64>>32))
    {
        printf("result.hi error\n");
        isError = 1;
    }
    if (result.lo != (uint32_t)(result64))
    {
        printf("result.lo error\n");
        isError = 1;
    }
    if (ret != ret64)
    {
        printf("ret error\n");
        isError = 1;
    }
    if (0 || isError)
    {
        DUMPNX32(a.hi);
        DUMPNX32(a.lo);
        DUMPNX32(b.hi);
        DUMPNX32(b.lo);
        DUMPNX32(result.hi);
        DUMPNX32(result.lo);
        DUMPNX32((uint32_t)(a64>>32));
        DUMPNX32((uint32_t)(a64));
        DUMPNX32((uint32_t)(b64>>32));
        DUMPNX32((uint32_t)(b64));
        DUMPNX32((uint32_t)(result64>>32));
        DUMPNX32((uint32_t)(result64));
        DUMPND(ret);
        DUMPND(ret64);
        DUMPND(isError);
        //break;
    }
#else
    printf("ENABLE_64_BIT_TEST is not enabled!! (%d)\n", ENABLE_64_BIT_TEST);
    isError= 2;
#endif
    printf("%s() done...\n", __func__);
    return isError;
}

int LibU64_TestDiff(u32 testCount)
{
    int isError = 0;
    u32 ctr = 0;
    //LibU64_TestDiff_unit_test(1,9,2,10);
    //LibU64_TestDiff_unit_test(0,0,0x00008FFF,0xFFFFFFFF, 0x00008000);
    while (1)
    {
        if(ctr==testCount)
            break;
        ctr++;
        isError = LibU64_TestDiff_unit_test(LibUtil_GetRand32(),LibUtil_GetRand32(),LibUtil_GetRand32(),LibUtil_GetRand32(), 0x00008000);
        if (isError)
            return isError;
    }
    printf("%s() done...\n", __func__);
    return isError;
}

int LibU64_TestAll(void)
{
#define TEST_COUNT 20000
    int ret = 0;
    LibUtil_InitRand();
    RETURN_CHK(ret, LibU64_TestAdd(TEST_COUNT));
    RETURN_CHK(ret, LibU64_TestSub(TEST_COUNT));
    RETURN_CHK(ret, LibU64_TestMul(TEST_COUNT));
    RETURN_CHK(ret, LibU64_TestDiv(TEST_COUNT));
    RETURN_CHK(ret, LibU64_TestDiff(TEST_COUNT));
    return ret;
#undef TEST_COUNT
}

int LibU48_Add48To48(LibU64_t *from, LibU64_t *increment, LibU64_t *result) //return 1 if u64 overflow
{
    int ret = 0;
    u32 isCarryHappened = 0;
    u32 temp;
    temp = from->lo + increment->lo;

    if (temp < from->lo) {
        isCarryHappened = 1;
    }
    result->lo = temp;

    //from->hi &= 0x0000FFFF;
    //increment->hi &= 0x0000FFFF;
    result->hi = from->hi + increment->hi + isCarryHappened;
    if (result->hi & 0xFFFF0000) {
        result->hi &= 0x0000FFFF;
        ret = 1;
    }

    return ret;
}

int LibU48_Sub48To48(LibU64_t *from, LibU64_t *decrement, LibU64_t *result) //return 1 if u64 overflow
{
    int isCarryHappened = 0;

    if(from->lo < decrement->lo) {
        isCarryHappened = 1;
        result->lo = 0xFFFFFFFF - decrement->lo + from->lo + 1;
    } else {
        result->lo = from->lo - decrement->lo;
    }

    result->hi = (from->hi - isCarryHappened) - decrement->hi;
    if (result->hi & 0xFFFF0000) {
        result->hi &= 0x0000FFFF;
        return 1;
    } else {
        return 0;
    }
}

int LibU48_Mul32To48(u32 a, u32 x, LibU64_t *result)
{
    uint32_t ah = a >> 16;
    uint32_t al = a & 0xFFFF;
    uint32_t xh = x >> 16;
    uint32_t xl = x & 0xFFFF;
    uint32_t alxl = al * xl;
    uint32_t alxh = al * xh;
    uint32_t ahxl = ah * xl;
    uint32_t ahxh = ah * xh;
    uint32_t temp;

    //calculte result[31:16] = temp[15:0], temp[31:16] is carry 
    temp = (alxl>>16) + (alxh&0xFFFF) + (ahxl&0xFFFF);
    result->lo = (temp<<16)|(0xFFFF&alxl);
    //calculate result[63:32] = temp[31:0]
    temp = (temp>>16)/*carry*/ + (alxh>>16) + (ahxl>>16) + ahxh;
    result->hi = temp;

    if (result->hi & 0xFFFF0000) {
        result->hi &= 0x0000FFFF;
        return 1;
    } else {
        return 0;
    }
    return 0;
}

int LibU48_Div32To48(LibU64_t *dividend64, u32 divisor32, LibU64_t *quotient, u32 *remainder)
{
    int i, carry;
    u32 q_47_16 = 0;
    u32 q_15_0 = 0;
    u32 temp = 0;
    u32 currDividend = dividend64->hi << 16;
    u32 divisorComp = ~divisor32;
    for (i = 0; i < 16; i++)
    {
        temp = (temp << 1) + ((currDividend & 0x80000000) ? 1 : 0);
        currDividend = currDividend << 1;

        if (temp >= divisor32)
        {
            temp -= divisor32;
            q_47_16 = (q_47_16 << 1) | 1;
        }
        else
        {
            q_47_16 = (q_47_16 << 1) | 0;
        }
    }
    currDividend = dividend64->lo;
    for (i = 0; i < 16; i++)
    {
        temp = (temp << 1) + ((currDividend & 0x80000000) ? 1 : 0);
        currDividend = currDividend << 1;

        if (temp >= divisor32)
        {
            temp -= divisor32;
            q_47_16 = (q_47_16 << 1) | 1;
        }
        else
        {
            q_47_16 = (q_47_16 << 1) | 0;
        }
    }
    for (i = 0; i < 16; i++)
    {
        if (temp & 0x80000000)
            carry = 1;
        else
            carry = 0;
        // update temp
        temp = (temp << 1) + ((currDividend & 0x80000000) ? 1 : 0);
        currDividend = currDividend << 1;
        if (carry)
        {
            temp += divisorComp + 1;
            q_15_0 = (q_15_0 << 1) | 1;
        }
        else
        {
            if (temp >= divisor32)
            {
                temp -= divisor32;
                q_15_0 = (q_15_0 << 1) | 1;
            }
            else
            {
                q_15_0 = (q_15_0 << 1) | 0;
            }
        }
    }

    *remainder = temp;
    quotient->hi = q_47_16 >> 16;
    quotient->lo = (q_47_16 << 16) | q_15_0;
    return 0;
}

LIB_U64_COMPARE_t LibU48_Diff48(LibU64_t *a, LibU64_t *b, LibU64_t *result, u32 turnaround_mask_63_32)
{
    LIB_U64_COMPARE_t ret = LibU64_Compare(a, b);

    if (ret == A_IS_GREATER) {
        LibU48_Sub48To48(a, b, result);
    } else {
        LibU48_Sub48To48(b, a, result);
    }

    // result of diff
    if (result->hi >= turnaround_mask_63_32)
    {
        // do revert
        result->hi = ~result->hi;
        result->lo = (~result->lo ) + 1;
        //result->lo += 1;
        if (result->lo == 0)
            result->hi += 1;
        result->hi = 0x0000FFFF & result->hi;
        if (ret == A_IS_GREATER)
            ret = B_IS_GREATER;
        else if (ret == B_IS_GREATER)
            ret = A_IS_GREATER;
    }
    return ret;
}

int LibU48_TestAdd(u32 testCount)
{
    int isError = 0;
#if ENABLE_64_BIT_TEST
    int ret, of;
    LibU64_t from, increment, result;
    uint64_t from64, increment64, result64;
    
    u32 ctr = 0;
    while (1)
    {
        isError = 0;
        if(ctr==testCount)
            break;
        ctr++;
        from.hi = LibUtil_GetRand16();
        from.lo = LibUtil_GetRand32();
        increment.hi = LibUtil_GetRand16();
        increment.lo = LibUtil_GetRand32();
        from64 = (((uint64_t)(from.hi)) << 32) | from.lo;
        increment64 =  (((uint64_t)(increment.hi)) << 32) | increment.lo;
        ret = LibU48_Add48To48(&from, &increment, &result);
        result64 = from64 + increment64;
        if (result64 & 0xFFFF000000000000) {
            of = 1;
            result64 &= 0x0000FFFFFFFFFFFF;
        } else {
            of = 0;
        }
        if (result.hi != (uint32_t)(result64>>32))
        {
            printf("result64.hi error\n");
            isError = 1;
        }
        if (result.lo != (uint32_t)(result64))
        {
            printf("quotient.lo error\n");
            isError = 1;
        }
        if (of != ret)
        {
            printf("overflow error\n");
            isError = 1;
        }
        if (isError)
        {
            DUMPNX32(from.hi);
            DUMPNX32(from.lo);
            DUMPNX32(increment.hi);
            DUMPNX32(increment.lo);
            DUMPNX32(result.hi);
            DUMPNX32(result.lo);
            DUMPNX32((uint32_t)(from64>>32));
            DUMPNX32((uint32_t)(from64));
            DUMPNX32((uint32_t)(increment64>>32));
            DUMPNX32((uint32_t)(increment64));
            DUMPNX32((uint32_t)(result64>>32));
            DUMPNX32((uint32_t)(result64));
            break;
        }
        printf("%s() PASSED...\n", __func__);
    }
#else
    printf("ENABLE_64_BIT_TEST is not enabled!! (%d)\n", ENABLE_64_BIT_TEST);
    isError= 2;
#endif
    printf("%s() done...\n", __func__);
    return isError;
}

int LibU48_TestSub(u32 testCount)
{
    int isError = 0;
#if ENABLE_64_BIT_TEST
    int ret, of;
    LibU64_t from, decrement, result;
    uint64_t from64, decrement64, result64;
    
    u32 ctr = 0;
    while (1)
    {
        isError = 0;
        if(ctr==testCount)
            break;
        ctr++;
        from.hi = LibUtil_GetRand16();
        from.lo = LibUtil_GetRand32();
        decrement.hi = LibUtil_GetRand16();
        decrement.lo = LibUtil_GetRand32();
        from64 = (((uint64_t)(from.hi)) << 32) | from.lo;
        decrement64 =  (((uint64_t)(decrement.hi)) << 32) | decrement.lo;
        ret = LibU48_Sub48To48(&from, &decrement, &result);
        result64 = from64 - decrement64;
        if (result64 & 0xFFFF000000000000) {
            of = 1;
            result64 &= 0x0000FFFFFFFFFFFF;
        } else {
            of = 0;
        }
        if (result.hi != (uint32_t)(result64>>32))
        {
            printf("result.hi error\n");
            isError = 1;
        }
        if (result.lo != (uint32_t)(result64))
        {
            printf("result.lo error\n");
            isError = 1;
        }
        if (of != ret)
        {
            printf("overflow error\n");
            isError = 1;
        }
        if (isError)
        {
            DUMPNX32(from.hi);
            DUMPNX32(from.lo);
            DUMPNX32(decrement.hi);
            DUMPNX32(decrement.lo);
            DUMPNX32(result.hi);
            DUMPNX32(result.lo);
            DUMPNX32((uint32_t)(from64>>32));
            DUMPNX32((uint32_t)(from64));
            DUMPNX32((uint32_t)(decrement64>>32));
            DUMPNX32((uint32_t)(decrement64));
            DUMPNX32((uint32_t)(result64>>32));
            DUMPNX32((uint32_t)(result64));
            break;
        }
        printf("%s() PASSED...\n", __func__);
    }
#else
    printf("ENABLE_64_BIT_TEST is not enabled!! (%d)\n", ENABLE_64_BIT_TEST);
    isError= 2;
#endif
    printf("%s() done...\n", __func__);
    return isError;
}

int LibU48_TestMul(u32 testCount)
{
    int isError = 0;
#if ENABLE_64_BIT_TEST
    u32 a, x;
    LibU64_t u64result;
    uint64_t a64, x64;
    int of, ret;
    
    u32 ctr = 0;
    while (1)
    {
        if(ctr==testCount)
            break;
        ctr++;
        a = LibUtil_GetRand32();
        x = LibUtil_GetRand32();
        ret = LibU48_Mul32To48(a, x, &u64result);
        a64 = a;
        x64 = x;
        a64 = a64 * x64;
        if (a64 & 0xFFFF000000000000) {
            of = 1;
            a64 &= 0x0000FFFFFFFFFFFF;
        } else {
            of = 0;
        }
        if (u64result.hi != (uint32_t)(a64>>32))
        {
            printf("hi number error ans=0x%08X, result=0x%08X, a=0x%08X, x=0x%08X\n", (uint32_t)(a64>>32), u64result.hi, a, x);
            isError = 1;
            break;
        }
        if (u64result.lo != (uint32_t)(a64))
        {
            printf("lo number error ans=0x%08X, result=0x%08X, a=0x%08X, x=0x%08X\n", (uint32_t)(a64), u64result.lo, a, x);
            isError = 1;
            break;
        }
        if (of != ret)
        {
            printf("overflow error\n");
            isError = 1;
        }
        printf("PASSED: hi=0x%08X, lo=0x%08X, a=0x%08X, x=0x%08X\n", u64result.hi, u64result.lo, a, x);
    }
#else
    printf("ENABLE_64_BIT_TEST is not enabled!! (%d)\n", ENABLE_64_BIT_TEST);
    isError= 2;
#endif
    printf("%s() done...\n", __func__);
    return isError;
}

int LibU48_TestDiv(u32 testCount)
{
    int isError = 0;
#if ENABLE_64_BIT_TEST
    u32 divisor, remainder;
    LibU64_t dividend, quotient;
    uint64_t dividend64, quotient64, remainder64;
    
    u32 ctr = 0;
    while (1)
    {
        if(ctr==testCount)
            break;
        ctr++;
        dividend.hi = LibUtil_GetRand16();
        dividend.lo = LibUtil_GetRand32();
        do {
            divisor = LibUtil_GetRand32();
        } while(divisor < 2);
        dividend64 = dividend.hi;
        dividend64 = dividend64 << 32 | dividend.lo;
        quotient64 = dividend64 / divisor;
        remainder64 = dividend64 % divisor;
        LibU48_Div32To48(&dividend, divisor, &quotient, &remainder);

        if (quotient.hi != (uint32_t)(quotient64>>32))
        {
            printf("quotient.hi error\n");
            printf("dividend64.hi=0x%08X  dividend64.lo=0x%08X  divisor=0x%08X\n", (uint32_t)(dividend64>>32), (uint32_t)dividend64, divisor);
            printf("quotient.hi=0x%08X  quotient.lo=0x%08X  remainder=0x%08X\n", quotient.hi, quotient.lo, remainder);
            printf("quotient64.hi=0x%08X  quotient64.lo=0x%08X  remainder64=0x%08X\n", (uint32_t)(quotient64>>32), (uint32_t)quotient64, (uint32_t)remainder64);
            isError = 1;
            break;
        }
        if (quotient.lo != (uint32_t)(quotient64))
        {
            printf("quotient.lo error\n");
            printf("dividend64.hi=0x%08X  dividend64.lo=0x%08X  divisor=0x%08X\n", (uint32_t)(dividend64>>32), (uint32_t)dividend64, divisor);
            printf("quotient.hi=0x%08X  quotient.lo=0x%08X  remainder=0x%08X\n", quotient.hi, quotient.lo, remainder);
            printf("quotient64.hi=0x%08X  quotient64.lo=0x%08X  remainder64=0x%08X\n", (uint32_t)(quotient64>>32), (uint32_t)quotient64, (uint32_t)remainder64);
            isError = 1;
            break;
        }
        if (remainder != (uint32_t)(remainder64))
        {
            printf("remainder error\n");
            printf("dividend64.hi=0x%08X  dividend64.lo=0x%08X  divisor=0x%08X\n", (uint32_t)(dividend64>>32), (uint32_t)dividend64, divisor);
            printf("quotient.hi=0x%08X  quotient.lo=0x%08X  remainder=0x%08X\n", quotient.hi, quotient.lo, remainder);
            printf("quotient64.hi=0x%08X  quotient64.lo=0x%08X  remainder64=0x%08X\n", (uint32_t)(quotient64>>32), (uint32_t)quotient64, (uint32_t)remainder64);
            isError = 1;
            break;
        }
        printf("%s() PASSED...\n", __func__);
    }
#else
    printf("ENABLE_64_BIT_TEST is not enabled!! (%d)\n", ENABLE_64_BIT_TEST);
    isError= 2;
#endif
    printf("%s() done...\n", __func__);
    return isError;
}

int LibU48_TestDiff_unit_test(u32 ah, u32 al, u32 bh, u32 bl, u32 turnaround_mask_63_32)
{
    int isError = 0;
#if ENABLE_64_BIT_TEST
    LIB_U64_COMPARE_t ret, ret64;
    LibU64_t a, b, result;
    uint64_t a64, b64, result64;

    a.hi = ah;
    a.lo = al;
    b.hi = bh;
    b.lo = bl;
    ret = LibU64_Diff64(&a, &b, &result, turnaround_mask_63_32);
    a64 = (((uint64_t)a.hi)<<32) | a.lo;
    b64 = (((uint64_t)b.hi)<<32) | b.lo;

    if (a64 > b64) {
        ret64 = A_IS_GREATER;
    } else if (a64 < b64) {
        ret64 = B_IS_GREATER;
    } else {
        ret64 = A_B_ARE_EQUAL;
    }

    if (ret64 == A_IS_GREATER) {
        result64 = a64 - b64;
    } else {
        result64 = b64 - a64;
    }
    // result of diff
    if (((uint32_t)(result64>>32)) >= turnaround_mask_63_32)
    {
        // do revert
        result64 = (~result64) + 1;
        if (ret64 == A_IS_GREATER)
            ret64 = B_IS_GREATER;
        else if (ret64 == B_IS_GREATER)
            ret64 = A_IS_GREATER;
    }
    //compare result
    if (result.hi != (uint32_t)(result64>>32))
    {
        printf("result.hi error\n");
        isError = 1;
    }
    if (result.lo != (uint32_t)(result64))
    {
        printf("result.lo error\n");
        isError = 1;
    }
    if (ret != ret64)
    {
        printf("ret error\n");
        isError = 1;
    }
    if (0 || isError)
    {
        DUMPNX32(a.hi);
        DUMPNX32(a.lo);
        DUMPNX32(b.hi);
        DUMPNX32(b.lo);
        DUMPNX32(result.hi);
        DUMPNX32(result.lo);
        DUMPNX32((uint32_t)(a64>>32));
        DUMPNX32((uint32_t)(a64));
        DUMPNX32((uint32_t)(b64>>32));
        DUMPNX32((uint32_t)(b64));
        DUMPNX32((uint32_t)(result64>>32));
        DUMPNX32((uint32_t)(result64));
        DUMPND(ret);
        DUMPND(ret64);
        DUMPND(isError);
        //break;
    }
#else
    printf("ENABLE_64_BIT_TEST is not enabled!! (%d)\n", ENABLE_64_BIT_TEST);
    isError= 2;
#endif
    printf("%s() done...\n", __func__);
    return isError;
}

int LibU48_TestDiff(u32 testCount)
{
    int isError = 0;
    u32 ctr = 0;
    //LibU64_TestDiff_unit_test(1,9,2,10);
    //LibU64_TestDiff_unit_test(0,0,0x00008FFF,0xFFFFFFFF, 0x00008000);
    while (1)
    {
        if(ctr==testCount)
            break;
        ctr++;
        isError = LibU64_TestDiff_unit_test(LibUtil_GetRand16(),LibUtil_GetRand32(),LibUtil_GetRand16(),LibUtil_GetRand32(), 0x00008000);
        if (isError)
            return isError;
    }
    printf("%s() done...\n", __func__);
    return isError;
}

int LibU48_TestAll(void)
{
#define TEST_COUNT 50000
    int ret = 0;
    LibUtil_InitRand();
    //RETURN_CHK(ret, LibU48_TestAdd(TEST_COUNT));
    //RETURN_CHK(ret, LibU48_TestSub(TEST_COUNT));
    //RETURN_CHK(ret, LibU48_TestMul(TEST_COUNT));
    //RETURN_CHK(ret, LibU48_TestDiv(TEST_COUNT));
    RETURN_CHK(ret, LibU48_TestDiff(TEST_COUNT));

    return ret;
#undef TEST_COUNT
}