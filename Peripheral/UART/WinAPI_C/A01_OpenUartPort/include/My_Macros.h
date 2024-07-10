


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



//
// Bit operation
//
// no effect
#define GET_BIT(var, pos)      ((var >> pos) & 1)
#define GET_MSB(var, datatype) GET_BIT(var, (sizeof(datatype) * 4 - 1))
#define GET_LSB(var)           GET_BIT(var, 0)
#define CHECK_BIT(var, pos)   (!!((var) & (1 << (pos))))
// with effect
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

