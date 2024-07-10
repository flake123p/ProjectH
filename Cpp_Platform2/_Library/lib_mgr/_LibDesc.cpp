
#include "Everything_Lib_Mgr.hpp"

Lib_Desc_Head_t *LibDesc_CreateList(size_t size_of_head, u32 num_of_preallocate_desc, size_t size_of_desc)
{
    size_t i;
    Lib_Desc_Head_t *desc_head = (Lib_Desc_Head_t *)MM_ALLOC(size_of_head);
    Lib_Desc_Info_t *desc;

    if (desc_head == NULL) {
        return NULL;
    }

    DLLIST_HEAD_RESET(&(desc_head->head_of_using));
    DLLIST_HEAD_RESET(&(desc_head->head_of_pool));
    desc_head->size_of_desc = size_of_desc;

    for (i=0; i<num_of_preallocate_desc; i++) {
        desc = (Lib_Desc_Info_t *)MM_ALLOC(size_of_desc);
        if (desc == NULL) {
            LibDesc_DestroyList(desc_head, 1);
            return NULL;
        }

        desc->is_pre_allocate = 1;

        //Add to list
        DLLIST_INSERT_LAST(&(desc_head->head_of_pool), desc);
    }

    return desc_head;
}

int LibDesc_DestroyList(Lib_Desc_Head_t *desc_head, int do_free_head)
{
    Lib_Desc_Info_t *prev_desc;
    Lib_Desc_Info_t *curr_desc;

    DLLIST_WHILE_START(&(desc_head->head_of_using), curr_desc, Lib_Desc_Info_t)
    {
        prev_desc = curr_desc;
        DLLIST_WHILE_NEXT(curr_desc, Lib_Desc_Info_t);
        MM_FREE(prev_desc);
    }

    DLLIST_WHILE_START(&(desc_head->head_of_pool), curr_desc, Lib_Desc_Info_t)
    {
        prev_desc = curr_desc;
        DLLIST_WHILE_NEXT(curr_desc, Lib_Desc_Info_t);
        MM_FREE(prev_desc);
    }

    if (do_free_head)
    {
        MM_FREE(desc_head);
    }

    return 0;
}

Lib_Desc_Info_t *LibDesc_GetDesc(Lib_Desc_Head_t *desc_head)
{
    Lib_Desc_Info_t *desc;

    if (DLLIST_IS_NOT_EMPTY(&(desc_head->head_of_pool))) {
        desc = (Lib_Desc_Info_t *)DLLIST_FIRST(&(desc_head->head_of_pool));
        DLLIST_REMOVE_FIRST(&(desc_head->head_of_pool));
    } else {
        desc = (Lib_Desc_Info_t *)MM_ALLOC(desc_head->size_of_desc);
        desc->is_pre_allocate = 0;
    }

    DLLIST_INSERT_LAST(&(desc_head->head_of_using), desc);

    return desc;
}

int LibDesc_ReleaseDesc(Lib_Desc_Head_t *desc_head, Lib_Desc_Info_t *desc)
{
    DLLIST_REMOVE_NODE(&(desc_head->head_of_using), desc);
    if (desc->is_pre_allocate)
    {
        DLLIST_INSERT_LAST(&(desc_head->head_of_pool), desc);
    }
    else
    {
        MM_FREE(desc);
    }
    return 0;
}

int LibDesc_Dump(Lib_Desc_Head_t *desc_head)
{
    Lib_Desc_Info_t *curr_desc;

    printf("list of using:\n");
    DLLIST_FOREACH(&(desc_head->head_of_using), curr_desc, Lib_Desc_Info_t)
    {
        printf("%d -> ", curr_desc->is_pre_allocate);
    }
    printf("\n");

    printf("list of pool:\n");
    DLLIST_FOREACH(&(desc_head->head_of_pool), curr_desc, Lib_Desc_Info_t)
    {
        printf("%d -> ", curr_desc->is_pre_allocate);
    }
    printf("\n");

    return 0;
}

Lib_DescMT_Head_t *LibDescMT_CreateList(u32 num_of_preallocate_desc, size_t size_of_desc)
{
    size_t i;
    Lib_DescMT_Head_t *head = (Lib_DescMT_Head_t *)MM_ALLOC(sizeof(Lib_DescMT_Head_t));
    Lib_Desc_Info_t *desc;

    if (head == NULL) {
        return NULL;
    }

    LibMT_UtilMutex_Init(&(head->lock));

    DLLIST_HEAD_RESET(&(head->head_entry));
    head->size_of_desc = size_of_desc;

    for (i=0; i<num_of_preallocate_desc; i++) {
        desc = (Lib_Desc_Info_t *)MM_ALLOC(size_of_desc);
        if (desc == NULL) {
            LibDescMT_DestroyList(head);
            return NULL;
        }

        desc->is_pre_allocate = 1;

        //Add to list
        DLLIST_INSERT_LAST(&(head->head_entry), desc);
    }

    return head;
}

