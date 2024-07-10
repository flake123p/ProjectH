
#include "Everything_Lib_Mgr.hpp"

#define _LIB_MEM_HEAD (&gLibMemHead)
#define _LIB_MEM_CURR (gLibMemCurr)
#define _LIB_MEM_DATA(cell) ((u8 *)(&(cell->data)))
#define _LIB_MEM_FLAG(cell) (((u8 *)&(cell->data))+cell->size_with_padding)

DLList_Head_t gLibMemHead = DLLIST_HEAD_INIT(_LIB_MEM_HEAD);
LibMem_Cell_t *gLibMemCurr = NULL;
LibMT_UtilMutex_t gLibMemLock = {NULL};

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

static LibMem_Cell_t *_LibMem_FindCellEntry_NotSafe(u8 *data_addr)
{
    u32 loopCtr = 0;

    if (gLibMemCurr == NULL)
        return NULL;

    {
        LibMem_Cell_t *curr_cell = gLibMemCurr;
        while (1)
        {
            if (loopCtr < 3) {
                DUMPD(loopCtr);DUMPNA(curr_cell);
            }

            if (_LIB_MEM_DATA(curr_cell) == data_addr) {
                gLibMemCurr = curr_cell;
                return curr_cell;
            }
            if (curr_cell->entry.next == NULL) {
                break;
            } else {
                curr_cell = (LibMem_Cell_t *)curr_cell->entry.next;
            }
            loopCtr++;
            if (loopCtr < 3) {
                DUMPD(loopCtr);DUMPNA(curr_cell);
            }
            if (loopCtr > 1000000) {
                BASIC_ASSERT(0);
            }
        }

        curr_cell = gLibMemCurr;
        while (1)
        {
            if (curr_cell->entry.prev == _LIB_MEM_HEAD)
                break;

            curr_cell = (LibMem_Cell_t *)curr_cell->entry.prev;

            if (_LIB_MEM_DATA(curr_cell) == data_addr) {
                gLibMemCurr = curr_cell;
                return curr_cell;
            }
            loopCtr++;
            if (loopCtr > 1000000) {
                BASIC_ASSERT(0);
            }
        }
    }
    return NULL;
}

static LibMem_Cell_t *_LibMem_FindCellEntry_ForFree_NotSafe(u8 *data_addr)
{
    u32 loopCtr = 0;

    if (gLibMemHead.head == NULL)
        return NULL;

    {
        LibMem_Cell_t *curr_cell = (LibMem_Cell_t *)gLibMemHead.head;
        while (1)
        {
            if (loopCtr < 3) {
                //DUMPD(loopCtr);DUMPNA(curr_cell);
            }

            if (_LIB_MEM_DATA(curr_cell) == data_addr) {
                return curr_cell;
            }
            if (curr_cell->entry.next == NULL) {
                break;
            } else {
                curr_cell = (LibMem_Cell_t *)curr_cell->entry.next;
            }
            loopCtr++;
            if (loopCtr < 3) {
                //DUMPD(loopCtr);DUMPNA(curr_cell);
            }
            if (loopCtr > 1000000) {
                BASIC_ASSERT(0);
            }
        }
    }
    return NULL;
}


static LibMem_Cell_t *_LibMem_FindCellEntryByAnyAddr(u8 *any_addr, u32 len)
{
    if (gLibMemCurr == NULL)
        return NULL;

    LibMT_UtilMutex_Lock(&gLibMemLock);
    {
        LibMem_Cell_t *curr_cell = gLibMemCurr;
        u8 *cell_addr_start = _LIB_MEM_DATA(curr_cell);
        u8 *cell_addr_end = cell_addr_start + curr_cell->size;
        u8 *target_addr_start = any_addr;
        u8 *target_addr_end = any_addr + len;


        while (1)
        {
            if (cell_addr_start <= target_addr_start && cell_addr_end >= target_addr_end) {
                gLibMemCurr = curr_cell;
                LibMT_UtilMutex_Unlock(&gLibMemLock);
                return curr_cell;
            }
            if (curr_cell->entry.next == NULL) {
                break;
            } else {
                curr_cell = (LibMem_Cell_t *)curr_cell->entry.next;
                cell_addr_start = _LIB_MEM_DATA(curr_cell);
                cell_addr_end = cell_addr_start + curr_cell->size;
            }
        }

        curr_cell = gLibMemCurr;
        while (1)
        {
            if (curr_cell->entry.prev == _LIB_MEM_HEAD)
                break;

            curr_cell = (LibMem_Cell_t *)curr_cell->entry.prev;
            cell_addr_start = _LIB_MEM_DATA(curr_cell);
            cell_addr_end = cell_addr_start + curr_cell->size;

            if (cell_addr_start <= target_addr_start && cell_addr_end >= target_addr_end) {
                gLibMemCurr = curr_cell;
                LibMT_UtilMutex_Unlock(&gLibMemLock);
                return curr_cell;
            }
        }
    }
    LibMT_UtilMutex_Unlock(&gLibMemLock);
    return NULL;
}

static int gLibMemIsInited = 0;
void LibMem_Init(void)
{
    if (gLibMemIsInited) {
        return;
    }
    gLibMemIsInited = 1;
    LibMT_UtilMutex_Init(&gLibMemLock);
}

