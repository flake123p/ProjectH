
#include "Everything_App.hpp"

static SimAir_Handle_t g_slave_hdl;
static SimAir_Info_t g_slave_info;

static u8  g_slave_rx_buf[300];
static u8  g_slave_tx_buf[300];

extern int Slave_Phy_Wake(SimAir_Info_t *info);
extern int Slave_Phy_Rx(SimAir_Info_t *info);
extern int Slave_Phy_Tx(SimAir_Info_t *info);

void Slave_Upper_InitSimAir(void)
{
    //F.Y.R.: SimAir_Demo()

    g_slave_hdl = SimAir_Init_AddDescriptor(Slave_Phy_Wake, Slave_Phy_Rx, Slave_Phy_Tx, NULL);
    //init first wake up & set hdl into info
    g_slave_info.hdl = g_slave_hdl;
    g_slave_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_slave_info.next_wake_up_time = 10;
    //init basic air info, must complete
    g_slave_info.clocks_per_bit = 10;
    g_slave_info.clocks_to_interrupt_trx = 80;
    g_slave_info.clocks_total = 1;
    g_slave_info.freq = 0;
    g_slave_info.tx_buf = g_slave_tx_buf;
    g_slave_info.tx_buf_len_in_bits = 300 * 8;
    g_slave_info.rx_buf = g_slave_rx_buf;
    g_slave_info.rx_buf_max_size_in_bits = 300 * 8;

    SimAir_Request(&g_slave_info);

    return;
}

void Slave_Upper_InitTest(void)
{

}

