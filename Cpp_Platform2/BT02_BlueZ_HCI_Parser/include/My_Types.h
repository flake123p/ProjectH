


#ifndef _MY_TYPES_H_INCLUDED_

#include <stdint.h> //for uint32_t

//For argument
#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IO
#define IO
#endif

#ifndef uchar
typedef unsigned char uchar;
#endif
#ifndef ushort
typedef unsigned short ushort;
#endif
#ifndef uint
typedef unsigned int uint;
#endif
#ifndef ulong
typedef unsigned long ulong;
#endif

#ifndef u8
typedef uint8_t u8;
#endif
#ifndef u16
typedef uint16_t u16;
#endif
#ifndef u32
typedef uint32_t u32;
#endif
#ifndef u64
typedef uint64_t u64;
#endif

#ifndef s8
typedef int8_t s8;
#endif
#ifndef s16
typedef int16_t s16;
#endif
#ifndef s32
typedef int32_t s32;
#endif
#ifndef s64
typedef int64_t s64;
#endif

//Return Code define:
#define EC_EXIT_SUCCESS (0)
#define EC_EXIT_FAILURE (1)

//Generic Return Code:
//0x00010000: Memory issue
#define RC_MEMORY_ALLOCATE_FAILED (0x00010000)
#define RC_BUFFER_FULL            (0x00011000)
#define RC_BUFFER_OVERFLOW        (0x00011001)
#define RC_BUFFER_TOO_SMALL       (0x00011002)
//0x00020000: File issue
#define RC_FILE_REACH_EOF         (0x00020000)
#define RC_FILE_OPEN_ERROR        (0x00020001)
#define RC_FILE_CLOSE_ERROR       (0x00020002)
#define RC_FILE_SCAN_ERROR        (0x00020003)
#define RC_FILE_PRINT_ERROR       (0x00020004)

#define DO_ENDIAN_SWAP (true)
#define NO_ENDIAN_SWAP (false)

#define _MY_TYPES_H_INCLUDED_
#endif//_MY_TYPES_H_INCLUDED_

