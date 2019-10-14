
#ifndef __LIB_MODULE_HPP_INCLUDED_

#include "My_Types.h"
#include "_LibLinkedList.hpp"

// ============================== Debug ==============================

// ============================== Define ==============================
typedef struct {
    DLList_Head_t head_of_activated;
    DLList_Head_t head_of_released; //pre-allocate pool
    size_t size_of_job_node;
} Module_Instance_t;

typedef struct {
    DLList_Entry_t dlist_entry;
    Module_Instance_t *mod_instance;
    int is_pre_allocate;
} Module_Job_t;

Module_Instance_t *LibModule_InitModuleInstance(size_t size_of_mod_instance, size_t num_of_preallocate_job, size_t size_of_job_node);
int LibModule_UninitModuleInstance(Module_Instance_t *mod_instance);
Module_Job_t *LibModule_ActivateJob(Module_Instance_t *mod_instance);
int LibModule_ReleaseJob(Module_Job_t *job);
int LibModule_Dump(Module_Instance_t *mod_instance);

void LibModule_Demo(void);

#define __LIB_MODULE_HPP_INCLUDED_
#endif//__LIB_MODULE_HPP_INCLUDED_

