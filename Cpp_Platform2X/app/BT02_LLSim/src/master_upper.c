
#include "Everything_App.hpp"

static SimAir_Handle_t g_master_hdl;
static SimAir_Info_t g_master_info;

static u8  g_master_rx_buf[300];
static u8  g_master_tx_buf[300];

extern int Master_Phy_Wake(SimAir_Info_t *info);
extern int Master_Phy_Rx(SimAir_Info_t *info);
extern int Master_Phy_Tx(SimAir_Info_t *info);

void Master_Upper_InitSimAir(void)
{
    //F.Y.R.: SimAir_Demo()

    g_master_hdl = SimAir_Init_AddDescriptor(Master_Phy_Wake, Master_Phy_Rx, Master_Phy_Tx);
    //init first wake up & set hdl into info
    g_master_info.hdl = g_master_hdl;
    g_master_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info.next_wake_up_time = 10;
    //init basic air info, must complete
    g_master_info.clocks_per_bit = 10;
    g_master_info.clocks_to_interrupt_trx = 80;
    g_master_info.clocks_total = 1;
    g_master_info.freq = 0;
    g_master_info.tx_buf = g_master_tx_buf;
    g_master_info.tx_buf_len_in_bits = 300 * 8;
    g_master_info.rx_buf = g_master_rx_buf;
    g_master_info.rx_buf_max_size_in_bits = 300 * 8;

    SimAir_Request(&g_master_info);

    return;
}


