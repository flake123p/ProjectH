
#include "Everything_Lib_Mgr.hpp"

//#define UNUSED_LIST (&unusedHead)
// todo: rename to DIFF_LIST_HEAD
#define USED_LIST (&head)

// no flag for one shot
#define DIFF_LIST_REPEAT_FINITE   0x00000001
#define DIFF_LIST_REPEAT_INFINITE 0x00000002

#define DIFF_LIST_PRIORITY_HI     0x00010000

#define DIFF_LIST_CLEAR_REPEAT_COUNT  0x20000000
#define DIFF_LIST_IS_IN_CB_PROC   0x40000000
#define DIFF_LIST_IS_ACTIVE       0x80000000 // remove this flag in callback will disable repeat/loop function, don't use this casually
typedef int (*DiffNode_CB_t)(void *node);

struct DiffNode {
    DLList_Entry_t entry;
    DiffNode_CB_t cb;
    u32 id;
    u32 max_repeat_counts;
    u32 flags;
    u32 diff;
    u32 curr_repeat_counts; // for DiffList
    u32 diff_in_list;       // for DiffList
    u32 start_value;        // for DiffList
    u32 end_value;          // for DiffList
    void *listHdl;          // for DiffList
    u8 intraData[];
};

class DiffList {
public:
    DLList_Head_t head;
    u32 accu;
    //u32 accu64hi;
    //u32 none_zero_next_diff;
    //u32 is_none_zero_next_exist;
    DiffList(void);
    ~DiffList(void){}; // Do nothing
    int Insert(struct DiffNode *new_node, int do_clear_repeat_counts = 1);
    int Delete(struct DiffNode *del_node);
    int Restart(struct DiffNode *del_node);
    int Go(void);
    int Go(u32 increment);
    void Dump(void);
};

DiffList::DiffList(void)
{
    DLLIST_HEAD_RESET(&head);
    accu = 0;
    //accu64hi = 0;
    //is_none_zero_next_exist = 0;
}

int DiffList::Insert(struct DiffNode *new_node, int do_clear_repeat_counts /*= 1*/)
{
    struct DiffNode *curr, *next;
    u32 first_node_remain;
    u32 diff_in_list;

    //printf("start\n");
    //Dump();

    diff_in_list = new_node->diff;

    curr = (struct DiffNode *)DLLIST_HEAD(USED_LIST);

    if (curr == NULL)
    {
        DLLIST_INSERT_FIRST(USED_LIST, new_node);
    }
    else
    {
        // in single global timer, first_node_remain is curr->diff_in_list;
        #if 0
        first_node_remain = accu - curr->start_value; //auto turn-around
        #else
        first_node_remain = curr->diff_in_list;
        #endif

        if (diff_in_list < first_node_remain)
        {
            // new node is 1st node now
            // insert new 1st, adjust duration of old 1st node
            curr->diff_in_list -= diff_in_list;
            DLLIST_INSERT_FIRST(USED_LIST, new_node);
        }
        else
        {
            //update new node duration & compare
            diff_in_list -= first_node_remain;

            // search & insert
            do
            {
                next = (struct DiffNode *)DLLIST_NEXT(curr);
                if (next == NULL) {
                    DLLIST_INSERT_AFTER(USED_LIST, curr, new_node);
                    break;
                }
                // compare "next node duration_in_list" to "new node duration"
                if (diff_in_list < next->diff_in_list) {
                    DLLIST_INSERT_AFTER(USED_LIST, curr, new_node);
                    next->diff_in_list -= diff_in_list;
                    break;
                } else if (diff_in_list == next->diff_in_list) {
                    if (new_node->flags & DIFF_LIST_PRIORITY_HI) {
                        DLLIST_INSERT_AFTER(USED_LIST, curr, new_node);
                        next->diff_in_list -= diff_in_list;
                        break;
                    }
                }
                diff_in_list -= next->diff_in_list;
                curr = next;
            } while (1);
        }
    }

    new_node->diff_in_list = diff_in_list;
    if (do_clear_repeat_counts) {
        new_node->curr_repeat_counts = 0;
    }
    new_node->start_value = accu;
    new_node->end_value = accu + new_node->diff;
    new_node->flags |= DIFF_LIST_IS_ACTIVE;
    new_node->listHdl = (void *)this;

    //printf("end\n");
    //Dump();

    return 0;
}

