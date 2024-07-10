
#include "Everything_App.hpp"

LL_Info_t g_ll_info_peer1 = {
    .dev_head = NULL,
    .dev_tail = NULL,
};
BT_PHY_Info_t g_phy_info_1 = {
    .sim_destroy = false,
    .clkn = 0,
    .clkb = 0,
};
static BT_PHY_Info_t *g_curr_phy_info = &g_phy_info_1;

SimAir_CB_Set_t g_peer1_cb_set[SIM_AIR_TASK_NUMBER] = {
/* SIM_AIR_TASK_CLKN     */ {BT_Phy_Wake_CLKN, NULL, NULL, "P1_CLKN"},
/* SIM_AIR_TASK_CLKB     */ {BT_Phy_Wake_CLKB, NULL, NULL, "P1_CLKB"},
/* SIM_AIR_TASK_0_TRX    */ {NULL, BT_Phy_Rx, BT_Phy_Tx, "P1_TxRx"},
/* SIM_AIR_TASK_1_TRX    */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_SCH_0    */ {BT_Phy_Wake_Scheduler, NULL, NULL, "P1_SCHE"},
/* SIM_AIR_TASK_SCH_1    */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_SCH_2    */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_SCH_3    */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_TIMER_0  */ {BT_Phy_Wake_Timer, NULL, NULL, "P1_TIMR"},
/* SIM_AIR_TASK_TIMER_1  */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_TIMER_2  */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_TIMER_3  */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_TIFS_0   */ {BT_Phy_Wake_TIFS_Timer, NULL, NULL, "P1_TIFS"},
/* SIM_AIR_TASK_TIFS_1   */ {NULL, NULL, NULL, NULL},
};

void Peer1_ChangeLLInfo(void)
{
    lc_conn_state_ll_info_set(&g_ll_info_peer1);
}

void Peer1_InitTemplateInfo(SimAir_Info_t *info, SimAir_Handle_t new_sim_air_hdl)
{
    //init first wake up & set hdl into info
    info->hdl = new_sim_air_hdl;
    info->requ_type = SIM_AIR_INVALID_REQUEST;
    info->next_wake_up_time = 0;
    //init basic air info, must complete
    info->clocks_per_bit = 10;
    info->clocks_to_interrupt_trx = 10; //every bit
    info->clocks_total = 1;
    info->freq = 0;
    info->tx_buf = g_curr_phy_info->phy_tx_buf;
    info->tx_buf_len_in_bits = PHY_TR_BUF_SIZE * 8;
    info->rx_buf = g_curr_phy_info->phy_rx_buf;
    info->rx_buf_max_size_in_bits = PHY_TR_BUF_SIZE * 8;

    info->upper_msg = 1; //peer "1"
    info->upper_hdl = (void *)g_curr_phy_info;
    info->upper_cb = (void *)Peer1_ChangeLLInfo;
}

void Peer1_InitSimAir(void)
{
    SimAir_Handle_t curr_hdl;
    //F.Y.R.: SimAir_Demo()
    for (u32 i=0; i<SIM_AIR_TASK_NUMBER; i++)
    {
        curr_hdl = SimAir_Init_AddDescriptor2(g_peer1_cb_set + i);
        Peer1_InitTemplateInfo(&(g_curr_phy_info->air_info[i]) , curr_hdl);
    }

    g_curr_phy_info->air_info[SIM_AIR_TASK_CLKN].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_curr_phy_info->air_info[SIM_AIR_TASK_CLKN].next_wake_up_time = 3125;
    SimAir_Request(&(g_curr_phy_info->air_info[SIM_AIR_TASK_CLKN]));

    g_curr_phy_info->air_info[SIM_AIR_TASK_CLKB].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_curr_phy_info->air_info[SIM_AIR_TASK_CLKB].next_wake_up_time = 3125;
    SimAir_Request(&(g_curr_phy_info->air_info[SIM_AIR_TASK_CLKB]));

    g_ll_info_peer1.phy_info = (void *)&g_phy_info_1;
    g_ll_info_peer1.vcdIndex = 2;
    Peer1_ChangeLLInfo();//lc_conn_state_ll_info_set(&g_ll_info_peer1);
    lc_conn_state_init();

    return;
}

extern Adv_Connect_Ind_Payload_t gTestConnInd;
void Peer1_StartTest(void)
{
/*
    gTestConnInd.LLData.WinSize = 1;
    gTestConnInd.LLData.WinOffset = 0;
    gTestConnInd.LLData.AA[0] = 0x11;
    gTestConnInd.LLData.AA[1] = 0x22;
    gTestConnInd.LLData.AA[2] = 0x33;
    gTestConnInd.LLData.AA[3] = 0x45;

    gTestConnInd.LLData.CRCInit[0] = 0x9A;
    gTestConnInd.LLData.CRCInit[1] = 0x88;
    gTestConnInd.LLData.CRCInit[2] = 0x77;

    gTestConnInd.LLData.Hop = 7;
    gTestConnInd.LLData.Interval = 2;
    SLAVE_DUMP2(" <<< TEST INFO: >>>\n");
    SLAVE_DUMP2(" <<< WinSize   = %d >>>\n", gTestConnInd.LLData.WinSize);
    SLAVE_DUMP2(" <<< WinOffset = %d >>>\n", gTestConnInd.LLData.WinOffset);
    SLAVE_DUMP2(" <<< Hop       = %d >>>\n", gTestConnInd.LLData.Hop);
    SLAVE_DUMP2(" <<< Interval  = %d >>>\n", gTestConnInd.LLData.Interval);
*/
    Bt_Dev_Info_t temp_dev;
    temp_dev.infrastructure = (void *)&gTestConnInd;

    Peer1_ChangeLLInfo();//lc_conn_state_ll_info_set(&g_ll_info_peer0);
    lc_sla_handle_conn_ind(&temp_dev);
}

