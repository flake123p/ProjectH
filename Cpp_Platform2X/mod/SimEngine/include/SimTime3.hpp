
#ifndef _SIM_TIME3_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

//Ver3
/*
    focus on insert before & insert back. Not priority!!
    focus on remove (remove when cb-ing / remove when not cb-ing)
    not add new feature as possible!!
*/
typedef enum {
    TIME3FLAG_IS_IN_TIMESUP_CB  = BIT_0,
    TIME3FLAG_IS_IN_ENTRY_CB    = BIT_1,
    TIME3FLAG_IS_IN_LEAVE_CB    = BIT_2,
}Time3Flag_t;
typedef enum {
    TIME3REQ_BYPASS_CB              = BIT_0,
}Time3Req_t;

#define MAX_TIME_INT 0xFFFFFFFF
typedef u32 timeint_t;
typedef u32 (*SimeTime3_CB_t)(Handle_t handle); //ret: Time3CbRet_t
typedef struct {
    DLList_Entry_t entry;
    timeint_t remain_time;
    u32 flag;   //Time3Flag_t, input for cb
    u32 req;    //Time3Req_t, request from cb
    SimeTime3_CB_t cb; // argument:handle is this descriptor
    void *timeDb;
    void *hdlA; // 2 way composition
    void *hdlB;
    u8 metaData[];
} SimTime3_Desc;

class SimTime3 {
public:
    DLList_Head_t head;
    timeint_t currTime;
    timeint_t minNextTime;
    SimTime3_Desc *pEntry;
    SimTime3_Desc *pLeave;
    SimTime3_Desc *next;
    SimTime3(void){Init();};
    ~SimTime3(void){Uninit();};
    void Init(void);
    void Uninit(void);
    int ClearDesc(SimTime3_Desc *pDes);
    int InsertHead(SimTime3_Desc *pDes, int doInit = 1);
    int InsertTail(SimTime3_Desc *pDes, int doInit = 1);
    int Remove(SimTime3_Desc *pDes);
    int Run(void);
    void Dump(SimTime3_Desc *pDes);
    void DumpAll(void);
};

#define _SIM_TIME3_HPP_INCLUDED_
#endif//_SIM_TIME3_HPP_INCLUDED_