#ifndef __BT_PHY_H__
#define __BT_PHY_H__

int BT_Phy_Log_Enable(void);
int BT_Phy_Log_Disable(void);
int BT_Phy_Wake_CLKN(SimAir_Info_t *info);
int BT_Phy_Wake_CLKB(SimAir_Info_t *info);
int BT_Phy_Wake_Scheduler(SimAir_Info_t *info);
int BT_Phy_Wake_Timer(SimAir_Info_t *info);
int BT_Phy_Wake_TIFS_Timer(SimAir_Info_t *info);
int BT_Phy_Tx(SimAir_Info_t *info);
int BT_Phy_Rx(SimAir_Info_t *info);

typedef enum {
    BT_PHY_RX_0_COMPARE_PREAMBLE,
    BT_PHY_RX_1_COMPARE_ACCESS_CODE, /* done for correlation OK*/
    BT_PHY_RX_2_STORE_HEADER,
    BT_PHY_RX_3_STORE_PAYLOAD,
    BT_PHY_RX_4_COMPARE_CRC,
}BT_PHY_Rx_State_t;

#define PHY_TR_BUF_SIZE (4000)
typedef struct {
    bool sim_destroy;
    u32 clkn;
    u32 clkb;

    SimAir_Info_t air_info[SIM_AIR_TASK_NUMBER];
    u8 phy_rx_buf[PHY_TR_BUF_SIZE];
    u8 phy_tx_buf[PHY_TR_BUF_SIZE];

    //timer 0
    Bt_Dev_Info_t *timer0_dev;
    u32 timer0_sleep_time_in_us;

    //scheduler
    Scheduler_Request_T *scheduler0;
    bool scheduler0_enable;

    //Tx Rx
    Bt_Dev_Info_t *rf_dev;
    u8 ACCESS_CODE[4];
    u8 LE_Control_Reg_LLbyteZ;
    u8 LE_HOP_CTRL0_Reg_HLbyteZ;
    u8 LE_PHY_CRC24[3];
    u8 *RXLE_PLH_SP;
    u8 *RXLE_PLH_CP;
    u8 *RXLE_PLH_EP;
    u8 *RXLE_PLD_SP;
    u8 *RXLE_PLD_CP;
    u8 *RXLE_PLD_EP;
    u8 *TXLE_PLH_SP;
    u8 *TXLE_PLH_CP_RE;
    u8 *TXLE_PLH_EP;
    u8 *TXLE_PLD_SP;
    u8 *TXLE_PLD_CP_RE;
    u8 *TXLE_PLD_EP;

    u8  DIRECT_RFIELD_DISABLE_TIMEOUTz;
    u32 DIRECT_RFIELD_RX_TIMEOUT_VALUEz;
    
    u32 T0_SLOT_TIMER;

    u8 RXENABLE; // 1 for immediate, 2 for TIFS
    u8 TXENABLE; // 1 for immediate, 2 for TIFS

    u8 is_prepare_rf;

    //RX
    BT_PHY_Rx_State_t rx_state;
    u8 rx_work_buf[4];
    u32 rx_work_buf_ctr;
    u32 rx_payload_len_in_bits;
}BT_PHY_Info_t;

#define PHY_INFO ((BT_PHY_Info_t *)g_ll_info->phy_info)

#define SET_RF_DEV(a)              PHY_INFO->rf_dev=a
#define LE_PHY_ACCESS_CODE_LLbyte  PHY_INFO->ACCESS_CODE[0]
#define LE_PHY_ACCESS_CODE_LHbyte  PHY_INFO->ACCESS_CODE[1]
#define LE_PHY_ACCESS_CODE_HLbyte  PHY_INFO->ACCESS_CODE[2]
#define LE_PHY_ACCESS_CODE_HHbyte  PHY_INFO->ACCESS_CODE[3]
#define LE_Control_Reg_LLbyte      PHY_INFO->LE_Control_Reg_LLbyteZ
#define LE_HOP_CTRL0_Reg_HLbyte    PHY_INFO->LE_HOP_CTRL0_Reg_HLbyteZ
#define LE_PHY_CRC24_LL            PHY_INFO->LE_PHY_CRC24[0]
#define LE_PHY_CRC24_LH            PHY_INFO->LE_PHY_CRC24[1]
#define LE_PHY_CRC24_HL            PHY_INFO->LE_PHY_CRC24[2]
#define DIRECT_RFIELD_AGU_RXLE_PLH_SP    PHY_INFO->RXLE_PLH_SP
#define DIRECT_RFIELD_AGU_RXLE_PLH_CP    PHY_INFO->RXLE_PLH_CP
#define DIRECT_RFIELD_AGU_RXLE_PLH_EP    PHY_INFO->RXLE_PLH_EP
#define DIRECT_RFIELD_AGU_RXLE_PLD_SP    PHY_INFO->RXLE_PLD_SP
#define DIRECT_RFIELD_AGU_RXLE_PLD_CP    PHY_INFO->RXLE_PLD_CP
#define DIRECT_RFIELD_AGU_RXLE_PLD_EP    PHY_INFO->RXLE_PLD_EP
#define DIRECT_RFIELD_AGU_TXLE_PLH_SP    PHY_INFO->TXLE_PLH_SP
#define DIRECT_RFIELD_AGU_TXLE_PLH_CP_RE PHY_INFO->TXLE_PLH_CP_RE
#define DIRECT_RFIELD_AGU_TXLE_PLH_EP    PHY_INFO->TXLE_PLH_EP
#define DIRECT_RFIELD_AGU_TXLE_PLD_SP    PHY_INFO->TXLE_PLD_SP
#define DIRECT_RFIELD_AGU_TXLE_PLD_CP_RE PHY_INFO->TXLE_PLD_CP_RE
#define DIRECT_RFIELD_AGU_TXLE_PLD_EP    PHY_INFO->TXLE_PLD_EP
#define LET0T1STOP                       PHY_INFO->RXENABLE=0;PHY_INFO->TXENABLE=0;PHY_INFO->is_prepare_rf=0
#define DIRECT_RFIELD_DISABLE_TIMEOUT    PHY_INFO->DIRECT_RFIELD_DISABLE_TIMEOUTz
#define DIRECT_RFIELD_RX_TIMEOUT_VALUE   PHY_INFO->DIRECT_RFIELD_RX_TIMEOUT_VALUEz

#define T0RXENABLE                       PHY_INFO->RXENABLE=1
#define T1RXENABLE                       PHY_INFO->RXENABLE=1
#define T0TXENABLE                       PHY_INFO->TXENABLE=1
#define T1TXENABLE                       PHY_INFO->TXENABLE=1
#define T0RXENABLE_TIFS                  PHY_INFO->RXENABLE=2
#define T1RXENABLE_TIFS                  PHY_INFO->RXENABLE=2
#define T0TXENABLE_TIFS                  PHY_INFO->TXENABLE=2
#define T1TXENABLE_TIFS                  PHY_INFO->TXENABLE=2


#endif //#define __BT_PHY_H__


