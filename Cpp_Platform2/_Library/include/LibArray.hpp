
#ifndef __LIB_ARRAY_HPP_INCLUDED_

#include "My_Types.h"
#include "_LibMem.hpp"

class LibAry {
public:
    u32 len;
    u8 *ptr;
    u32 allocatedLenInBytes;
    u32 allocateIncrement;

    LibAry(void){Init();};
    ~LibAry(void){Uninit();};
    void Init(void){len=0;ptr=NULL;allocatedLenInBytes=0;allocateIncrement=32;};
    void Uninit(void) {SAFE_FREE(ptr);len=0;ptr=NULL;};

    int Write(u32 position, const u8 *from, u32 fromLen);
    int Push(const u8 *from, u32 fromLen) { return Write(len, from, fromLen);};

    void Dump(void) {
        DUMPND(allocatedLenInBytes);
        DUMPND(allocateIncrement);
        DUMPND(len);
        if (len) {
            ARRAYDUMPX2(ptr,len);
        }
    };
};

/*
class LibAryX : public LibAry {
public:
    LibAryX(void){PRINT_FUNC;};
    ~LibAryX(void){PRINT_FUNC;};
};
*/

void LibArray_Demo(void);


#define __LIB_ARRAY_HPP_INCLUDED_
#endif//__LIB_ARRAY_HPP_INCLUDED_

