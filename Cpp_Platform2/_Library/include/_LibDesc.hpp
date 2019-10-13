
#ifndef __LIB_DESC_HPP_INCLUDED_

#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================
typedef struct {
    void *head_of_used_desc_list;
    void *tail_of_used_desc_list;
    void *head_of_unused_desc_list; //pre-allocate pool
    void *tail_of_unused_desc_list; //pre-allocate pool
    u32 size_of_desc;
} Cmn_Module_Instance_t;

typedef struct {
    void *prev;
    void *next;
    Cmn_Module_Instance_t *mod_instance;
    int is_pre_allocate_desc;
} Cmn_Desc_t;

Cmn_Module_Instance_t *LibDesc_InitNewModule(u32 size_of_mod_instance, u32 num_of_preallocate_desc, u32 size_of_desc);
int LibDesc_UninitModule(Cmn_Module_Instance_t *mod_instance);
Cmn_Desc_t *LibDesc_GetDesc(Cmn_Module_Instance_t *mod_instance);
int LibDesc_ReleaseDesc(Cmn_Desc_t *desc);
int LibDesc_Dump(Cmn_Module_Instance_t *mod_instance);

void LibDesc_Demo(void);

#define __LIB_DESC_HPP_INCLUDED_
#endif//__LIB_DESC_HPP_INCLUDED_

