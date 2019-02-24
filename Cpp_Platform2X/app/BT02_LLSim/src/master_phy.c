





#if 0









#include "Everything_App.hpp"

extern SimAir_Info_t g_master_info[SIM_AIR_TASK_NUMBER];

u32 g_is_simulation_times_up = 0;

Bt_Dev_Info_t *g_dev_mas_timer_00;
u32 g_sleep_time_in_us;

void MAS_PHY_API_Timer_0(Bt_Dev_Info_t *mas_dev, u32 sleep_time_in_us)
{
    g_dev_mas_timer_00 = mas_dev;
    g_sleep_time_in_us = sleep_time_in_us;
    g_master_info[SIM_AIR_TASK_TIMER_0].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info[SIM_AIR_TASK_TIMER_0].next_wake_up_time = sleep_time_in_us * g_master_info[SIM_AIR_TASK_TIMER_0].clocks_per_bit;
    SimAir_Request(&(g_master_info[SIM_AIR_TASK_TIMER_0]));
}

int Master_Phy_Wake_Timer0(SimAir_Info_t *info)
{
    MASTER_DUMP2(" timer0 wake up, sleep_time_in_us=%d\n", g_sleep_time_in_us);

    printf("%s() -- %d\n", __func__, info->response.ref_clock_L);
    {
        extern void lc_mas_conn_state_machineXX(Bt_Dev_Info_t *mas_dev, LC_CONNECTION_STATE_EVENT_t evt);
        lc_mas_conn_state_machineXX(g_dev_mas_timer_00, LC_CONN_STT_EVT_SLEEP_TIMESUP);
    }

    return 0;
}

int Master_Phy_Wake_TRx0(SimAir_Info_t *info)
{
    printf("%s() -- %d\n", __func__, info->response.ref_clock_L);

    return 0;
}

int Master_Phy_Rx_0(SimAir_Info_t *info)
{
    printf("%s() -- %d, resp_type:%d, rx_done_bits:%d\n", __func__, info->response.ref_clock_L, info->response.resp_type, info->response.rx_done_bits);

    return 0;
}

int Master_Phy_Tx_0(SimAir_Info_t *info)
{
    printf("%s() -- %d\n", __func__, info->response.ref_clock_L);

    return 0;
}

//Bt_Dev_Info_t *g_dev_mas_sch_00;
Scheduler_Request_T *g_sch_requ_mas_sch_00;
void MAS_PHY_API_Sch_0_Add_RequestXX(Scheduler_Request_T *p_sch_requ)
{
    u32 sch_delay_in_us = scheduler_simulate_delay_in_us();
    g_sch_requ_mas_sch_00 = p_sch_requ;

    //DUMPD(g_sch_requ_mas_sch_00->periodical_interval_us);
    g_sch_requ_mas_sch_00->sim_state = SIM_SCH_STT_WAITING_1ST_WAKUP_TO_GRANT;

    g_master_info[SIM_AIR_TASK_SCH_0].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info[SIM_AIR_TASK_SCH_0].next_wake_up_time = sch_delay_in_us * g_master_info[SIM_AIR_TASK_SCH_0].clocks_per_bit;
    //DUMPD(g_master_info[SIM_AIR_TASK_SCH_0].next_wake_up_time);
    MASTER_DUMP2(" Sch 0 add request, sch_delay_in_us = %d\n", sch_delay_in_us);
    SimAir_Request(&(g_master_info[SIM_AIR_TASK_SCH_0]));
}

int Master_Phy_Wake_Sch0(SimAir_Info_t *info)
{
    if (g_is_simulation_times_up)
        return 0;

    //printf(" [ MAS ][ Sch 0]%s() -- %d\n", __func__, info->response.ref_clock_L);
    //MASTER_DUMP(" [ MAS ][ Sch 0 ][%8d]\n", info->response.ref_clock_L);
    MASTER_DUMP1(" Sch 0 wake up\n");

    //if (g_sch_requ_mas_sch_00->sim_state == SIM_SCH_STT_WAITING_1ST_WAKUP_TO_GRANT)
    {
        extern void lc_mas_conn_state_machineXX(Bt_Dev_Info_t *mas_dev, LC_CONNECTION_STATE_EVENT_t evt);
        lc_mas_conn_state_machineXX(g_sch_requ_mas_sch_00->dev, LC_CONN_STT_EVT_SCH_GRANT);
    }

    g_master_info[SIM_AIR_TASK_SCH_0].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info[SIM_AIR_TASK_SCH_0].next_wake_up_time = g_sch_requ_mas_sch_00->periodical_interval_us * g_master_info[SIM_AIR_TASK_SCH_0].clocks_per_bit;
    SimAir_Request(&(g_master_info[SIM_AIR_TASK_SCH_0]));

    return 0;
}

