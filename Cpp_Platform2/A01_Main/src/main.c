/*
    TODO:
        1.task message send/receive                [LibHiTask.cpp]
            create task & trigger event
            task go with message
            wait task done with comeback message
        2.thread-safe LibDesc                      [LibHiTask.cpp]
        3.task pool (enable true multi-threading)  [LibHiTask.cpp]
*/

#include "Everything_App.hpp"
//#include "unix_sys_queue.h"

static int gLibUtil_GetUniqueU32_Inited = 0;
static u32 gLibUtil_GetUniqueU32_Base = 0;
static u32 gLibUtil_GetUniqueU32_Increment = 0;
u32 LibUtil_GetUniqueU32(void)
{
    if (0 == gLibUtil_GetUniqueU32_Inited)
    {
        gLibUtil_GetUniqueU32_Inited = 1;

        LibUtil_InitRand();
        gLibUtil_GetUniqueU32_Base = (u32)LibUtil_GetRand();
        gLibUtil_GetUniqueU32_Increment = (u32)LibUtil_GetRand();
        gLibUtil_GetUniqueU32_Increment |= 0x00000001;
    }

    gLibUtil_GetUniqueU32_Base += gLibUtil_GetUniqueU32_Increment;
    if (gLibUtil_GetUniqueU32_Base == 0)
        gLibUtil_GetUniqueU32_Base += gLibUtil_GetUniqueU32_Increment;

    return gLibUtil_GetUniqueU32_Base;
}

static int gLibUtil_GetUniqueU16_Inited = 0;
static u16 gLibUtil_GetUniqueU16_Base = 0;
static u16 gLibUtil_GetUniqueU16_Increment = 0;
u16 LibUtil_GetUniqueU16(void)
{
    if (0 == gLibUtil_GetUniqueU16_Inited)
    {
        gLibUtil_GetUniqueU16_Inited = 1;

        LibUtil_InitRand();
        gLibUtil_GetUniqueU16_Base = (u16)LibUtil_GetRand();
        gLibUtil_GetUniqueU16_Increment = (u16)LibUtil_GetRand();
        gLibUtil_GetUniqueU16_Increment |= 0x0001;
    }

    gLibUtil_GetUniqueU16_Base += gLibUtil_GetUniqueU16_Increment;
    if (gLibUtil_GetUniqueU16_Base == 0)
        gLibUtil_GetUniqueU16_Base += gLibUtil_GetUniqueU16_Increment;

    return gLibUtil_GetUniqueU16_Base;
}

#define LIB_MEM_ENABLE        ( 1 )
#define LIB_MEM_ASSERT_ENABLE ( 1 )

#define LIB_MEM_HEAD (&gLibMemHead)
#define LIB_MEM_CURR (gLibMemCurr)
#define LIB_MEM_DATA(cell) ((u8 *)(&(cell->data)))
#define LIB_MEM_FLAG(cell) (((u8 *)&(cell->data))+cell->size_with_padding)

#define LIB_MEM_READ_CHECK(addr,len,key) LibMem_ReadCheckEx((u8 *)(addr),len,key,__FILE__,__LINE__)
#define LIB_MEM_WRITE_CHECK(addr,len,key,doWrite) LibMem_WriteCheckEx((u8 *)(addr),len,key,doWrite,__FILE__,__LINE__)

