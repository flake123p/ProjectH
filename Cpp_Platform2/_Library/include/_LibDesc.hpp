
#ifndef __LIB_DESC_HPP_INCLUDED_

#include "My_Types.h"
#include "_LibLinkedList.hpp"

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

void LibDesc_Demo(void);

#define __LIB_DESC_HPP_INCLUDED_
#endif//__LIB_DESC_HPP_INCLUDED_

