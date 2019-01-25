#ifndef __CMN_LE_LM_SLA_H__
#define __CMN_LE_LM_SLA_H__

#ifdef DFS_SIM_ON
#define Bt_Dev_Info_t u32
#define Adv_Connect_Ind_Payload_t u32
#else
#include "common/hci.h"
#include "common/cmn_scheduler.h"
#include "common/le_lc/cmn_le_lc_adv.h"
#include "common/cmn_le.h"
#endif

typedef enum{
    CONN_STATE_IDLE,
    
}CONN_STATE_T;

typedef struct Conn_Info_t {
    Adv_Connect_Ind_Payload_t conn_ind_payload; //Packet payload from advertiser

    //u8 section
    u8 Role; // LE_ROLE_SLAVE or LE_ROLE_MASTER
    u8 Mode; // LE_MODE_CONNECTION or ...
    u8 sn; //S1 for slave,
    u8 nesn; //N1 for slave,

    //u16 section
    u16 conn_hdl;

    //u32 section
    u32 conn_evt_remain_clks;
} Conn_Info_t;

//#include "common/mac.h"
typedef enum{
    LM_SLA_MSG_HANDLE_CONNECTION_CREATED,
    LM_SLA_MSG_HANDLE_RX_PACKET_EMPTY_OR_CONTINUE_PDU,
    LM_SLA_MSG_HANDLE_RX_PACKET_START_OR_COMPLETE_PDU,
    LM_SLA_MSG_HANDLE_RX_PACKET_CONTROL_PDU,
    LM_SLA_MSG_HANDLE_TX_PACKET,

    MAX_NUM_OF_LM_SLA_MSG,
}LM_SLA_MSG_T ;

extern s32 lm_sla_entry(u16 id, Bt_Dev_Info_t *dev, s32 len, u8 *data);

#endif //#define __CMN_LE_LM_SLA_H__

