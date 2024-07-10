#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

enum{
    REQUEST_INACTIVE,
    REQUEST_ACTIVE
};
enum{
    CLK_TYPE_CLKN,
    CLK_TYPE_CLKB
};
enum{
    ANCHOR_UNIT_CLKU,
    ANCHOR_UNIT_CLOCK
};
enum{
    BANDWIDTH_UNIT_US,
    BANDWIDTH_UNIT_SLOT
};
enum {
    REQUEST_TYPE_PREEMPTIVE,
    REQUEST_TYPE_NORMAL,
    REQUEST_TYPE_MAX
};

typedef enum {
    SIM_SCH_STT_WAITING_1ST_WAKUP_TO_GRANT, //sleep 3xx us
    SIM_SCH_STT_WAITING_PERIODIC_TO_GRANT,
}Sim_Sch_State_t;

typedef struct Scheduler_Request_T {
    TAILQ_ENTRY(Scheduler_Request_T) next;
    u8 request_type;//0: preemptive, 1: normal
    u8 active;  // scheduler will check active request only
    u8 priority;
    u8 CLK_type;                               // module's clock system. 0: CLKN, 1: CLKB
    u8 anchor_unit;                              // the unit of anchor and granted_anchor, 0: clku, 1: clock
    u8 bandwidth_unit;                      // the unit of min_request_bandwidth, max_request_bandwidth and granted_bandwidth, 0: us, 1: slot

    u8 anchor_valid;                        //for preemptive request, 0: scheduler will decide the anchor, 1: module decide the anchor, for normal request anchor_valid initial to 0
    u32 anchor;                                    // module decide anchor of preempt type request, scheduler decide anchor of normal type request


    u32 min_request_bandwidth;      // the minimum effective bandwidth needed
    u32 max_request_bandwidth;      // the maximum bandwidth needed

    // Periodical token attributes, scheduler will re-insert periodical token, 
    //repetitive token is periodical and inteval=bandwidth
    bool   is_periodical;
    u16   max_random_delay_us; //0 is no random, for advertising random delay
    u32 periodical_interval_us;//0 is continuous (if is_periodical is 1), must no less than 1 slot
    Bt_Dev_Info_t *dev;
    u8 data_len;
    u8 *data;

    u8 ID;
    
    u8   ACK_callback_task_queue;
    u32  ACK_callback_message_id;
    u8   NACK_callback_task_queue;
    u32  NACK_callback_message_id;
    u8   ABORT_callback_task_queue;
    u32  ABORT_callback_message_id;
    u8   Clear_Request_callback_task_queue;
    u32  Clear_Request_callback_message_id;


    //below are internal use by scheduler
    u8 state;
    u8 clear_action;
    u32 internal_flags;//bit0: use max (0) or min (1) bandwidth
    
    u8 lose_times;
    u8 priority_backup; // the original priority when module request
    //CLKU timing system in scheduler, re-use below three (anchor, min_request_bandwidth, max_request_bandwidth )
    //, so module should not use them, otherwise, values may be different because timing system is different
    //u32 anchor_clku;                                    // module decide anchor of preempt type request, scheduler decide anchor of normal type request
    //u32 min_request_bandwidth_clku;      // the minimum effective bandwidth needed
    //u32 max_request_bandwidth_clku;      // the maximum bandwidth needed
    //separate internal granted_anchor and granted_bandwidth. both scheduler and module will use.
    //create new two
    u32 granted_anchor_clku;
    u32 granted_bandwidth_clku;
    //above are internal use by scheduler

    //return values
    u32 granted_anchor;
    u32 granted_bandwidth;
#ifdef DFS_SIM_ON
    Sim_Sch_State_t sim_state;
#endif
} Scheduler_Request_T;

typedef struct Request_ADDR_T {
    TAILQ_ENTRY(Request_ADDR_T) next;
    u32 Request_ADDR;
}Request_ADDR_T;
typedef TAILQ_HEAD(SCHED_LIST, Scheduler_Request_T)  Scheduler_List_t ; 

typedef TAILQ_HEAD(REQUEST_ADDRESS_LIST, Request_ADDR_T)  Request_ADDR_List_t ;

Request_ADDR_List_t * Scheduler_APIGetRequestAddressList();
Request_ADDR_T * Scheduler_APIGetRequestAddress();
Scheduler_Request_T * Scheduler_APIGetRequest();

#endif
