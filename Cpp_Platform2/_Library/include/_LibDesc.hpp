
#ifndef __LIB_DESC_HPP_INCLUDED_

#include "My_Types.h"
#include "_LibLinkedList.hpp"
#include "_LibMT.hpp"

// ============================== Debug ==============================

// ============================== Define ==============================
typedef struct {
    DLList_Head_t head_of_using;
    DLList_Head_t head_of_pool; //pre-allocate pool
    size_t size_of_desc;
} Lib_Desc_Head_t;

typedef struct {
    DLList_Entry_t entry;
    int is_pre_allocate;
} Lib_Desc_Info_t;

Lib_Desc_Head_t *LibDesc_CreateList(size_t size_of_head, u32 num_of_preallocate_desc, size_t size_of_desc);
int LibDesc_DestroyList(Lib_Desc_Head_t *desc_head, int do_free_head);
Lib_Desc_Info_t *LibDesc_GetDesc(Lib_Desc_Head_t *desc_head);
int LibDesc_ReleaseDesc(Lib_Desc_Head_t *desc_head, Lib_Desc_Info_t *desc);
int LibDesc_Dump(Lib_Desc_Head_t *desc_head);



typedef struct {
    DLList_Head_t head_entry;
    LibMT_UtilMutex_t lock;
    size_t size_of_desc;
} Lib_DescMT_Head_t;

Lib_DescMT_Head_t *LibDescMT_CreateList(u32 num_of_preallocate_desc, size_t size_of_desc);
int LibDescMT_DestroyList(Lib_DescMT_Head_t *head);
Lib_Desc_Info_t *LibDescMT_GetDesc(Lib_DescMT_Head_t *head);
int LibDescMT_ReleaseDesc(Lib_DescMT_Head_t *head, Lib_Desc_Info_t *desc);
int LibDescMT_Dump(Lib_DescMT_Head_t *head);



void LibDesc_Demo(void);
void LibDesc_DemoMT(void);


#define __LIB_DESC_HPP_INCLUDED_
#endif//__LIB_DESC_HPP_INCLUDED_