#if LIB_MEM_ENABLE
#define LIB_MEM_ALLOC(size)               LibMem_MallocEx(size,__FILE__,__LINE__)
#define LIB_MEM_FREE(ptr)                 LibMem_Free(ptr)
#define LIB_MEM_KEY_INIT(addr,key)        LibMem_KeyInit(addr,key)
#define LIB_MEM_CONFIG(addr,len,key,flag) LibMem_ConfigureProtection(addr,len,key,flag)
#define LIB_MEM_DUMP()                    LibMem_Dump()
#define LIB_MEM_DUMP_CELL(addr)           LibMem_DumpCell(addr)
#define WT(a,b)           LIB_MEM_WRITE_CHECK(&(a),sizeof(a),0,1);(a)=(b)
#define WTKEY(key,a,b)    LIB_MEM_WRITE_CHECK(&(a),sizeof(a),key,1);(a)=(b)
#define RW(a,op,b)        LIB_MEM_READ_CHECK(&(a),sizeof(a),0);LIB_MEM_WRITE_CHECK(&(a),sizeof(a),0,1);(a)op(b)
#define RWKEY(key,a,op,b) LIB_MEM_READ_CHECK(&(a),sizeof(a),key);LIB_MEM_WRITE_CHECK(&(a),sizeof(a),key,1);(a)op(b)
#define RD(a,b)           LIB_MEM_READ_CHECK(&(b),sizeof(b),0);(a)=(b)
#define RDKEY(key,a,b)    LIB_MEM_READ_CHECK(&(b),sizeof(b),key);(a)=(b)
#else
#define LIB_MEM_ALLOC(size) malloc(size)
#define LIB_MEM_FREE(ptr)   free(ptr)
#define LIB_MEM_KEY_INIT(addr,key)
#define LIB_MEM_CONFIG(addr,len,key,flag)
#define LIB_MEM_DUMP()
#define LIB_MEM_DUMP_CELL(addr)
#define WT(a,b)           (a)=(b)
#define WTKEY(key,a,b)    (a)=(b)
#define RW(a,op,b)        (a)op(b)
#define RWKEY(key,a,op,b) (a)op(b)
#define RD(a,b)           (a)=(b)
#define RDKEY(key,a,b)    (a)=(b)
#endif

typedef enum {
    LIB_MEM_BYTE_WAS_WRITTEN  = 0x01, /* used in flag array */

    LIB_MEM_READ_PROTECT_OFF  = 0x10,
    LIB_MEM_WRITE_PROTECT_OFF = 0x20,
    LIB_MEM_READ_PROTECT_ON   = 0x40, /* used in flag array */
    LIB_MEM_WRITE_PROTECT_ON  = 0x80, /* used in flag array */
} LIB_MEM_FLAG_t;

typedef enum {
    LIB_MEM_RC_SUCCESS = 0,
    LIB_MEM_RC_CANT_FIND_CELL,
    LIB_MEM_RC_DUPLICAT_KEY_INIT,
    LIB_MEM_RC_KEY_IS_NOT_INITED,
    LIB_MEM_RC_KEY_IS_NOT_MATCH,
    LIB_MEM_RC_READ_BEFORE_WRITE,
    LIB_MEM_RC_READ_PROTECT_VIOLATION,
    LIB_MEM_RC_WRITE_PROTECT_VIOLATION,
} LIB_MEM_RETURN_CODE_t;

typedef struct {
    DLList_Entry_t entry;
    u32 key;
    size_t size;
    size_t real_size;
    size_t size_with_padding;
    u8 *data;
} LibMem_Cell_t;

DLList_Head_t gLibMemHead = DLLIST_HEAD_INIT(LIB_MEM_HEAD);
LibMem_Cell_t *gLibMemCurr = NULL;
int gLibMemInitiated = 0;
MUTEX_HANDLE_t gLibMemMutexHdl = NULL;

static const char *_LibMem_ErrorCodeString(int errorCode)
{
    switch (errorCode) {
        case LIB_MEM_RC_CANT_FIND_CELL: return "LIB_MEM_RC_CANT_FIND_CELL";
        case LIB_MEM_RC_DUPLICAT_KEY_INIT: return "LIB_MEM_RC_DUPLICAT_KEY_INIT";
        case LIB_MEM_RC_KEY_IS_NOT_INITED: return "LIB_MEM_RC_KEY_IS_NOT_INITED";
        case LIB_MEM_RC_KEY_IS_NOT_MATCH: return "LIB_MEM_RC_KEY_IS_NOT_MATCH";
        case LIB_MEM_RC_READ_BEFORE_WRITE: return "LIB_MEM_RC_READ_BEFORE_WRITE";
        case LIB_MEM_RC_READ_PROTECT_VIOLATION: return "LIB_MEM_RC_READ_PROTECT_VIOLATION";
        case LIB_MEM_RC_WRITE_PROTECT_VIOLATION: return "LIB_MEM_RC_WRITE_PROTECT_VIOLATION";
    }
    return "NULL";
}

