

#ifndef _SIM_Air_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

typedef u32 SimAir_Handle_t;

typedef enum {
    SIM_AIR_INVALID_REQUEST = 100,

    SIM_AIR_WAKEUP_REQUEST,
    SIM_AIR_RX_REQUEST,
    SIM_AIR_TX_REQUEST,
}SimAir_Request_Type_t;

typedef enum {
    SIM_AIR_INVALID_RESPONSE = 200,

    SIM_AIR_WAKEUP_TIMESUP,
    SIM_AIR_RXING,
    SIM_AIR_RX_DONE,
    SIM_AIR_TXING,
    SIM_AIR_TX_DONE,
}SimAir_Response_Type_t;

typedef struct {
    SimAir_Response_Type_t resp_type;

    u32 clocks_elapsed; //curr clocks

    u32 tx_done_bits;
    u32 rx_done_bits;
    u32 accu_power_for_rx;

    u32 ref_clock_H; //global ticks in SimTimeSlice
    u32 ref_clock_L; //global ticks in SimTimeSlice
}SimAir_Response_t;

typedef struct {
    SimAir_Handle_t hdl;
    SimAir_Request_Type_t requ_type;
    u32 next_wake_up_time; //for wake

    u32 clocks_per_bit; //data rate for tx/rx
    u32 clocks_to_interrupt_trx;
    u32 clocks_total; //total tx/rx time, must bigger than 1

    u32 freq; //for rx/tx
    u32 position_x; //TODO
    u32 position_y; //TODO
    u32 position_z; //TODO
    u32 power;      //TODO

    u32 tx_buf_len_in_bits; //if it's 5, then real tx_data[0] = tx_data[0] & 0x1F, (LSB first)
    u8 *tx_buf; //pre-allocated buffer
    u32 rx_buf_max_size_in_bits;
    u8 *rx_buf; //pre-allocated buffer

    SimAir_Response_t response;
}SimAir_Info_t;

typedef int (*SimAir_CB_t)(SimAir_Info_t *info);
typedef struct {
    SimAir_CB_t wake_cb;
    SimAir_CB_t rx_cb;
    SimAir_CB_t tx_cb;
}SimAir_CB_Set_t;

/*
typedef struct {
    SimAir_CB_t wake_up_done_cb;
    SimAir_CB_t rx_done_cb;
    SimAir_CB_t tx_done_cb;
}SimAir_Descriptor_t;
*/

u32 SimAir_TimeStamp_Low_Get(void);
u32 SimAir_TimeStamp_High_Get(void);

SimAir_Handle_t SimAir_Init_AddDescriptor(SimAir_CB_t wake_up_cb, SimAir_CB_t rxing_cb, SimAir_CB_t txing_cb);
SimAir_Handle_t SimAir_Init_AddDescriptor2(SimAir_CB_Set_t *cb_set);
int SimAir_Start(void);
int SimAir_Uninit(void);
int SimAir_Log_Enable(void);
int SimAir_Log_Disable(void);

int SimAir_Request(SimAir_Info_t *info);

int SimAir_Demo(void);

void SimAir_Dump(void);

//
// For Sim Air Channel Manager
//
int SimAir_ExecuteResponseCallback(SimAir_Info_t *info);

//
// Sim Air Channel Manager
//
int SimAir_ChMgr_AddRequest(SimAir_Info_t *info);
int SimAir_ChMgr_ProcessOneTick(void);
int SimAir_ChMgr_CheckDoesNeedToTick(void);
void SimAir_ChMgr_Uninit(void);

#define _SIM_Air_INCLUDED_
#endif//_SIM_Air_INCLUDED_