// 0 for delete success
int DiffList::Delete(struct DiffNode *del_node)
{
    if ((void *)this != del_node->listHdl) {
        BASIC_ASSERT(0);
        return 1;
    }

    if (FLG_CHK(del_node->flags, DIFF_LIST_IS_ACTIVE) == 0) {
        BASIC_ASSERT(0);
        return 2;
    }

    if (FLG_CHK(del_node->flags, DIFF_LIST_IS_IN_CB_PROC))
    {
        FLG_RMV(del_node->flags, DIFF_LIST_IS_ACTIVE);
        return 0;
    }

    FLG_RMV(del_node->flags, DIFF_LIST_IS_ACTIVE);
    DLLIST_REMOVE_NODE(USED_LIST, del_node);

    return 0;
}

int DiffList::Restart(struct DiffNode *del_node)
{
    Delete(del_node);

    if (FLG_CHK(del_node->flags, DIFF_LIST_IS_IN_CB_PROC))
    {
        // let Go() insert, like repeat
        FLG_ADD(del_node->flags, DIFF_LIST_IS_ACTIVE);

        FLG_ADD(del_node->flags, DIFF_LIST_CLEAR_REPEAT_COUNT);
    }
    else
    {
        // manual inster
        Insert(del_node, 1);
    }

    return 0;
}

int DiffList::Go(void)
{
    int do_reinsert;
    u32 increment;
    struct DiffNode *first;

    while (1) {
        do_reinsert = 0;
        first = (struct DiffNode *)DLLIST_HEAD(USED_LIST);

        if (first == NULL)
            return 0;

        DLLIST_REMOVE_FIRST(USED_LIST);

        increment = first->diff_in_list;

        if (increment != 0) {
            accu += increment;
            // increment cb
        }
        first->diff_in_list -= increment;

        // 2.infinite loop check
        if (first->flags & DIFF_LIST_REPEAT_INFINITE)
        {
            do_reinsert = 1;
        }

        // 3.finite loop check
        if (first->flags & DIFF_LIST_REPEAT_FINITE)
        {
            first->curr_repeat_counts++;

            if (first->curr_repeat_counts != first->max_repeat_counts)
            {
                do_reinsert = 1;
            }
        }

        if (do_reinsert)
        {
            FLG_ADD(first->flags, DIFF_LIST_IS_IN_CB_PROC);
            (*(first->cb))(first);
            FLG_RMV(first->flags, DIFF_LIST_IS_IN_CB_PROC);

            //repeat
            if (FLG_CHK(first->flags, DIFF_LIST_IS_ACTIVE))
            {
                if (FLG_CHK(first->flags, DIFF_LIST_CLEAR_REPEAT_COUNT))
                {
                    FLG_RMV(first->flags, DIFF_LIST_CLEAR_REPEAT_COUNT);
                    Insert(first, 1);
                }
                else
                {
                    Insert(first, 0);
                }
            }
        }
        else
        {
            FLG_RMV(first->flags, DIFF_LIST_IS_ACTIVE);

            FLG_ADD(first->flags, DIFF_LIST_IS_IN_CB_PROC);
            (*(first->cb))(first);
            FLG_RMV(first->flags, DIFF_LIST_IS_IN_CB_PROC);
        }
    }
    return 1;
}