static void _LibMem_Mutex_Lock(void)
{
    if (gLibMemInitiated == 0)
    {
        int retVal;
        gLibMemInitiated = 1;
        ASSERT_CHK( retVal, LibIPC_Mutex_Create(&gLibMemMutexHdl) );
    }

	LibIPC_Mutex_Lock(gLibMemMutexHdl);
}

static void _LibMem_Mutex_Unlock(void)
{
    BASIC_ASSERT(gLibMemInitiated == 1);

    LibIPC_Mutex_Unlock(gLibMemMutexHdl);
}

static void _LibMem_Mutex_Destroy(void)
{
    int retVal;

    if (gLibMemMutexHdl == NULL)
        return;

    ASSERT_CHK( retVal, LibIPC_Mutex_Destroy(gLibMemMutexHdl) );
    gLibMemMutexHdl = NULL;
}

static LibMem_Cell_t *_LibMem_FindCellEntry(u8 *data_addr)
{
    if (gLibMemCurr == NULL)
        return NULL;

    LibMem_Cell_t *curr_cell = gLibMemCurr;

    while (1)
    {
        if (LIB_MEM_DATA(curr_cell) == data_addr) {
            gLibMemCurr = curr_cell;
            return curr_cell;
        }
        if (curr_cell->entry.next == NULL) {
            break;
        } else {
            curr_cell = (LibMem_Cell_t *)curr_cell->entry.next;
        }
    }

    curr_cell = gLibMemCurr;
    while (1)
    {
        if (curr_cell->entry.prev == LIB_MEM_HEAD)
            break;

        curr_cell = (LibMem_Cell_t *)curr_cell->entry.prev;

        if (LIB_MEM_DATA(curr_cell) == data_addr) {
            gLibMemCurr = curr_cell;
            return curr_cell;
        }
    }

    return NULL;
}

static LibMem_Cell_t *_LibMem_FindCellEntryByAnyAddr(u8 *any_addr, u32 len)
{
    if (gLibMemCurr == NULL)
        return NULL;

    LibMem_Cell_t *curr_cell = gLibMemCurr;
    u8 *cell_addr_start = LIB_MEM_DATA(curr_cell);
    u8 *cell_addr_end = cell_addr_start + curr_cell->size;
    u8 *target_addr_start = any_addr;
    u8 *target_addr_end = any_addr + len;

    while (1)
    {
        if (cell_addr_start <= target_addr_start && cell_addr_end >= target_addr_end) {
            gLibMemCurr = curr_cell;
            return curr_cell;
        }
        if (curr_cell->entry.next == NULL) {
            break;
        } else {
            curr_cell = (LibMem_Cell_t *)curr_cell->entry.next;
            cell_addr_start = LIB_MEM_DATA(curr_cell);
            cell_addr_end = cell_addr_start + curr_cell->size;
        }
    }

    curr_cell = gLibMemCurr;
    while (1)
    {
        if (curr_cell->entry.prev == LIB_MEM_HEAD)
            break;

        curr_cell = (LibMem_Cell_t *)curr_cell->entry.prev;
        cell_addr_start = LIB_MEM_DATA(curr_cell);
        cell_addr_end = cell_addr_start + curr_cell->size;

        if (cell_addr_start <= target_addr_start && cell_addr_end >= target_addr_end) {
            gLibMemCurr = curr_cell;
            return curr_cell;
        }
    }

    return NULL;
}

void LibMem_Init(void)
{
    if (gLibMemInitiated == 0)
    {
        int retVal;
        gLibMemInitiated = 1;
        ASSERT_CHK( retVal, LibIPC_Mutex_Create(&gLibMemMutexHdl) );
    }
}

void LibMem_Uninit(void)
{
    LibMem_Cell_t *curr_cell;
    LibMem_Cell_t *cell_to_free;

    DLLIST_WHILE_START(LIB_MEM_HEAD, curr_cell, LibMem_Cell_t)
    {
        cell_to_free = curr_cell;
        DLLIST_WHILE_NEXT(curr_cell, LibMem_Cell_t);
        free(cell_to_free);
    }
    gLibMemHead = DLLIST_HEAD_INIT(LIB_MEM_HEAD);
    gLibMemCurr = NULL;

    _LibMem_Mutex_Destroy();
	gLibMemInitiated = 0;
	gLibMemMutexHdl = NULL;
}

