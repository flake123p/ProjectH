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
    LC_CONN_ROLE_MASTER,
    LC_CONN_ROLE_SLAVE,
}LC_CONNECTION_ROLE_t;

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

#define LL_TX_BUF_SIZE (10)
typedef struct {
    Bt_Dev_Info_t *dev_head;
    Bt_Dev_Info_t *dev_tail;
    u8 *tx_hdr_buf;
    u8 *tx_pld_buf;
    u8 *rx_hdr_buf;
    u8 *rx_pld_buf;
    u32 rx_pld_buf_curr_index;

    void *phy_info;
} LL_Info_t;

typedef struct Conn_State_Tx_Request_t{
    u8 tx_request_active; // upper set 1 to enable lc to send tx
    u8 tx_request_done; // upper shall set 0 as initial value
    u32 tx_len;
    u8 *tx_buf;
    Conn_State_Tx_Request_t *next;
} Conn_State_Tx_Request_t;

typedef struct {
    Adv_Connect_Ind_Payload_t conn_ind_payload; //Packet payload from advertiser

    //LC:read/write, LM:forbiden
    LC_CONNECTION_ROLE_t role;
    LC_CONNECTION_STATE_t state;
    u8 channel;
    u8 llid;
    u8 sn; //S1 for slave,
    u8 nesn; //N1 for slave,
    u8 md;
    u8 tx_len;
    u8 *tx_buf;
    u8 last_rx_nesn_is_match;
    u8 last_tx_is_acked; //init to 1, set to 0 before new tx
    u32 accu_tx_len; //accumulate
    u32 window_size_in_us;
    u32 window_widen_size_in_us;
    u32 window_size_remain_in_us; //TODO, remain window size in single connection event
    u8 timeout_ctr; //supervision timer

    //LC:write, LM:read
    u32 tx_ctr;
    u32 rx_ctr;
    u16 conn_hdl;

    //LC:read, LM:write
    u8 tx_max_len;
    u8 tx_md_enable;
    u8 rx_md_enable;
    u8 tx_hold; // 1 for hold, tx empty packet
    u8 rx_hold; // 1 for hold, rx nack to peer device, peer device will keep resend data repeatly.

    //LC:read/write, LM:forbiden
    Conn_State_Tx_Request_t *curr_tx_request;
    //LC:read pointer, only write lm_tx_request->tx_request_done
    //LM:read/write pointer,
    Conn_State_Tx_Request_t *lm_tx_request;

    //LC init, LM read/write, LC don't care
    Bt_Dev_Info_t *prev_dev;
    Bt_Dev_Info_t *next_dev;
} Conn_State_Info_t;

void lc_conn_state_ll_info_set(LL_Info_t *ll_info);
void lc_conn_state_init(void);
void lc_mas_handle_conn_ind(Bt_Dev_Info_t *dev_from_ini);
void lc_sla_handle_conn_ind(Bt_Dev_Info_t *dev_from_adv);
void lc_mas_conn_state_machine(Bt_Dev_Info_t *mas_dev, LC_CONNECTION_STATE_EVENT_t evt);
void lc_sla_conn_state_machine(Bt_Dev_Info_t *sla_dev, LC_CONNECTION_STATE_EVENT_t evt);

#endif //#define __CMN_LE_LM_CONN_STATE_H__