int DiffList::Go(u32 increment)
{
    int do_reinsert;
    //u32 increment;
    struct DiffNode *first;

    do {
        do_reinsert = 0;
        first = (struct DiffNode *)DLLIST_HEAD(USED_LIST);

        if (first == NULL)
            return 0;

        DLLIST_REMOVE_FIRST(USED_LIST);

        increment = first->diff_in_list;

        if (increment != 0) {
            accu += increment;
            // increment cb
        }
        first->diff_in_list -= increment;

        if (first->diff_in_list)
            break;

        // 2.infinite loop check
        if (first->flags & DIFF_LIST_REPEAT_INFINITE)
        {
            do_reinsert = 1;
        }

        // 3.finite loop check
        if (first->flags & DIFF_LIST_REPEAT_FINITE)
        {
            first->curr_repeat_counts++;

            if (first->curr_repeat_counts != first->max_repeat_counts)
            {
                do_reinsert = 1;
            }
        }

        if (do_reinsert)
        {
            FLG_ADD(first->flags, DIFF_LIST_IS_IN_CB_PROC);
            (*(first->cb))(first);
            FLG_RMV(first->flags, DIFF_LIST_IS_IN_CB_PROC);

            //repeat
            if (FLG_CHK(first->flags, DIFF_LIST_IS_ACTIVE))
            {
                if (FLG_CHK(first->flags, DIFF_LIST_CLEAR_REPEAT_COUNT))
                {
                    FLG_RMV(first->flags, DIFF_LIST_CLEAR_REPEAT_COUNT);
                    Insert(first, 1);
                }
                else
                {
                    Insert(first, 0);
                }
            }
        }
        else
        {
            FLG_RMV(first->flags, DIFF_LIST_IS_ACTIVE);

            FLG_ADD(first->flags, DIFF_LIST_IS_IN_CB_PROC);
            (*(first->cb))(first);
            FLG_RMV(first->flags, DIFF_LIST_IS_IN_CB_PROC);
        }
    }while (0);
    return 1;
}

void DiffList::Dump(void)
{
    struct DiffNode *curr;

    DLLIST_FOREACH(&head, curr, struct DiffNode)
    {
        printf("id=%d(%d) ->",
            curr->id,
            curr->diff_in_list);
    }

    printf("\n");
}

DiffList lx;
struct DiffNode gNode[3];
int LibList_Demo_CB0(void *node)
{
    PRINT_FUNC;
    struct DiffNode *pNode = (struct DiffNode *)node;
    DiffList *p = (DiffList *)pNode->listHdl;
    printf("accu=%d, start=%d, end=%d\n", p->accu, pNode->start_value, pNode->end_value);

    static int x = 1;
    if(x)
    {
        x=0;
        //lx.Restart(pNode);
        lx.Restart(&(gNode[1]));
    }
    return 0;
}

int LibList_Demo_CB1(void *node)
{
    PRINT_FUNC;
    struct DiffNode *pNode = (struct DiffNode *)node;
    DiffList *p = (DiffList *)pNode->listHdl;
    printf("accu=%d, start=%d, end=%d\n", p->accu, pNode->start_value, pNode->end_value);
    return 0;
}

int LibList_Demo_CB2(void *node)
{
    PRINT_FUNC;
    struct DiffNode *pNode = (struct DiffNode *)node;
    DiffList *p = (DiffList *)pNode->listHdl;
    printf("accu=%d, start=%d, end=%d\n", p->accu, pNode->start_value, pNode->end_value);
    return 0;
}

void LibList_Demo(void)
{
    int i;
    //DiffList lx;
    //struct DiffNode node[3];
    i=0;
    gNode[i].cb = LibList_Demo_CB0;
    gNode[i].id = i;
    gNode[i].flags = DIFF_LIST_REPEAT_FINITE | DIFF_LIST_PRIORITY_HI;
    gNode[i].diff = 10;
    gNode[i].max_repeat_counts = 3;
    lx.Insert(&(gNode[i]));
    i=1;
    gNode[i].cb = LibList_Demo_CB1;
    gNode[i].id = i;
    gNode[i].flags = DIFF_LIST_REPEAT_FINITE | 0;
    gNode[i].diff = 5;
    gNode[i].max_repeat_counts = 3;
    lx.Insert(&(gNode[i]));
    i=2;
    gNode[i].cb = LibList_Demo_CB2;
    gNode[i].id = i;
    gNode[i].flags = DIFF_LIST_REPEAT_FINITE | DIFF_LIST_PRIORITY_HI;
    gNode[i].diff = 10;
    gNode[i].max_repeat_counts = 3;
    //lx.Insert(&(gNode[i]));

    //lx.Go();
    while(lx.Go(1)) {
    };
}

/*
    [ ] implement delete func
    [ ] implement restart func
    [ ] go() function with increment input
        [ ] nested go() lists demo
    [ ] implement auto tests
    [ ] sim engine
    [ ] simple tx rx, sim channel, ideal channel
        [ ] DHCP
    [ ] VCD
    [ ] wireshark log
*/
