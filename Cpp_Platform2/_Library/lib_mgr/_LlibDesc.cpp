
#include "Everything_Lib_Mgr.hpp"

int LibDesc_UninitModule(Cmn_Module_Instance_t *mod_instance)
{
    //TBD
    //free used list
    //free unused list
    free(mod_instance);
    return 0;
}

Cmn_Module_Instance_t *LibDesc_InitNewModule(u32 size_of_mod_instance, u32 num_of_preallocate_desc, u32 size_of_desc)
{
    u32 i;
    Cmn_Module_Instance_t *mod_instance = (Cmn_Module_Instance_t *)malloc(size_of_mod_instance);
    Cmn_Desc_t *desc;

    if (mod_instance == NULL) {
        return NULL;
    }

    for (i=0; i<num_of_preallocate_desc; i++) {
        desc = (Cmn_Desc_t *)malloc(size_of_desc);
        if (desc == NULL) {
            LibDesc_UninitModule(mod_instance);
            return NULL;
        }

        desc->mod_instance = mod_instance;
        desc->is_pre_allocate_desc = 1;
        //TBD
        //Add to list
    }

    return mod_instance;
}


Cmn_Desc_t *LibDesc_GetDesc(Cmn_Module_Instance_t *mod_instance);
int LibDesc_ReleaseDesc(Cmn_Desc_t *desc);
int LibDesc_Dump(Cmn_Module_Instance_t *mod_instance);

typedef struct {
    Cmn_Module_Instance_t mod_instance;
    u32 mod_id;
} Demo_Module_Instance_t;

typedef struct {
    Cmn_Desc_t cmn_desc;
    u32 desc_id;
} Demo_Desc_t;

void LibDesc_Demo(void)
{
    Demo_Module_Instance_t *mod_1 = (Demo_Module_Instance_t *)LibDesc_InitNewModule(sizeof(Demo_Module_Instance_t), 4, sizeof(Demo_Desc_t));
    LibDesc_UninitModule((Cmn_Module_Instance_t *)mod_1);
    printf("%s()\n", __func__);
}