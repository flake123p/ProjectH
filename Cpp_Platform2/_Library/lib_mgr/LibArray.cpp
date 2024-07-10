
#if 1
#include "Everything_Lib_Mgr.hpp"
#else
#include "Everything_Lib_Mgr_Lite.hpp"
#include "_LibString.hpp"
#include "_LibMemory.hpp"
#include "_Lib51Hex.hpp"
#include "LibTime.hpp"
#include "LibFileIO.hpp"
#include "LibFileIO2.hpp"
#endif

#define LIB_ARRAY_DEBUG_EN ( 0 )

int LibAry::Write(u32 position, const u8 *from, u32 fromLen)
{
    u32 newLen = position + fromLen;
    int needReallocate = 0;

    if (position > len) {
        return 1; //illegal position
    }

    if (newLen > allocatedLenInBytes) {
        needReallocate = 1;
    }

    if (needReallocate == 0) {
        MM_CPY(&ptr[position], from, fromLen);
        len = newLen;
        return 0;
    } else {
        u8 *newPtr;
        u32 newAllocatedLenInBytes = newLen/allocateIncrement;
        newAllocatedLenInBytes = (newAllocatedLenInBytes+1)*allocateIncrement;
        newPtr = (u8 *)MM_ALLOC(newAllocatedLenInBytes);
        if (newPtr == NULL) {
            return 1;
        }
        MM_CPY(newPtr, ptr, position);
        MM_CPY(&newPtr[position], from, fromLen);
        SAFE_FREE(ptr);
        ptr = newPtr;
        len = newLen;
        allocatedLenInBytes = newAllocatedLenInBytes;
    }

    return 0;
};

void LibArray_Demo(void)
{
    MM_INIT();

    {
        LibAry x;
        x.Dump();
        u8 abc[200] = {1,2,3};
        x.Write(0, abc, 33);
        x.Dump();
    }

#if LIB_ARRAY_DEBUG_EN
    {
        int ret;
        LibAry test;
        test.allocateIncrement = 4;
        BASIC_ASSERT(test.ptr == NULL);
        u8 abc[200] = {1,2,3};
        u8 def[200] = {4,5,6};
        ret = test.Write(1, abc, 2);
        BASIC_ASSERT(ret == 1);
        ret = test.Write(0, abc, 2);
        BASIC_ASSERT(test.len == 2 && test.ptr[0] == 1 && ret == 0 && test.allocatedLenInBytes == 4);

        ret = test.Write(1, def, 3);
        BASIC_ASSERT(test.len == 4 && test.ptr[1] == 4 && ret == 0 && test.allocatedLenInBytes == 4);
        test.Dump();

        ret = test.Write(0, abc, 5);
        BASIC_ASSERT(test.len == 5 && test.ptr[0] == 1 && ret == 0 && test.allocatedLenInBytes == 8);
        ret = test.Write(0, def, 9);
        BASIC_ASSERT(test.len == 9 && test.ptr[0] == 4 && ret == 0 && test.allocatedLenInBytes == 12);
        test.Dump();
    }
#endif

    MM_UNINIT();
}

