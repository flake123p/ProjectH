#ifndef __CMN_LE_LM_CONN_STATE_H__
#define __CMN_LE_LM_CONN_STATE_H__

#ifdef DFS_SIM_ON
  #include "stub_for_sim.h"
#else //#ifdef DFS_SIM_ON
  #include "common/hci.h"
  #include "common/cmn_scheduler.h"
  #include "common/le_lc/cmn_le_lc_adv.h"
  #include "common/cmn_le.h"
#endif //#ifdef DFS_SIM_ON

typedef enum{
    LC_CONN_STT_ESTABLISHED = 20,
    LC_CONN_STT_SLEEPING_FOR_ADDING_REQUEST,
    LC_CONN_STT_WAITING_FOR_1ST_GRANT,
    LC_CONN_STT_ON_CONNECTION_EVENT,
    LC_CONN_STT_WAITING_CONNECTION_EVENT,
    LC_CONN_STT_DISCONNECTED,

    LC_CONN_STT_INVALID,
}LC_CONNECTION_STATE_t;
typedef enum{
    LC_CONN_STT_EVT_JUST_RECEIVED_CONN_IND = 0,
    LC_CONN_STT_EVT_JUST_SENT_CONN_IND,
    LC_CONN_STT_EVT_SLEEP_TIMESUP,
    LC_CONN_STT_EVT_SCH_GRANT,
    LC_CONN_STT_EVT_SCH_NACK,
    LC_CONN_STT_EVT_SCH_ABORT,
    LC_CONN_STT_EVT_SCH_CLEAR_REQUEST,
    LC_CONN_STT_EVT_CORR_OK,
    LC_CONN_STT_EVT_PL_RCV_OK,
    LC_CONN_STT_EVT_PL_RCV_ERR,
    LC_CONN_STT_EVT_RX_TIMEOUT,
    LC_CONN_STT_EVT_TX_DONE,
}LC_CONNECTION_STATE_EVENT_t;

typedef struct {
    u8 tx_request_active; // upper set 1 to enable lc to send tx
    u8 tx_request_done; // upper shall set 0 as initial value
    u32 tx_len;
    u8 *tx_buf;
    void *lc_hdl;
} Conn_State_Tx_Request_From_Upper_t;

typedef struct {
    Adv_Connect_Ind_Payload_t conn_ind_payload; //Packet payload from advertiser

    //u8 section
    //u8 Role; // LE_ROLE_SLAVE or LE_ROLE_MASTER
    //u8 Mode; // LE_MODE_CONNECTION or ...
    u8 sn; //S1 for slave,
    u8 nesn; //N1 for slave,

    //u16 section
    u16 conn_hdl;

    //u32 section
    //u32 conn_evt_remain_clks;

    LC_CONNECTION_STATE_t state;

    Bt_Dev_Info_t *prev_dev;
    Bt_Dev_Info_t *next_dev;

    u32 remain_tx_len;
    Conn_State_Tx_Request_From_Upper_t *tx_request;
} Conn_State_Info_t;

#endif //#define __CMN_LE_LM_CONN_STATE_H__

