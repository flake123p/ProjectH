
#include "Everything_App.hpp"

int Slave_Phy_Wake(SimAir_Info_t *info)
{
    printf("%s() -- %d\n", __func__, info->response.ref_clock_L);

    return 0;
}

int Slave_Phy_Rx(SimAir_Info_t *info)
{
    printf("%s() -- %d, resp_type:%d, rx_done_bits:%d\n", __func__, info->response.ref_clock_L, info->response.resp_type, info->response.rx_done_bits);

    return 0;
}

int Slave_Phy_Tx(SimAir_Info_t *info)
{
    printf("%s() -- %d\n", __func__, info->response.ref_clock_L);

    return 0;
}

