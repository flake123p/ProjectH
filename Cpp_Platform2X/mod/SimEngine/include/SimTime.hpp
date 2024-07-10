

#ifndef _SIM_TIME_HPP_INCLUDED_


#include "Everything_Lib_Mgr.hpp"

typedef struct {
    DLList_Entry_t entry;
    u32 time;
} SimTime_LocalNode_t;

typedef struct {
    DLList_Head_t gSimTime;
    u32 gTime32_L;
    u32 gTime32_H;
    int (*Simple_CB_t)(void);
} SimTime_Main_t;

int SimTime_Demo(void);
int SimTime_SortedList_Demo(void);

#define _SIM_TIME_HPP_INCLUDED_
#endif//_SIM_TIME_HPP_INCLUDED_



