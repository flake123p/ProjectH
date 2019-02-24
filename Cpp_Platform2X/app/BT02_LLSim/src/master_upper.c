#if 0



#include "Everything_App.hpp"

SimAir_Handle_t g_master_hdl[SIM_AIR_TASK_NUMBER];
SimAir_Info_t g_master_info[SIM_AIR_TASK_NUMBER];

#define TR_BUF_SIZE (500)
u8 g_master_rx_buf[TR_BUF_SIZE];
u8 g_master_tx_buf[TR_BUF_SIZE];

extern int Master_Phy_Wake_CLKN(SimAir_Info_t *info);
extern int Master_Phy_Wake_CLKB(SimAir_Info_t *info);
extern int Master_Phy_Wake_TRx0(SimAir_Info_t *info);
extern int Master_Phy_Rx_0(SimAir_Info_t *info);
extern int Master_Phy_Tx_0(SimAir_Info_t *info);
extern int Master_Phy_Wake_Timer0(SimAir_Info_t *info);
extern int Master_Phy_Wake_Sch0(SimAir_Info_t *info);

extern int Master_Phy_Wake_TIFS_0(SimAir_Info_t *info);
extern int Master_Phy_Wake_TIFS_1(SimAir_Info_t *info);


SimAir_CB_Set_t g_master_cb_set[SIM_AIR_TASK_NUMBER] = {
/* SIM_AIR_TASK_CLKN     */ {Master_Phy_Wake_CLKN, NULL, NULL, NULL},
/* SIM_AIR_TASK_CLKB     */ {Master_Phy_Wake_CLKB, NULL, NULL, NULL},
/* SIM_AIR_TASK_0_TRX    */ {Master_Phy_Wake_TRx0, Master_Phy_Rx_0, Master_Phy_Tx_0, NULL},
/* SIM_AIR_TASK_1_TRX    */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_SCH_0    */ {Master_Phy_Wake_Sch0, NULL, NULL, NULL},
/* SIM_AIR_TASK_SCH_1    */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_SCH_2    */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_SCH_3    */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_TIMER_0  */ {Master_Phy_Wake_Timer0, NULL, NULL, NULL},
/* SIM_AIR_TASK_TIMER_1  */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_TIMER_2  */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_TIMER_3  */ {NULL, NULL, NULL, NULL},
/* SIM_AIR_TASK_TIFS_0   */ {Master_Phy_Wake_TIFS_0, NULL, NULL, NULL},
/* SIM_AIR_TASK_TIFS_1   */ {Master_Phy_Wake_TIFS_1, NULL, NULL, NULL},
};





void Master_Upper_InitTemplateInfo(SimAir_Info_t *info, SimAir_Handle_t new_sim_air_hdl)
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
    info->tx_buf = g_master_tx_buf;
    info->tx_buf_len_in_bits = TR_BUF_SIZE * 8;
    info->rx_buf = g_master_rx_buf;
    info->rx_buf_max_size_in_bits = TR_BUF_SIZE * 8;
}

void Master_Upper_InitSimAir(void)
{
    //F.Y.R.: SimAir_Demo()
    for (u32 i=0; i<SIM_AIR_TASK_NUMBER; i++)
    {
        g_master_hdl[i] = SimAir_Init_AddDescriptor2(g_master_cb_set + i);
        Master_Upper_InitTemplateInfo(&(g_master_info[i]), g_master_hdl[i]);
    }

    g_master_info[SIM_AIR_TASK_CLKN].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info[SIM_AIR_TASK_CLKN].next_wake_up_time = 3125;
    SimAir_Request(&(g_master_info[SIM_AIR_TASK_CLKN]));

    g_master_info[SIM_AIR_TASK_CLKB].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info[SIM_AIR_TASK_CLKB].next_wake_up_time = 3125;
    SimAir_Request(&(g_master_info[SIM_AIR_TASK_CLKB]));

    return;
}

Adv_Connect_Ind_Payload_t gTestConnIndXX;
void Master_Upper_InitTest(void)
{
    gTestConnIndXX.LLData.WinSize = 1;
    gTestConnIndXX.LLData.WinOffset = 0;
    gTestConnIndXX.LLData.AA[0] = 0x11;
    gTestConnIndXX.LLData.AA[1] = 0x22;
    gTestConnIndXX.LLData.AA[2] = 0x33;
    gTestConnIndXX.LLData.AA[3] = 0x44;

    gTestConnIndXX.LLData.CRCInit[0] = 0x99;
    gTestConnIndXX.LLData.CRCInit[1] = 0x88;
    gTestConnIndXX.LLData.CRCInit[2] = 0x77;

    gTestConnIndXX.LLData.Hop = 5;
    gTestConnIndXX.LLData.Interval = 2;
    MASTER_DUMP2(" <<< TEST INFO: >>>\n");
    MASTER_DUMP2(" <<< WinSize   = %d >>>\n", gTestConnIndXX.LLData.WinSize);
    MASTER_DUMP2(" <<< WinOffset = %d >>>\n", gTestConnIndXX.LLData.WinOffset);
    MASTER_DUMP2(" <<< Hop       = %d >>>\n", gTestConnIndXX.LLData.Hop);
    MASTER_DUMP2(" <<< Interval  = %d >>>\n", gTestConnIndXX.LLData.Interval);

    Bt_Dev_Info_t temp_dev;
    temp_dev.infrastructure = (void *)&gTestConnIndXX;

    lc_mas_handle_conn_indXX(&temp_dev);
}


#endif //#if 0