void LibMem_Uninit(void)
{
    LibMem_Cell_t *curr_cell;
    LibMem_Cell_t *cell_to_free;

    BASIC_ASSERT(gLibMemIsInited == 1);

    DLLIST_WHILE_START(_LIB_MEM_HEAD, curr_cell, LibMem_Cell_t)
    {
        printf("Unreleased meory:0x%p, file:%s, line:%d\n", curr_cell, curr_cell->callerFile, curr_cell->callerLine);
        cell_to_free = curr_cell;
        DLLIST_WHILE_NEXT(curr_cell, LibMem_Cell_t);
        free(cell_to_free);
    }
    gLibMemHead = DLLIST_HEAD_INIT(_LIB_MEM_HEAD);
    gLibMemCurr = NULL;

    LibMT_UtilMutex_Uninit(&gLibMemLock);
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

    LibMT_UtilMutex_Lock(&gLibMemLock);
    curr_cell = (LibMem_Cell_t *)malloc(real_size);
    //printf("%s(), size=%u, size_with_padding=%u, real_size=%u\n", __func__, (unsigned int)size, (unsigned int)size_with_padding, (unsigned int)real_size);
    if (curr_cell == NULL) {
        LibMT_UtilMutex_Unlock(&gLibMemLock);
        return NULL;
    }
    DLLIST_INSERT_LAST(_LIB_MEM_HEAD, curr_cell);
    gLibMemCurr = curr_cell;
    LibMT_UtilMutex_Unlock(&gLibMemLock);

    curr_cell->key = 0;
    curr_cell->size = size;
    curr_cell->real_size = real_size;
    curr_cell->size_with_padding = size_with_padding;
    curr_cell->callerFile = "DUMMY_FILE";
    curr_cell->callerLine = 999999;

    flag = _LIB_MEM_FLAG(curr_cell);
    memset(flag, 0, size);

    return (void *)&(curr_cell->data);
}

void *LibMem_MallocEx(size_t size, const char *file_str, int line)
{
    void *ret = LibMem_Malloc(size);
    if (NULL == ret) {
        printf("Can't allocate memory! Assert in file: %s, line: %d\n", file_str, line);
        BASIC_ASSERT(0);
    }
    {
        LibMem_Cell_t *cell;

        cell = STRUCT_ENTRY(ret, LibMem_Cell_t, data);

        cell->callerFile = file_str;
        cell->callerLine = line;
    }
    return ret;
}

int LibMem_Free(void *ptr)
{
    LibMem_Cell_t *curr_cell;

    LibMT_UtilMutex_Lock(&gLibMemLock);
    curr_cell = _LibMem_FindCellEntry_ForFree_NotSafe((u8 *)ptr);
    if (curr_cell == NULL) {
        LibMT_UtilMutex_Unlock(&gLibMemLock);
        SAFE_PRINT("Unknown address for free()\n");
        BASIC_ASSERT(0);
        return LIB_MEM_RC_CANT_FIND_CELL;
    }
    
    DLLIST_REMOVE_NODE(_LIB_MEM_HEAD, curr_cell);
    if (gLibMemCurr == curr_cell) {
        gLibMemCurr = (LibMem_Cell_t *)gLibMemHead.head;
    }
    free(curr_cell);
    LibMT_UtilMutex_Unlock(&gLibMemLock);
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
    LibMem_Cell_t *curr_cell;

    LibMT_UtilMutex_Lock(&gLibMemLock);
    curr_cell = _LibMem_FindCellEntry_NotSafe(cell_data_addr);
    LibMT_UtilMutex_Unlock(&gLibMemLock);
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
    ptr = _LIB_MEM_DATA(curr_cell);
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
    ptr = _LIB_MEM_DATA(curr_cell) + curr_cell->size_with_padding;
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
    void *addrC;
    void *addrD;
    void *flag;
    void *prev;
    void *next;

    DUMPNA(_LIB_MEM_HEAD);
    DUMPNA(_LIB_MEM_CURR);

    DLLIST_FOREACH(_LIB_MEM_HEAD, curr_cell, LibMem_Cell_t)
    {
        key = curr_cell->key;
        size = curr_cell->size;
        real_size = curr_cell->real_size;
        addrC = (void *)curr_cell;
        addrD = (void *)_LIB_MEM_DATA(curr_cell);
        flag = (void *)_LIB_MEM_FLAG(curr_cell);
        prev = (void *)curr_cell->entry.prev;
        next = (void *)curr_cell->entry.next;
        DUMPX(key);
        DUMPD(size);
        DUMPD(real_size);
        DUMPA(addrC);
        DUMPA(addrD);
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

void LibMem_Demo(int do_init /*= 0*/)
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

    if (do_init) {
        LibMem_Init();
    }

    ptr = (u8 *)MM_ALLOC(3);
    ptr2 = (u8 *)MM_ALLOC(9);
    ptraaa = (testaaa *)ptr2;

    MM_KEY_INIT(ptr, 2);

    MM_DUMP();

    MM_KEY_INIT(ptr2, ptr2_key);
    MM_CONFIG(&ptraaa->a, sizeof(ptraaa->a), ptr2_key, LIB_MEM_READ_PROTECT_ON);
    MM_CONFIG((u8 *)&ptraaa->b, sizeof(ptraaa->b), ptr2_key, LIB_MEM_WRITE_PROTECT_ON);

    MM_SETK(ptr2_key, ptraaa, 2, sizeof(testaaa));
    WTK(0, ptraaa->a, 0x78);
    MM_DUMP_CELL(ptr2);
    u8 x;
    RDK(ptr2_key, x, ptraaa->a);
    DUMPNX(x);
    RWK(ptr2_key, ptraaa->a, =, 0x0f);
    DUMPNX(ptraaa->a);

    if (do_init) {
        LibMem_Uninit();
    }
}

