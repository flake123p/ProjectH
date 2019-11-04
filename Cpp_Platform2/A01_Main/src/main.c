
#include "Everything_App.hpp"
//#include "unix_sys_queue.h"


#define LIB_MEM_HEAD (&gLibMemHead)
DLList_Entry_t gLibMemHead = DLLIST_HEAD_INIT(LIB_MEM_HEAD);

void *LibMem_Malloc(size_t size)
{
    return NULL;
}

void LibMem_MallocX(size_t size, const char *file_str, int line)
{
    printf("%s, %d\n", file_str, line);
}



int main(int argc, char *argv[])
{
    LibUtile_Demo2();
    //LibMem_Malloc(0, __FILE__, __LINE__);
    return 99;
}


