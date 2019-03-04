
#include "Everything_App.hpp"

Bt_Dev_Info_t *dev_init(BT_DEV_TYPE_t type, BT_DEV_ROLE_t role, u16 size_of_infrastructure)
{
    Bt_Dev_Info_t *dev = NULL;
    dev = (Bt_Dev_Info_t *)malloc(sizeof (Bt_Dev_Info_t));
    dev->Type = type;
    dev->Role = role;
    dev->infrastructure = malloc(size_of_infrastructure);
    return dev;
}

void Dump_Conn_Info(Bt_Dev_Info_t *dev)
{
    DUMPD(dev->Role);
    DUMPD(dev->Type);


    Conn_State_Info_t *info = (Conn_State_Info_t *)dev->infrastructure;
    DUMPD(info->tx_sn);
    DUMPD(info->tx_nesn);
    DUMPD(info->conn_hdl);
    DUMPD(info->state);

}

u32 scheduler_simulate_delay_in_us(void)
{
    srand (time(NULL));

    u32 randon_delay_in_us = 444;//rand() % 100 + 350;

    return randon_delay_in_us;
}
