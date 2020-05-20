

#ifndef _SIM_TIME_SLICE_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

//Ver1
typedef struct {
    u32 *p_remain_time;
    Simple_CB_t times_up_cb;
}Time_Slice_Descriptor;

//Ver2
typedef enum {
    TIME_SLICE_CB_WAITING,
    TIME_SLICE_CB_DONE,

    TIME_SLICE_CB_INVALID,
}TimeSlice_CB_State;
typedef struct {
    u32 remain_time; //ver2, not pointer
    TimeSlice_CB_State state; //ver2

    Common_CB_t times_up_cb;
    Handle_t hdl_to_cb;
//    void *lower_hdl; //for internal time slice module
}Time_Slice_Descriptor2; //for SimTimeSlice2_Init() & SimTimeSlice2_Start()
typedef struct {
    Simple_CB_t pre_cb;
    Simple_CB_t post_cb;
}Time_Slice_Descriptor2_Ext;

typedef int (*SimTimeSlice_TimeAdd_CB_t)(u32 timeToAdd);

int SimTimeSlice_Demo_Old(void);
int SimTimeSlice1_Demo(void);
int SimTimeSlice2_Demo(void);

u32 SimTimeSlice_TimeStamp_Low_Get(void);
u32 SimTimeSlice_TimeStamp_High_Get(void);
void SimTimeSlice_TimeStampGet(u32 *outTimeStamp1, u32 *outTimeStamp2);
void SimTimeSlice_TimeStampSet(u32 inTimeStamp1, u32 inTimeStamp2);

int SimTimeSlice_Init(Time_Slice_Descriptor *descriptor_table, u32 table_number);
int SimTimeSlice_RegisterTimeAddCallback(SimTimeSlice_TimeAdd_CB_t timeAdd_CB);

int SimTimeSlice_Start(void);

Time_Slice_Descriptor2 *SimTimeSlice2_GetLastStandaloneDes(void);
int SimTimeSlice2_Init_AddDescriptor(Time_Slice_Descriptor2 *p_descriptor);
int SimTimeSlice2_Init_PrePostCB(Simple_CB_t in_pre_cb, Simple_CB_t in_post_cb);
int SimTimeSlice2_Start(void);
int SimTimeSlice2_Uninit(void);

void SimTimeSlice2_Dump(void);

#define _SIM_TIME_SLICE_INCLUDED_
#endif//_SIM_TIME_SLICE_INCLUDED_