#define ___TX_RX________________________
#define ___TX_RX_______________________
#define ___TX_RX______________________
u8 *MAS__RXLE_PLH_SP = NULL;
u8 *MAS__RXLE_PLH_CP = NULL;
u8 *MAS__RXLE_PLH_EP = NULL;
u8 *MAS__RXLE_PLD_SP = NULL;
u8 *MAS__RXLE_PLD_CP = NULL;
u8 *MAS__RXLE_PLD_EP = NULL;
u8 *MAS__TXLE_PLH_SP = NULL;
u8 *MAS__TXLE_PLH_CP_RE = NULL;
u8 *MAS__TXLE_PLH_EP = NULL;
u8 *MAS__TXLE_PLD_SP = NULL;
u8 *MAS__TXLE_PLD_CP_RE = NULL;
u8 *MAS__TXLE_PLD_EP = NULL;
u8 MAS__ACCESS_CODE[4];
u8 MAS__LE_Control_Reg_LLbyte;
u8 MAS__LE_HOP_CTRL0_Reg_HLbyte; //Bit 7 is for fixed channel setting
u8 MAS__LE_PHY_CRC24[3];
u8 MAS__DIRECT_RFIELD_DISABLE_TIMEOUT;
u32 MAS__DIRECT_RFIELD_RX_TIMEOUT_VALUE;
u32 MAS__T0_SLOT_TIMER = 0;
//u32 MAS__T1_SLOT_TIMER;
u8 MAS__RXENABLE = 0;
u8 MAS__TXENABLE = 0;
/* FYR
#define T0RXENABLE MAS__RXENABLE=1
#define T1RXENABLE MAS__RXENABLE=1
#define T0TXENABLE MAS__TXENABLE=1
#define T1TXENABLE MAS__TXENABLE=1
#define T0RXENABLE_TIFS MAS__RXENABLE=2
#define T1RXENABLE_TIFS MAS__RXENABLE=2
#define T0TXENABLE_TIFS MAS__TXENABLE=2
#define T1TXENABLE_TIFS MAS__TXENABLE=2
*/

#define ___CLOCK__N__B__________________
#define ___CLOCK__N__B_________________
#define ___CLOCK__N__B________________


#define CLOCKS_PER_BT_CLOCK 3125
#define TOTAL_SIM_BT_CLOCKS 30
u32 g_sim_time_in_clock = TOTAL_SIM_BT_CLOCKS * CLOCKS_PER_BT_CLOCK;
u32 g_sim_time_curr = 0;
u32 g_master_clkn = 0;
u32 g_master_clkb = 0;
u32 g_is_prepare_rf = 0;
int Master_Phy_Wake_TIFS_0(SimAir_Info_t *info)
{
    return 0;
}

int Master_Phy_Wake_TIFS_1(SimAir_Info_t *info)
{
    return 0;
}

