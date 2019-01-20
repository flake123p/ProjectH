

#ifndef _SIM_TIME_SLICE_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

typedef int (*Time_Slice_CB)(void);
//typedef int (*Time_Slice_Mgr_CB)(void *on_going_cb);

typedef struct {
    u32 *p_remain_time;
    Time_Slice_CB times_up_cb;
}Time_Slice_Descriptor;

typedef enum {
    TIME_SLICE_CB_WAITING,
    TIME_SLICE_CB_DONE,

    TIME_SLICE_CB_INVALID,
}Time_Slice_CB_State;
typedef struct {
    u32 remain_time; //ver2, not pointer
    Time_Slice_CB times_up_cb;
    Time_Slice_CB_State state; //ver2
    void *next; //ver2
}Time_Slice_Descriptor2; //for TimeSliceSim_Init2() & TimeSliceSim_Start2()
typedef struct {
    Time_Slice_CB pre_cb;
    Time_Slice_CB post_cb;
}Time_Slice_Descriptor2_Mgr;

int TimeSliceSim_Demo(void);
int TimeSliceSim_Demo2(void);
int TimeSliceSim_Demo3(void);

void TimeSliceSime_GetTimeStamp(u32 *outTimeStamp1, u32 *outTimeStamp2);
void TimeSliceSime_SetTimeStamp(u32 inTimeStamp1, u32 inTimeStamp2);

int TimeSliceSim_Init(Time_Slice_Descriptor *descriptor_table, u32 table_number);
int TimeSliceSim_Start(void);

int TimeSliceSim_Init2(Time_Slice_Descriptor2 *descriptor_list);
int TimeSliceSim_Mgr_Init2(Time_Slice_CB in_pre_cb, Time_Slice_CB in_post_cb);
int TimeSliceSim_Start2(void);

#define _SIM_TIME_SLICE_INCLUDED_
#endif//_SIM_TIME_SLICE_INCLUDED_