int LibDescMT_DestroyList(Lib_DescMT_Head_t *head)
{
    Lib_Desc_Info_t *prev_desc;
    Lib_Desc_Info_t *curr_desc;

    DLLIST_WHILE_START(&(head->head_entry), curr_desc, Lib_Desc_Info_t)
    {
        prev_desc = curr_desc;
        DLLIST_WHILE_NEXT(curr_desc, Lib_Desc_Info_t);
        MM_FREE(prev_desc);
    }

    LibMT_UtilMutex_Uninit(&(head->lock));
    MM_FREE(head);

    return 0;
}

Lib_Desc_Info_t *LibDescMT_GetDesc(Lib_DescMT_Head_t *head)
{
    Lib_Desc_Info_t *desc;

    LibMT_UtilMutex_Lock(&(head->lock));
    if (DLLIST_IS_NOT_EMPTY(&(head->head_entry))) {
        desc = (Lib_Desc_Info_t *)DLLIST_FIRST(&(head->head_entry));
        DLLIST_REMOVE_FIRST(&(head->head_entry));
    } else {
        desc = (Lib_Desc_Info_t *)MM_ALLOC(head->size_of_desc);
        desc->is_pre_allocate = 0;
    }
    LibMT_UtilMutex_Unlock(&(head->lock));

    return desc;
}

int LibDescMT_ReleaseDesc(Lib_DescMT_Head_t *head, Lib_Desc_Info_t *desc)
{
    LibMT_UtilMutex_Lock(&(head->lock));
    if (desc->is_pre_allocate)
    {
        DLLIST_INSERT_LAST(&(head->head_entry), desc);
    }
    else
    {
        MM_FREE(desc);
    }
    LibMT_UtilMutex_Unlock(&(head->lock));

    return 0;
}

int LibDescMT_Dump(Lib_DescMT_Head_t *head)
{
    Lib_Desc_Info_t *curr_desc;

    DLLIST_FOREACH(&(head->head_entry), curr_desc, Lib_Desc_Info_t)
    {
        printf("%d -> ", curr_desc->is_pre_allocate);
    }
    printf("\n");

    return 0;
}


typedef struct {
    Lib_Desc_Head_t head_entry;
    u32 mod_id;
} Demo_Module_Instance2_t;

typedef struct {
    Lib_Desc_Info_t desc_entry;
    u32 desc_id;
} Demo_Desc2_t;

void LibDesc_Demo(void)
{
    Demo_Module_Instance2_t *mod_1 = (Demo_Module_Instance2_t *)LibDesc_CreateList(sizeof(Demo_Module_Instance2_t), 2, sizeof(Demo_Desc2_t));

    printf("%s()\n", __func__);

    Demo_Desc2_t *desc1 = (Demo_Desc2_t *)LibDesc_GetDesc((Lib_Desc_Head_t *)mod_1);
    desc1 = desc1;
    Demo_Desc2_t *desc2 = (Demo_Desc2_t *)LibDesc_GetDesc((Lib_Desc_Head_t *)mod_1);
    Demo_Desc2_t *desc3 = (Demo_Desc2_t *)LibDesc_GetDesc((Lib_Desc_Head_t *)mod_1);

    LibDesc_ReleaseDesc((Lib_Desc_Head_t *)mod_1, (Lib_Desc_Info_t *)desc3);
    LibDesc_ReleaseDesc((Lib_Desc_Head_t *)mod_1, (Lib_Desc_Info_t *)desc2);
    LibDesc_Dump((Lib_Desc_Head_t *)mod_1);

    LibDesc_DestroyList((Lib_Desc_Head_t *)mod_1, 1);
}

void LibDesc_DemoMT(void)
{
    Lib_DescMT_Head_t *head = LibDescMT_CreateList(6, 20);
    Lib_Desc_Info_t *desc = LibDescMT_GetDesc(head);
    LibDescMT_ReleaseDesc(head, desc);

    LibDescMT_Dump(head);

    LibDescMT_DestroyList(head);
}