


#ifndef _MY_MACROS_H_INCLUDED_

/**
https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html

A variadic macro can be defined named or unnamed.

The named method which involves putting an argument name followed by "..."
which is later referenced by just using the argument name in the macro body.
("CPP" permits this, as an extension.)

e.g.
_DEBUG(fmt, args...) printf("%s:%s:%d: "fmt, __FILE__, __FUNCTION__, __LINE__, args) 
_DEBUG(fmt, args...) printf("%s:%s:%d: "fmt, __FILE__, __FUNCTION__, __LINE__ , ##args) 

p.s. __FUNCTION__ is a C99 and GCC feature only.


An unnamed variadic macro just involves using "..." and later referenced
in the macro body using the predefined macro __VA_ARGS__

e.g.
log(fmt, ...) printf(("[%d] %s(): " fmt), __LINE__, __FUNCTION__, ##__VA_ARGS__)
*/



//
// More descriptive for '!' operator
//
#define NOT(a) (!(a))

//
// Do not use "unless" with "else", it's hard to read.
//
#define unless(a) if(NOT(a))

#define until(x)  while(NOT(x)) 

//wait until x is true.
#define wait_until(x) until(x){/* do nothing */;}

#define UpTo(i, n) for((i) = 0; (i) < (n); ++(i))



//
//  Obtain the number of elements in the given C array
//
#define GET_ARRAY_LEN( arrayName ) (sizeof( arrayName ) / sizeof(( arrayName)[ 0 ] ))



//
// Unsafe macro, do not use with an argument containing a "side effect"!!
//
#ifndef MIN
#define MIN(num1,num2) ((num1)<(num2)?(num1):(num2))
#endif
#ifndef MAX
#define MAX(num1,num2) ((num1)>(num2)?(num1):(num2))
#endif
#ifndef ABS
#define ABS(Num)	((Num)>=0 ? (Num) : -(Num))
#endif
#define SIGN(num)  ((num)>0 ? 1 : ((num)<0?-1:0))

#define IS_ODD(num)   ((num) & 1)
#define IS_EVEN(num)  (!IS_ODD((num)))



//
// Include both numLow and numHigh!!
//
#define IS_BETWEEN(numToTest, numLow, numHigh) \
        ((unsigned char)((numToTest) >= (numLow) && (numToTest) <= (numHigh)))



//
// Increas/Decrease in range. Include both numLow and numHigh!!
//
#define INC_IN_RANGE(numToInc, numLow, numHigh) \
        ((numToInc) < (numHigh) ? (numToInc)+1 : (numToInc))

#define INC_IN_RANGE_CYCLE(numToInc, numLow, numHigh) \
        ((numToInc) < (numHigh) ? (numToInc)+1 : (numLow))

#define DEC_IN_RANGE(numToDec, numLow, numHigh) \
        ((numToDec) > (numLow) ? (numToDec)-1 : (numToDec))

#define DEC_IN_RANGE_CYCLE(numToDec, numLow, numHigh) \
        ((numToDec) > (numLow) ? (numToDec)-1 : (numHigh))



/*
 * Bit operation
 */
/*** no effect ***/
#define GET_BIT(var, pos)      (((var) >> (pos)) & 1)
#define GET_MSB(var, datatype) GET_BIT((var), (sizeof(datatype) * 8 - 1))
#define GET_LSB(var)           GET_BIT((var), 0)
#define CHECK_BIT(var, pos)      ((var) & (1 << (pos)))
#define CHECK_BIT_BOOL(var, pos) (!!((var) & (1 << (pos))))
/*** with effect ***/
#define SET_BIT(var, pos)     ((var) |= (1 << (pos)))
#define CLEAR_BIT(var, pos)   ((var) &= (~(1 << pos)))
#define TOGGLE_BIT(var, pos)  ((var) ^= (1 << (pos)))



//
// Flag operation
//
// no effect
#define FLG_CHK(a, b)  ((a) & (b))
// with effect
#define FLG_ADD(a, b)  ((a) |= (b))
#define FLG_RMV(a, b)  ((a) &= (~(b)))



//
// No Effect Version.  Beware of using this!!!
//
#define SetBit(var, pos)     ((var) | (1 << (pos)))
#define ClearBit(var, pos)   ((var) & (~(1 << pos)))
#define ToggleBit(var, pos)  ((var) ^ (1 << (pos)))
#define FlgAdd(a, b)         ((a) | (b))
#define FlgRmv(a, b)         ((a) & (~(b)))



