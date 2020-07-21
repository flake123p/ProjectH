
#ifndef __LIB_ARRAY_HPP_INCLUDED_

#include "My_Types.h"
#include "_LibMem.hpp"

class LibAry {
public:
    u32 len;
    u8 *ptr;
    u32 allocatedLenInBytes;
    u32 allocateIncrement;

    LibAry(void){len=0;ptr=NULL;allocatedLenInBytes=0;allocateIncrement=16;};
    ~LibAry(void){SAFE_FREE(ptr)};
    int Push(u8 *from, u32 fromLen) {
        u32 left = allocatedLenInBytes - len;
        if (fromLen <= left) {
            MM_CPY(&ptr[len], from, fromLen);
            len += fromLen;
        } else {
            u32 *newPtr;
            u32 newAllocatedLenInBytes = (len+fromLen)/allocateIncrement;
            newAllocatedLenInBytes = (newAllocatedLenInBytes+1)*allocateIncrement;
            newPtr = 
        }
    };
    void Dump(void) {
        DUMPND(allocatedLenInBytes);
        DUMPND(allocateIncrement);
        ARRAYDUMPX2(ptr,len);
    };
};

class LibAryX : public LibAry {
public:
    LibAryX(void){PRINT_FUNC;};
    ~LibAryX(void){PRINT_FUNC;};
};

void LibArray_Demo(void);


#define __LIB_ARRAY_HPP_INCLUDED_
#endif//__LIB_ARRAY_HPP_INCLUDED_