void *LibMem_Malloc(size_t size)
{
    size_t memory_bus_size = sizeof(u8 *);
    LibMem_Cell_t *curr_cell;
    size_t size_with_padding;
    size_t real_size;
    u8 *flag;

    size_with_padding = (size / memory_bus_size) * memory_bus_size;
    if (size % memory_bus_size)
        size_with_padding += memory_bus_size;

    real_size = sizeof(LibMem_Cell_t) + size_with_padding - memory_bus_size/*u8 *data*/ + size_with_padding;

    _LibMem_Mutex_Lock();
    curr_cell = (LibMem_Cell_t *)malloc(real_size);
    //printf("%s(), size=%u, size_with_padding=%u, real_size=%u\n", __func__, (unsigned int)size, (unsigned int)size_with_padding, (unsigned int)real_size);
    if (curr_cell == NULL) {
        return NULL;
    }
    DLLIST_INSERT_LAST(LIB_MEM_HEAD, curr_cell);
    _LibMem_Mutex_Unlock();

    curr_cell->key = 0;
    curr_cell->size = size;
    curr_cell->real_size = real_size;
    curr_cell->size_with_padding = size_with_padding;

    flag = LIB_MEM_FLAG(curr_cell);
    memset(flag, 0, size);

    gLibMemCurr = curr_cell;

    return (void *)&(curr_cell->data);
}

void *LibMem_MallocEx(size_t size, const char *file_str, int line)
{
    void *ret = LibMem_Malloc(size);
    if (NULL == ret) {
        printf("Can't allocate memory! Assert in file: %s, line: %d\n", file_str, line);
        BASIC_ASSERT(0);
    }
    return ret;
}

int LibMem_Free(void *ptr)
{
    LibMem_Cell_t *curr_cell = _LibMem_FindCellEntry((u8 *)ptr);

    if (curr_cell == NULL)
        return LIB_MEM_RC_CANT_FIND_CELL;

    _LibMem_Mutex_Lock();
    DLLIST_REMOVE_NODE(LIB_MEM_HEAD, curr_cell);
    _LibMem_Mutex_Unlock();

    free(curr_cell);

    return 0;
}

int LibMem_RangeCheck(u8 *addr, u32 size, int do_read_check)
{
    // 1 range check
    // 2 initiated check (flag = 0 = uninitiated)
    return 0;
}

int LibMem_KeyInit(u8 *cell_data_addr, u32 key)
{
    LibMem_Cell_t *curr_cell = _LibMem_FindCellEntry(cell_data_addr);

    if (curr_cell == NULL)
        return LIB_MEM_RC_CANT_FIND_CELL;

    if (curr_cell->key != 0)
        return LIB_MEM_RC_DUPLICAT_KEY_INIT;

    curr_cell->key = key;
    return 0;
}

int LibMem_ConfigureProtection(u8 *any_addr, u32 len, u32 key, u8 act_flags/*LIB_MEM_FLAG_t*/)
{
    LibMem_Cell_t *curr_cell = _LibMem_FindCellEntryByAnyAddr(any_addr, len);

    if (curr_cell == NULL)
        return LIB_MEM_RC_CANT_FIND_CELL;

    if (curr_cell->key == 0)
        return LIB_MEM_RC_KEY_IS_NOT_INITED;

    if (curr_cell->key != key)
        return LIB_MEM_RC_KEY_IS_NOT_MATCH;

    {
        u8 *flag_start = any_addr + curr_cell->size_with_padding;

        if (act_flags & LIB_MEM_READ_PROTECT_OFF) {
            FOREACH_I(len) {
                FLG_RMV(*(flag_start+i), LIB_MEM_READ_PROTECT_ON);
            }
        }
        if (act_flags & LIB_MEM_WRITE_PROTECT_OFF) {
            FOREACH_I(len) {
                FLG_RMV(*(flag_start+i), LIB_MEM_WRITE_PROTECT_ON);
            }
        }
        if (act_flags & LIB_MEM_READ_PROTECT_ON) {
            FOREACH_I(len) {
                FLG_ADD(*(flag_start+i), LIB_MEM_READ_PROTECT_ON);
            }
        }
        if (act_flags & LIB_MEM_WRITE_PROTECT_ON) {
            FOREACH_I(len) {
                FLG_ADD(*(flag_start+i), LIB_MEM_WRITE_PROTECT_ON);
            }
        }
    }
    return 0;
}

