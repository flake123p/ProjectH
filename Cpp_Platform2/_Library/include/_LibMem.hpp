

#ifndef __LIB_MEM_HPP_INCLUDED_

#include "My_Types.h"
#include "_LibLinkedList.hpp" //LinkedListClass

// ============================== Debug ==============================

// ============================== Define ==============================
#define LIB_MEM_ASSERT_ENABLE ( 1 )

#define SAFE_FREE(ptr)      if(ptr!=NULL){MM_FREE(ptr);}
#define SAFE_FREE2(ptr)     if(ptr!=NULL){MM_FREE(ptr);}ptr=NULL
#define SAFE_DELETE(ptr)    if(ptr!=NULL){delete(ptr);}
#define SAFE_DELETE2(ptr)   if(ptr!=NULL){delete(ptr);}ptr=NULL

#define LIB_MEM_READ_CHECK(addr,len,key) LibMem_ReadCheckEx((u8 *)(addr),len,key,__FILE__,__LINE__)
#define LIB_MEM_WRITE_CHECK(addr,len,key,doWrite) LibMem_WriteCheckEx((u8 *)(addr),len,key,doWrite,__FILE__,__LINE__)

#if ENABLE_LIB_MEM
#define MM_INIT()                    LibMem_Init()
#define MM_UNINIT()                  LibMem_Uninit()
#define MM_ALLOC(size)               LibMem_MallocEx(size,__FILE__,__LINE__)
#define MM_ALLOC2(size,log)          LibMem_MallocEx(size,log,99999)
#define MM_FREE(ptr)                 LibMem_Free(ptr)
#define MM_KEY_INIT(addr,key)        LibMem_KeyInit(addr,key)
#define MM_CONFIG(addr,len,key,flag) LibMem_ConfigureProtection(addr,len,key,flag)
#define MM_DUMP()                    LibMem_Dump()
#define MM_DUMP_CELL(addr)           LibMem_DumpCell(addr)
//void * memset ( void * ptr, int value, size_t num );
#define MM_SET(ptr,val,num)      LIB_MEM_WRITE_CHECK(ptr,num,0,1);memset(ptr,val,num)
#define MM_SETK(key,ptr,val,num) LIB_MEM_WRITE_CHECK(ptr,num,key,1);memset(ptr,val,num)
//void * memcpy ( void * destination, const void * source, size_t num );
#define MM_CPY(dst,src,num)       LIB_MEM_WRITE_CHECK(dst,num,0,1);memcpy(dst,src,num)
#define MM_CPYK(key,dst,src,num)  LIB_MEM_WRITE_CHECK(dst,num,key,1);memcpy(dst,src,num)
#define MM_CPYB(dst,src,num)      LIB_MEM_WRITE_CHECK(dst,num,0,1);LIB_MEM_READ_CHECK(&(src),sizeof(src),0);memcpy(dst,src,num)
#define MM_CPYKB(key,dst,src,num) LIB_MEM_WRITE_CHECK(dst,num,key,1);LIB_MEM_READ_CHECK(&(src),sizeof(src),key);memcpy(dst,src,num)
#define WT(mem,a)         LIB_MEM_WRITE_CHECK(&(mem),sizeof(mem),0,1);(mem)=(a)
#define WTK(key,mem,a)    LIB_MEM_WRITE_CHECK(&(mem),sizeof(mem),key,1);(mem)=(a)
#define RW(mem,op,a)      LIB_MEM_READ_CHECK(&(mem),sizeof(mem),0);LIB_MEM_WRITE_CHECK(&(mem),sizeof(mem),0,1);(mem)op(mem)
#define RWK(key,mem,op,a) LIB_MEM_READ_CHECK(&(mem),sizeof(mem),key);LIB_MEM_WRITE_CHECK(&(mem),sizeof(mem),key,1);(mem)op(mem)
#define RD(a,mem)         LIB_MEM_READ_CHECK(&(mem),sizeof(mem),0);(a)=(mem)
#define RDK(key,a,mem)    LIB_MEM_READ_CHECK(&(mem),sizeof(mem),key);(a)=(mem)
//Bi-direction macros
#define RWB(mem,op,mem2)           LIB_MEM_READ_CHECK(&(mem),sizeof(mem),0);LIB_MEM_WRITE_CHECK(&(mem),sizeof(mem),0,1);LIB_MEM_READ_CHECK(&(mem2),sizeof(mem2),0);(mem)op(mem2)
#define RWKB(key,key2,mem,op,mem2) LIB_MEM_READ_CHECK(&(mem),sizeof(mem),key);LIB_MEM_WRITE_CHECK(&(mem),sizeof(mem),key,1);LIB_MEM_READ_CHECK(&(mem2),sizeof(mem2),key2);(mem)op(mem2)
#else
#define MM_INIT()
#define MM_UNINIT()
#define MM_ALLOC(size) malloc(size)
#define MM_ALLOC2(size,log) malloc(size)
#define MM_FREE(ptr)   free(ptr)
#define MM_KEY_INIT(addr,key)
#define MM_CONFIG(addr,len,key,flag)
#define MM_DUMP()
#define MM_DUMP_CELL(addr)
//void * memset ( void * ptr, int value, size_t num );
#define MM_SET(ptr,val,num)       memset(ptr,val,num)
#define MM_SETK(key,ptr,val,num)  memset(ptr,val,num)
//void * memcpy ( void * destination, const void * source, size_t num );
#define MM_CPY(dst,src,num)       memcpy(dst,src,num)
#define MM_CPYK(key,dst,src,num)  memcpy(dst,src,num)
#define MM_CPYB(dst,src,num)      memcpy(dst,src,num)
#define MM_CPYKB(key,dst,src,num) memcpy(dst,src,num)
#define WT(mem,a)         (mem)=(a)
#define WTK(key,mem,a)    (mem)=(a)
#define RW(mem,op,a)      (mem)op(a)
#define RWK(key,mem,op,a) (mem)op(a)
#define RD(mem,a)         (mem)=(a)
#define RDK(key,mem,a)    (mem)=(a)
//Bi-direction macros
#define RWB(mem,op,mem2)           (mem)op(mem2)
#define RWKB(key,key2,mem,op,mem2) (mem)op(mem2)
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
    const char *callerFile;
    int callerLine;
    u8 *data;
} LibMem_Cell_t;

void LibMem_Init(void);
void LibMem_Uninit(void);
void *LibMem_Malloc(size_t size);
void *LibMem_MallocEx(size_t size, const char *file_str, int line);
int LibMem_Free(void *ptr);
int LibMem_RangeCheck(u8 *addr, u32 size, int do_read_check);
int LibMem_KeyInit(u8 *cell_data_addr, u32 key);
int LibMem_ConfigureProtection(u8 *any_addr, u32 len, u32 key, u8 act_flags/*LIB_MEM_FLAG_t*/);
int LibMem_ReadCheck(u8 *any_addr, u32 len, u32 key);
int LibMem_ReadCheckEx(u8 *any_addr, u32 len, u32 key, const char *file_str, int line);
int LibMem_WriteCheck(u8 *any_addr, u32 len, u32 key, int do_write);
int LibMem_WriteCheckEx(u8 *any_addr, u32 len, u32 key, int do_write, const char *file_str, int line);
void LibMem_DumpCell(u8 *any_addr);
void LibMem_Dump(void);
void LibMem_Demo(int do_init = 0);



#define __LIB_MEM_HPP_INCLUDED_
#endif//__LIB_MEM_HPP_INCLUDED_

