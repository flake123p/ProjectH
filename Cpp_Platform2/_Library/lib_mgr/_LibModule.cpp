
#include "Everything_Lib_Mgr.hpp"

Module_Instance_t *LibModule_InitModuleInstance(size_t size_of_mod_instance, size_t num_of_preallocate_job, size_t size_of_job_node)
{
    size_t i;
    Module_Instance_t *mod_instance = (Module_Instance_t *)malloc(size_of_mod_instance);
    Module_Job_t *job;

    if (mod_instance == NULL) {
        return NULL;
    }

    DLLIST_HEAD_RESET(&(mod_instance->head_of_activated));
    DLLIST_HEAD_RESET(&(mod_instance->head_of_released));
    mod_instance->size_of_job_node = size_of_job_node;

    for (i=0; i<num_of_preallocate_job; i++) {
        job = (Module_Job_t *)malloc(size_of_job_node);
        if (job == NULL) {
            LibModule_UninitModuleInstance(mod_instance);
            return NULL;
        }

        job->mod_instance = mod_instance;
        job->is_pre_allocate = 1;

        //Add to list
        DLLIST_INSERT_LAST(&(mod_instance->head_of_released), job);
    }

    return mod_instance;
}

int LibModule_UninitModuleInstance(Module_Instance_t *mod_instance)
{
    Module_Job_t *prev_job;
    Module_Job_t *curr_job;

    DLLIST_WHILE_START(&(mod_instance->head_of_activated), curr_job, Module_Job_t)
    {
        prev_job = curr_job;
        DLLIST_WHILE_NEXT(curr_job, Module_Job_t);
        free(prev_job);
    }

    DLLIST_WHILE_START(&(mod_instance->head_of_released), curr_job, Module_Job_t)
    {
        prev_job = curr_job;
        DLLIST_WHILE_NEXT(curr_job, Module_Job_t);
        free(prev_job);
    }

    free(mod_instance);
    return 0;
}

Module_Job_t *LibModule_ActivateJob(Module_Instance_t *mod_instance);
int LibModule_ReleaseJob(Module_Job_t *job);
int LibModule_Dump(Module_Instance_t *mod_instance);

typedef struct {
    Module_Instance_t mod_instance;
    u32 mod_id;
} Demo_Module_Instance_t;

typedef struct {
    Module_Job_t cmn_desc;
    u32 desc_id;
} Demo_Desc_t;

void LibModule_Demo(void)
{
    Demo_Module_Instance_t *mod_1 = (Demo_Module_Instance_t *)LibModule_InitModuleInstance(sizeof(Demo_Module_Instance_t), 4, sizeof(Demo_Desc_t));
    LibModule_UninitModuleInstance((Module_Instance_t *)mod_1);
    printf("%s()\n", __func__);
}