
#include "Everything_App.hpp"
//#include "unix_sys_queue.h"


#define LIB_MEM_HEAD (&gLibMemHead)
#define LIB_MEM_CURR (gLibMemCurr)
#define LIB_MEM_DATA(cell) (&(cell->data))
#define LIB_MEM_FLAG(cell) (((u8 *)&(cell->data))+cell->size_with_padding)
#define LIB_MEM_ALLOC(size) LibMem_MallocEx(size,__FILE__,__LINE__)

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

static LibMem_Cell_t *_LibMem_FindCellEntry(void *data_addr)
{
    if (gLibMemCurr == NULL)
        return NULL;

    LibMem_Cell_t *curr_cell = gLibMemCurr;

    while (1)
    {
        if (LIB_MEM_DATA(curr_cell) == data_addr) {
            return curr_cell;
        }
        if (curr_cell->entry.next == NULL)
            break;
        else
            curr_cell = (LibMem_Cell_t *)curr_cell->entry.next;
    }

    while (1)
    {
        if (curr_cell->entry.prev == LIB_MEM_HEAD)
            break;

        curr_cell = (LibMem_Cell_t *)curr_cell->entry.prev;

        if (LIB_MEM_DATA(curr_cell) == data_addr) {
            return curr_cell;
        }
    }

    return NULL;
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

    curr_cell = (LibMem_Cell_t *)malloc(real_size);
    printf("%s(), size=%u, size_with_padding=%u, real_size=%u\n", __func__, (unsigned int)size, (unsigned int)size_with_padding, (unsigned int)real_size);
    if (curr_cell == NULL)
        return NULL;

    _LibMem_Mutex_Lock();
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

int LibMem_RangeCheck(u8 *addr, u32 size, int do_read_check)
{
    // 1 range check
    // 2 initiated check (flag = 0 = uninitiated)
    return 0;
}

int LibMem_KeyInit(void *data_addr, u32 key)
{
    LibMem_Cell_t *curr_cell = _LibMem_FindCellEntry(data_addr);

    if (curr_cell == NULL)
        return 1;

    if (curr_cell->key != 0)
        return 2;

    curr_cell->key = key;
    return 0;
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
    u8 *ptr;
    u8 *ptr2;
    LibMem_Cell_t *curr_cell;
    ptr = (u8 *)LIB_MEM_ALLOC(3);
    ptr2 = (u8 *)LIB_MEM_ALLOC(9);
    curr_cell = _LibMem_FindCellEntry(ptr2);
    curr_cell->size = 999;
    curr_cell = _LibMem_FindCellEntry(ptr);
    curr_cell->size = 888;
    //LibMem_KeyInit(ptr, 2);
    LibMem_Dump();


    LibMem_Uninit();
}

int main(int argc, char *argv[])
{
    //LibUtile_Demo2();

    LibMem_Demo();
    return 99;
}