int LibMem_ReadCheck(u8 *any_addr, u32 len, u32 key)
{
    int key_is_matched = 0;
    LibMem_Cell_t *curr_cell = _LibMem_FindCellEntryByAnyAddr(any_addr, len);

    if (curr_cell == NULL)
        return LIB_MEM_RC_CANT_FIND_CELL;

    if (curr_cell->key != 0) {
        if (curr_cell->key == key)
            key_is_matched = 1;
        else if (key != 0)
            return LIB_MEM_RC_KEY_IS_NOT_MATCH;
    }

    {
        u8 *flag_start = any_addr + curr_cell->size_with_padding;

        if (key_is_matched) {
            FOREACH_I(len) {
                if (0 == (flag_start[i] & LIB_MEM_BYTE_WAS_WRITTEN))
                    return LIB_MEM_RC_READ_BEFORE_WRITE;
            }
        } else {
            FOREACH_I(len) {
                if (0 == (flag_start[i] & LIB_MEM_BYTE_WAS_WRITTEN))
                    return LIB_MEM_RC_READ_BEFORE_WRITE;

                if (flag_start[i] & LIB_MEM_READ_PROTECT_ON)
                    return LIB_MEM_RC_READ_PROTECT_VIOLATION;
            }
        }

    }
    return 0;
}

int LibMem_ReadCheckEx(u8 *any_addr, u32 len, u32 key, const char *file_str, int line)
{
    int ret = LibMem_ReadCheck(any_addr, len, key);

    if (ret) {
        printf("%s() error, code: %s, in file:%s, line:%d\n", __func__, _LibMem_ErrorCodeString(ret), file_str, line);
        #if LIB_MEM_ASSERT_ENABLE
        BASIC_ASSERT(0);
        #endif
    }
    return ret;
}

int LibMem_WriteCheck(u8 *any_addr, u32 len, u32 key, int do_write)
{
    int key_is_matched = 0;
    LibMem_Cell_t *curr_cell = _LibMem_FindCellEntryByAnyAddr(any_addr, len);

    if (curr_cell == NULL)
        return LIB_MEM_RC_CANT_FIND_CELL;

    if (curr_cell->key != 0) {
        if (curr_cell->key == key)
            key_is_matched = 1;
        else if (key != 0)
            return LIB_MEM_RC_KEY_IS_NOT_MATCH;
    }

    {
        u8 *flag_start = any_addr + curr_cell->size_with_padding;

        if (key_is_matched) {
            if (do_write) {
                FOREACH_I(len) {
                    flag_start[i] |= LIB_MEM_BYTE_WAS_WRITTEN;
                }
            }
        } else {
            FOREACH_I(len) {
                if (flag_start[i] & LIB_MEM_WRITE_PROTECT_ON)
                    return LIB_MEM_RC_WRITE_PROTECT_VIOLATION;

                if (do_write)
                    flag_start[i] |= LIB_MEM_BYTE_WAS_WRITTEN;
            }
        }

    }
    return 0;
}

int LibMem_WriteCheckEx(u8 *any_addr, u32 len, u32 key, int do_write, const char *file_str, int line)
{
    int ret = LibMem_WriteCheck(any_addr, len, key, do_write);

    if (ret) {
        printf("%s() error, code: %s, in file:%s, line:%d\n", __func__, _LibMem_ErrorCodeString(ret), file_str, line);
        #if LIB_MEM_ASSERT_ENABLE
        BASIC_ASSERT(0);
        #endif
    }
    return ret;
}