#define BIT_0  (1<<0 )  //0x00000001
#define BIT_1  (1<<1 )  //0x00000002
#define BIT_2  (1<<2 )  //0x00000004
#define BIT_3  (1<<3 )  //0x00000008
#define BIT_4  (1<<4 )  //0x00000010
#define BIT_5  (1<<5 )  //0x00000020
#define BIT_6  (1<<6 )  //0x00000040
#define BIT_7  (1<<7 )  //0x00000080
#define BIT_8  (1<<8 )  //0x00000100
#define BIT_9  (1<<9 )  //0x00000200
#define BIT_10 (1<<10)  //0x00000400
#define BIT_11 (1<<11)  //0x00000800
#define BIT_12 (1<<12)  //0x00001000
#define BIT_13 (1<<13)  //0x00002000
#define BIT_14 (1<<14)  //0x00004000
#define BIT_15 (1<<15)  //0x00008000
#define BIT_16 (1<<16)  //0x00010000
#define BIT_17 (1<<17)  //0x00020000
#define BIT_18 (1<<18)  //0x00040000
#define BIT_19 (1<<19)  //0x00080000
#define BIT_20 (1<<20)  //0x00100000
#define BIT_21 (1<<21)  //0x00200000
#define BIT_22 (1<<22)  //0x00400000
#define BIT_23 (1<<23)  //0x00800000
#define BIT_24 (1<<24)  //0x01000000
#define BIT_25 (1<<25)  //0x02000000
#define BIT_26 (1<<26)  //0x04000000
#define BIT_27 (1<<27)  //0x08000000
#define BIT_28 (1<<28)  //0x10000000
#define BIT_29 (1<<29)  //0x20000000
#define BIT_30 (1<<30)  //0x40000000
#define BIT_31 (1<<31)  //0x80000000
#define SIZE_1  (1)
#define SIZE_2  (2)
#define SIZE_3  (3)
#define SIZE_4  (4)
#define SIZE_5  (5)
#define SIZE_6  (6)
#define SIZE_7  (7)
#define SIZE_8  (8)
#define SIZE_9  (9)
#define SIZE_10 (10)
#define SIZE_11 (11)
#define SIZE_12 (12)
#define SIZE_13 (13)
#define SIZE_14 (14)
#define SIZE_15 (15)
#define SIZE_16 (16)
#define SIZE_17 (17)
#define SIZE_18 (18)
#define SIZE_19 (19)
#define SIZE_20 (20)
#define SIZE_21 (21)
#define SIZE_22 (22)
#define SIZE_23 (23)
#define SIZE_24 (24)
#define SIZE_25 (25)
#define SIZE_26 (26)
#define SIZE_27 (27)
#define SIZE_28 (28)
#define SIZE_29 (29)
#define SIZE_30 (30)
#define SIZE_31 (31)
#define SIZE_32 (32)
#define OFFSET_0  (0)
#define OFFSET_1  (1)
#define OFFSET_2  (2)
#define OFFSET_3  (3)
#define OFFSET_4  (4)
#define OFFSET_5  (5)
#define OFFSET_6  (6)
#define OFFSET_7  (7)
#define OFFSET_8  (8)
#define OFFSET_9  (9)
#define OFFSET_10 (10)
#define OFFSET_11 (11)
#define OFFSET_12 (12)
#define OFFSET_13 (13)
#define OFFSET_14 (14)
#define OFFSET_15 (15)
#define OFFSET_16 (16)
#define OFFSET_17 (17)
#define OFFSET_18 (18)
#define OFFSET_19 (19)
#define OFFSET_20 (20)
#define OFFSET_21 (21)
#define OFFSET_22 (22)
#define OFFSET_23 (23)
#define OFFSET_24 (24)
#define OFFSET_25 (25)
#define OFFSET_26 (26)
#define OFFSET_27 (27)
#define OFFSET_28 (28)
#define OFFSET_29 (29)
#define OFFSET_30 (30)
#define OFFSET_31 (31)
#define BIT_MASK(size) ((1<<(size))-1)

#define CLEAR_BIT_FIELD(a, size, offset)        a=FlgRmv((a), BIT_MASK(size)<<(offset))
#define SET_BIT_FIELD(a, value, size, offset)   a=FlgAdd(FlgRmv((a), (BIT_MASK(size)<<(offset))), (value)<<(offset))
#define CHECK_BIT_FIELD(a, value, size, offset) (((a)&(BIT_MASK(size)<<(offset)))==((value)<<(offset)))

#define ClearBitField(a, size, offset)      FlgRmv((a), BIT_MASK(size)<<(offset))
#define SetBitField(a, value, size, offset) FlgAdd(FlgRmv((a), (BIT_MASK(size)<<(offset))), (value)<<(offset))



/**
Determines whether the memory architecture of current processor is LittleEndian.

Optimizing compiler should be able to reduce this macro to a boolean constant
TRUE or FALSE.

@return 1 if LittleEndian, else 0
*/
#define IS_LITTLE_ENDIAN()  (((*(short *)"21") & 0xFF) == '2')

/**
Determines whether the memory architecture of current processor is BigEndian.

Optimizing compiler should be able to reduce this macro to a boolean constant
TRUE or FALSE.

@return 1 if BigEndian, else 0
*/
#define IS_BIG_ENDIAN()     (((*(short *)"21") & 0xFF) == '1')

/**
SEE ALSO:
http://www.codeproject.com/Articles/567335/EssentialplusMacrosplusforplusCplusProgramming
*/



#define _MY_MACROS_H_INCLUDED_
#endif//_MY_MACROS_H_INCLUDED_