u32 Master_Phy_Wake_CLKB__RF_Controll__Tx_Buffer_Copy(void)
{
    //extern u8 g_master_rx_buf[];
    extern u8 g_master_tx_buf[];

    u32 buf_ctr = 0;
    u32 ll_pdu_len;

    //preamble
    g_master_tx_buf[buf_ctr] = 0x55; buf_ctr++;
    //Access Address
    g_master_tx_buf[buf_ctr] = MAS__ACCESS_CODE[0]; buf_ctr++;
    g_master_tx_buf[buf_ctr] = MAS__ACCESS_CODE[1]; buf_ctr++;
    g_master_tx_buf[buf_ctr] = MAS__ACCESS_CODE[2]; buf_ctr++;
    g_master_tx_buf[buf_ctr] = MAS__ACCESS_CODE[3]; buf_ctr++;
    //2bytes header
    g_master_tx_buf[buf_ctr] = MAS__TXLE_PLH_CP_RE[0]; buf_ctr++; MAS__TXLE_PLH_CP_RE++;
    ll_pdu_len = MAS__TXLE_PLH_CP_RE[0];
    g_master_tx_buf[buf_ctr] = MAS__TXLE_PLH_CP_RE[0]; buf_ctr++; MAS__TXLE_PLH_CP_RE++;
    //PDU
    for (u32 i = 0; i < ll_pdu_len; i++) {
        g_master_tx_buf[buf_ctr] = MAS__TXLE_PLD_CP_RE[0]; buf_ctr++; MAS__TXLE_PLD_CP_RE++;
    }
    //CRC
    g_master_tx_buf[buf_ctr] = MAS__LE_PHY_CRC24[0]; buf_ctr++;
    g_master_tx_buf[buf_ctr] = MAS__LE_PHY_CRC24[1]; buf_ctr++;
    g_master_tx_buf[buf_ctr] = MAS__LE_PHY_CRC24[2]; buf_ctr++;

    return buf_ctr;
}

void Master_Phy_Wake_CLKB__RF_Controll(void)
{
    if (g_master_clkb == MAS__T0_SLOT_TIMER) {
        MASTER_DUMP2(" PHY - PREPARE RF\n");
        g_is_prepare_rf = 1;
        return;
    }

    if (g_is_prepare_rf) {
        g_is_prepare_rf = 0; //prepare done
        MASTER_DUMP2(" PHY - START RF\n");

        g_master_info[SIM_AIR_TASK_0_TRX].requ_type = SIM_AIR_TX_REQUEST;
        u32 total_tx_bytes = Master_Phy_Wake_CLKB__RF_Controll__Tx_Buffer_Copy();
        g_master_info[SIM_AIR_TASK_0_TRX].clocks_total = total_tx_bytes * 8 * g_master_info[SIM_AIR_TASK_0_TRX].clocks_per_bit;
        g_master_info[SIM_AIR_TASK_0_TRX].freq = MAS__LE_HOP_CTRL0_Reg_HLbyte & 0x7F;
        SimAir_Request(&(g_master_info[SIM_AIR_TASK_0_TRX]));
    }
}

int Master_Phy_Wake_CLKN(SimAir_Info_t *info)
{
    g_master_clkn++;
    //printf("%s() -- %d, clkn:%d\n", __func__, info->response.ref_clock_L, g_master_clkn);
    //MASTER_DUMP("N[ MAS ][ CLK N ][%8d][%5d]\n", info->response.ref_clock_L, g_master_clkn);
    MASTER_DUMPn(" clkn:%d\n", g_master_clkn);

    BASIC_ASSERT(CLOCKS_PER_BT_CLOCK == SimAir_TimeStamp_Low_Get() - g_sim_time_curr)
    g_sim_time_curr += CLOCKS_PER_BT_CLOCK;
    if (g_sim_time_curr >= g_sim_time_in_clock) {
        g_is_simulation_times_up = 1;
    }

    if (g_is_simulation_times_up)
        return 0;

    g_master_info[SIM_AIR_TASK_CLKN].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info[SIM_AIR_TASK_CLKN].next_wake_up_time = CLOCKS_PER_BT_CLOCK;
    SimAir_Request(&(g_master_info[SIM_AIR_TASK_CLKN]));

    return 0;
}

int Master_Phy_Wake_CLKB(SimAir_Info_t *info)
{
    g_master_clkb++;
    //printf("%s() -- %d, clkb:%d\n", __func__, info->response.ref_clock_L, g_master_clkb);
    //MASTER_DUMP("B[ MAS ][ CLK B ][%8d][%5d]\n", info->response.ref_clock_L, g_master_clkb);
    MASTER_DUMPb(" clkb:%d\n", g_master_clkb);

    if (g_is_simulation_times_up)
        return 0;

    Master_Phy_Wake_CLKB__RF_Controll();

    g_master_info[SIM_AIR_TASK_CLKB].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info[SIM_AIR_TASK_CLKB].next_wake_up_time = CLOCKS_PER_BT_CLOCK;
    SimAir_Request(&(g_master_info[SIM_AIR_TASK_CLKB]));
    return 0;
}


#endif //#if 0