void LibMem_DumpCell(u8 *any_addr)
{
    u8 *ptr;
    LibMem_Cell_t *curr_cell = _LibMem_FindCellEntryByAnyAddr(any_addr, 0);

    if (curr_cell == NULL) {
        printf("Can't find matched cell!\n");
        return;
    }

    DUMPNA(curr_cell);
    DUMPNX(curr_cell->key);
    DUMPNU(curr_cell->size);
    DUMPNU(curr_cell->real_size);
    DUMPNU(curr_cell->size_with_padding);

    printf("Data in hex:\n");
    ptr = LIB_MEM_DATA(curr_cell);
    FOREACH_I(curr_cell->size) {
        if (i%4 == 3)
            printf("%02X, ", ptr[i]);
        else
            printf("%02X ", ptr[i]);
        if (i%16 == 15)
            PRINT_NEXT_LINE;
    }
    PRINT_NEXT_LINE;

    printf("Flag in hex:\n");
    ptr = LIB_MEM_DATA(curr_cell) + curr_cell->size_with_padding;
    FOREACH_I(curr_cell->size) {
        if (i%4 == 3)
            printf("%02X, ", ptr[i]);
        else
            printf("%02X ", ptr[i]);
        if (i%16 == 15)
            PRINT_NEXT_LINE;
    }
    PRINT_NEXT_LINE;
}

void LibMem_Dump(void)
{
    LibMem_Cell_t *curr_cell;
    u32 key;
    size_t size;
    size_t real_size;
    void *addr1;
    void *addr2;
    void *flag;
    void *prev;
    void *next;

    DUMPNA(LIB_MEM_HEAD);
    DUMPNA(LIB_MEM_CURR);

    DLLIST_FOREACH(LIB_MEM_HEAD, curr_cell, LibMem_Cell_t)
    {
        key = curr_cell->key;
        size = curr_cell->size;
        real_size = curr_cell->real_size;
        addr1 = (void *)curr_cell;
        addr2 = (void *)LIB_MEM_DATA(curr_cell);
        flag = (void *)LIB_MEM_FLAG(curr_cell);
        prev = (void *)curr_cell->entry.prev;
        next = (void *)curr_cell->entry.next;
        DUMPX(key);
        DUMPD(size);
        DUMPD(real_size);
        DUMPA(addr1);
        DUMPA(addr2);
        DUMPA(flag);
        DUMPA(prev);
        DUMPNA(next);
    }
}

/*
key
read protect
write protect
read uninitialed data
bitwise write
mutex
*/

void LibMem_Demo(void)
{
typedef struct {
    u8 a;
    u16 b;
    u32 c;
}testaaa;
    DUMPND(sizeof(testaaa));

    u8 *ptr;
    u8 *ptr2;
    testaaa *ptraaa;
    u32 ptr2_key = LibUtil_GetUniqueU32();
    //LibMem_Cell_t *curr_cell;
    ptr = (u8 *)LIB_MEM_ALLOC(3);
    ptr2 = (u8 *)LIB_MEM_ALLOC(9);
    ptraaa = (testaaa *)ptr2;

    LIB_MEM_KEY_INIT(ptr, 2);

    LIB_MEM_DUMP();

    LIB_MEM_KEY_INIT(ptr2, ptr2_key);
    LIB_MEM_CONFIG(&ptraaa->a, sizeof(ptraaa->a), ptr2_key, LIB_MEM_READ_PROTECT_ON);
    LIB_MEM_CONFIG((u8 *)&ptraaa->b, sizeof(ptraaa->b), ptr2_key, LIB_MEM_WRITE_PROTECT_ON);

    WTKEY(0, ptraaa->a, 0x78);
    LIB_MEM_DUMP_CELL(ptr2);
    u8 x;
    RDKEY(ptr2_key, x, ptraaa->a);
    DUMPNX(x);
    RWKEY(ptr2_key, ptraaa->a, =, 0x0f);
    DUMPNX(ptraaa->a);

    LibMem_Uninit();
}

int main(int argc, char *argv[])
{
    //LibUtile_Demo2();

    LibMem_Demo();
    //DUMPND(LibUtil_GetUniqueU32());
    //DUMPND(LibUtil_GetUniqueU32());
    //DUMPND(LibUtil_GetUniqueU32());
    //DUMPND(LibUtil_GetUniqueU32());
    return 99;
}


