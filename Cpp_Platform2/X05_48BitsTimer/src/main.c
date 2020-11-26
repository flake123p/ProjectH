

#include "Everything_App.hpp"

/*
    Hx3 = 500Mhz
    1 cycle = 8 system clock
    1 second = 500,000,000 / 8 = 62,500,000 cycles
    1 ms = 62,500 cycles
    1 us = 62.5 cycles

    Hx4 = 2000Mhz
    1 cycle = 8 system clock ???
    1 second = 2,000,000,000 / 8 = 250,000,000 cycles
    1 ms = 250,000 cycles
    1 us = 250 cycles
*/
#define CYCLES_PER_MS 65000
//
#define MS_PER_HI_BYTES 66076
#define MS_PER_HI_BYTES_REMAINDER 27296
#define CYCLES_PER_US 125
#define CYCLES_PER_US_DIVISOR 1

typedef struct {
    DLList_Entry_t entry;
    u32 time;
    u32 start;
} SimTime_Demo_t;

void SimTime_SortedList_DemoDump(DLList_Head_t *pHead)
{
    SimTime_Demo_t *curr;

    curr=(SimTime_Demo_t *)DLLIST_HEAD(pHead);
    DLLIST_FOREACH(pHead,curr,SimTime_Demo_t)
    {
        printf("%d -> ", curr->time);
    }
    printf("NULL\n");
}

void SimTime_SortedList_Insert(DLList_Head_t *pHead, SimTime_Demo_t *newNode, u32 globalTime, u32 *pFirstDurationCounter)
{
    SimTime_Demo_t *curr, *next;
    u32 firstNodeRemainTime;

    curr = (SimTime_Demo_t *)DLLIST_HEAD(pHead);

    newNode->start = globalTime;
    if (curr == NULL)
    {
        DLLIST_INSERT_FIRST(pHead, newNode);
        return;
    }

    firstNodeRemainTime = curr->time - *pFirstDurationCounter;

    if (newNode->time < firstNodeRemainTime)
    {
        // insert 1st

        // 1.adjust duration of 1st node
        curr->time = firstNodeRemainTime - newNode->time;

        DLLIST_INSERT_FIRST(pHead, newNode);
        *pFirstDurationCounter = 0;
    }
    else
    {
        newNode->time -= firstNodeRemainTime;

        // search & insert
        do
        {
            next = (SimTime_Demo_t *)DLLIST_NEXT(curr);
            if (next == NULL)
            {
                DLLIST_INSERT_AFTER(pHead, curr, newNode);
                break;
            }
            if (newNode->time < next->time)
            {
                DLLIST_INSERT_AFTER(pHead, curr, newNode);
                next->time -= (newNode->time);
                break;
            }
            else
            {
                newNode->time -= next->time;
                curr = next;
            }
        } while (curr != NULL);
    }
}

void SimTime_SortedList_Remove(DLList_Head_t *pHead, SimTime_Demo_t *node)
{
    SimTime_Demo_t *curr, *next;

    DLLIST_FOREACH(pHead, curr, SimTime_Demo_t)
    {
        // matching...
        if (node == curr)
        {
            next = (SimTime_Demo_t *)DLLIST_NEXT(curr);
            if (next != NULL)
            {
                next->time += curr->time;
            }
            DLLIST_REMOVE_NODE(pHead, node);
            break;
        }
    }
}

int SimTime_SortedList_Demo(void)
{
    u32 increment;
    int isTimesup;
    SimTime_Demo_t node[4];
    DLList_Head_t head = DLLIST_HEAD_INIT(&head);
    SimTime_Demo_t *curr;
    u32 globalTime = 0;
    u32 firstDurationCounter = 0;

    node[0].time = 20;
    node[1].time = 50; //50
    node[2].time = 60; //60
    node[3].time = 10;

    //TestCase_48BitsMath();

    SimTime_SortedList_Insert(&head, &node[1], globalTime, &firstDurationCounter);
    SimTime_SortedList_DemoDump(&head);
    SimTime_SortedList_Insert(&head, &node[0], globalTime, &firstDurationCounter);
    SimTime_SortedList_DemoDump(&head);
    SimTime_SortedList_Insert(&head, &node[2], globalTime, &firstDurationCounter);
    SimTime_SortedList_DemoDump(&head);

    //firstDurationCounter = 0;
    while(1)
    {
        isTimesup = 0;
        curr=(SimTime_Demo_t *)DLLIST_HEAD(&head);
        if (curr == NULL)
        {
            break;
        }

        if (firstDurationCounter >= curr->time)
        {
            isTimesup = 1;
            while(1)
            {
                DLLIST_REMOVE_FIRST(&head);
                curr=(SimTime_Demo_t *)DLLIST_HEAD(&head);
                if (curr == NULL)
                    break;
                if (curr->time != 0)
                    break;
            }
            firstDurationCounter = 0;
        }
        printf("[%d][%3d][%4d] ", isTimesup, firstDurationCounter, globalTime);
        SimTime_SortedList_DemoDump(&head);

        increment = 1;
        if (curr != NULL)
        {
            increment = curr->time;
        }
        globalTime += increment;
        firstDurationCounter += increment;
        //sleep now;

        if (0 && globalTime == 5) {
            SimTime_SortedList_Insert(&head, &node[3], globalTime, &firstDurationCounter);
        }
        if (0 && globalTime == 12) {
            SimTime_SortedList_Remove(&head, &node[3]);
            printf("duration of removed node is %d\n", globalTime - node[3].start);
            node[3].time = 10;
            SimTime_SortedList_Insert(&head, &node[3], globalTime, &firstDurationCounter);
        }

    }
    PRINT_LINE;
    return 0;
}


int main(int argc, char *argv[])
{
#if ( 0 )
    Lib_Init();
#else
    Lib_Init(LIB_MT_ENABLE);
#endif

    Timer48();
    //int LibU48_Add48To48(LibU64_t *from, LibU64_t *increment, LibU64_t *result) //return 1 if u64 overflow

    //SimTime_SortedList_Demo();

    //LibU48_TestAll();

    Lib_Uninit();
    return 0;
}


