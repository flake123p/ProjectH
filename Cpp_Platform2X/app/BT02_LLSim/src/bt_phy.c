
#include "Everything_App.hpp"

u32 BT_Phy_Start_Tx__Buffer_Copy(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    u32 buf_ctr = 0;
    u32 ll_pdu_len;

    //preamble
    info->tx_buf[buf_ctr] = 0x55; buf_ctr++;
    //Access Address
    info->tx_buf[buf_ctr] = phy_info->ACCESS_CODE[0]; buf_ctr++;
    info->tx_buf[buf_ctr] = phy_info->ACCESS_CODE[1]; buf_ctr++;
    info->tx_buf[buf_ctr] = phy_info->ACCESS_CODE[2]; buf_ctr++;
    info->tx_buf[buf_ctr] = phy_info->ACCESS_CODE[3]; buf_ctr++;
    //2bytes header
    info->tx_buf[buf_ctr] = phy_info->TXLE_PLH_CP_RE[0]; buf_ctr++; phy_info->TXLE_PLH_CP_RE++;
    ll_pdu_len = phy_info->TXLE_PLH_CP_RE[0];
    info->tx_buf[buf_ctr] = phy_info->TXLE_PLH_CP_RE[0]; buf_ctr++; phy_info->TXLE_PLH_CP_RE++;
    //PDU
    for (u32 i = 0; i < ll_pdu_len; i++) {
        info->tx_buf[buf_ctr] = phy_info->TXLE_PLD_CP_RE[0]; buf_ctr++; phy_info->TXLE_PLD_CP_RE++;
    }
    //CRC
    info->tx_buf[buf_ctr] = phy_info->LE_PHY_CRC24[0]; buf_ctr++;
    info->tx_buf[buf_ctr] = phy_info->LE_PHY_CRC24[1]; buf_ctr++;
    info->tx_buf[buf_ctr] = phy_info->LE_PHY_CRC24[2]; buf_ctr++;

    return buf_ctr;
}

void BT_Phy_Start_Tx(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;
    u32 total_tx_bytes = BT_Phy_Start_Tx__Buffer_Copy(info);

    phy_info->air_info[SIM_AIR_TASK_0_TRX].requ_type = SIM_AIR_TX_REQUEST;
    phy_info->air_info[SIM_AIR_TASK_0_TRX].clocks_total = total_tx_bytes * 8 * phy_info->air_info[SIM_AIR_TASK_0_TRX].clocks_per_bit;
    phy_info->air_info[SIM_AIR_TASK_0_TRX].freq = phy_info->LE_HOP_CTRL0_Reg_HLbyteZ & 0x7F;
    SimAir_Request(&(phy_info->air_info[SIM_AIR_TASK_0_TRX]));
}

void BT_Phy_Start_Rx(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    phy_info->rx_state = BT_PHY_RX_COMPARE_PREAMBLE;

    phy_info->air_info[SIM_AIR_TASK_0_TRX].requ_type = SIM_AIR_RX_REQUEST;
    phy_info->air_info[SIM_AIR_TASK_0_TRX].clocks_total = phy_info->DIRECT_RFIELD_RX_TIMEOUT_VALUEz * phy_info->air_info[SIM_AIR_TASK_0_TRX].clocks_per_bit;
    phy_info->air_info[SIM_AIR_TASK_0_TRX].freq = phy_info->LE_HOP_CTRL0_Reg_HLbyteZ & 0x7F;
    SimAir_Request(&(phy_info->air_info[SIM_AIR_TASK_0_TRX]));
}


int BT_Phy_Wake_CLKN(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    phy_info->clkn++;
    if (phy_info->clkn >= 40)
        phy_info->sim_destroy = true;

    if (phy_info->sim_destroy) {
        return 0;
    }
    else {
        info->requ_type = SIM_AIR_WAKEUP_REQUEST;
        info->next_wake_up_time = 3125;
        SimAir_Request(info);
    }
    return 0;
}

void BT_Phy_Wake_CLKB__RF_Task_Check(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    if (phy_info->clkb == phy_info->T0_SLOT_TIMER && (phy_info->TXENABLE==1||phy_info->RXENABLE==1)) {
        MASTER_DUMP2(" common PHY - PREPARE RF\n");
        phy_info->is_prepare_rf = 1;
        return;
    }

    if (phy_info->is_prepare_rf) {
        phy_info->is_prepare_rf = 0; //prepare done

        if (phy_info->TXENABLE == 1)
        {
            BASIC_ASSERT(phy_info->RXENABLE != 1);
            phy_info->TXENABLE = 0;
            //TX
            MASTER_DUMP2(" common PHY - START RF - TX\n");

            BT_Phy_Start_Tx(info);
        }
        else if (phy_info->RXENABLE == 1)
        {
            BASIC_ASSERT(phy_info->TXENABLE != 1);
            phy_info->RXENABLE = 0;
            //RX
            MASTER_DUMP2(" common PHY - START RF - rX\n");
        }
    }
}

int BT_Phy_Wake_CLKB(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    phy_info->clkb++;

    if (phy_info->sim_destroy) {
        return 0;
    }
    else {
        BT_Phy_Wake_CLKB__RF_Task_Check(info);
        
        info->requ_type = SIM_AIR_WAKEUP_REQUEST;
        info->next_wake_up_time = 3125;
        SimAir_Request(info);
    }
    return 0;
}


int BT_Phy_Wake_Scheduler(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    if (phy_info->sim_destroy)
        return 0;

    if (phy_info->timer0_dev->Role == LE_MASTER) {
        extern void lc_mas_conn_state_machine(Bt_Dev_Info_t *mas_dev, LC_CONNECTION_STATE_EVENT_t evt);
        lc_mas_conn_state_machine(phy_info->scheduler0->dev , LC_CONN_STT_EVT_SCH_GRANT);
    }
    else {
        //todo:slave state machine
    }

    phy_info->air_info[SIM_AIR_TASK_SCH_0].requ_type = SIM_AIR_WAKEUP_REQUEST;
    phy_info->air_info[SIM_AIR_TASK_SCH_0].next_wake_up_time = phy_info->scheduler0->periodical_interval_us * phy_info->air_info[SIM_AIR_TASK_SCH_0].clocks_per_bit;
    SimAir_Request(&(phy_info->air_info[SIM_AIR_TASK_SCH_0]));

    return 0;
}

int BT_Phy_Wake_Timer(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    if (phy_info->timer0_dev->Role == LE_MASTER) {
        extern void lc_mas_conn_state_machine(Bt_Dev_Info_t *mas_dev, LC_CONNECTION_STATE_EVENT_t evt);
        lc_mas_conn_state_machine(phy_info->timer0_dev, LC_CONN_STT_EVT_SLEEP_TIMESUP);
    }
    else {
        //todo:slave state machine
    }

    return 0;
}

int BT_Phy_Wake_TIFS_Timer(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    if (phy_info->RXENABLE == 2)
    {
        phy_info->RXENABLE = 0;
        BT_Phy_Start_Rx(info);
    }
    else if (phy_info->TXENABLE == 2)
    {
        phy_info->TXENABLE = 0;
        BT_Phy_Start_Tx(info);
    }

    return 0;

}

int BT_Phy_Tx(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    if (info->response.resp_type == SIM_AIR_TX_DONE)
    {
        //TIFS RX check
        if (phy_info->RXENABLE == 2)
        {
            phy_info->air_info[SIM_AIR_TASK_TIFS_0].requ_type = SIM_AIR_WAKEUP_REQUEST;
            phy_info->air_info[SIM_AIR_TASK_TIFS_0].next_wake_up_time = 150 * phy_info->air_info[SIM_AIR_TASK_TIFS_0].clocks_per_bit;
            SimAir_Request(&(phy_info->air_info[SIM_AIR_TASK_TIFS_0]));
        }
    }

    return 0;
}

int BT_Phy_Rx(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    switch (info->response.resp_type)
    {
        case SIM_AIR_RXING: {
        } break;

        case SIM_AIR_RX_DONE: {
            if (phy_info->rx_state == BT_PHY_RX_COMPARE_PREAMBLE) {
                //RX timeout
                if (phy_info->timer0_dev->Role == LE_MASTER) {
                    extern void lc_mas_conn_state_machine(Bt_Dev_Info_t *mas_dev, LC_CONNECTION_STATE_EVENT_t evt);
                    lc_mas_conn_state_machine(phy_info->timer0_dev, LC_CONN_STT_EVT_RX_TIMEOUT);
                }
                else {
                    //todo:slave state machine
                }
            } else {
                BASIC_ASSERT(0);
            }
        } break;

        default:
            BASIC_ASSERT(0);
            break;
    }

    return 0;
}

void BT_Phy_API_Timer_0(BT_PHY_Info_t *phy_info, Bt_Dev_Info_t *dev, u32 sleep_time_in_us)
{
    phy_info->timer0_dev = dev;
    phy_info->timer0_sleep_time_in_us = sleep_time_in_us;
    phy_info->air_info[SIM_AIR_TASK_TIMER_0].requ_type = SIM_AIR_WAKEUP_REQUEST;

    phy_info->air_info[SIM_AIR_TASK_TIMER_0].next_wake_up_time = sleep_time_in_us * phy_info->air_info[SIM_AIR_TASK_TIMER_0].clocks_per_bit;
    SimAir_Request(&(phy_info->air_info[SIM_AIR_TASK_TIMER_0]));
}

void BT_Phy_API_Sch_0_Add_Request(BT_PHY_Info_t *phy_info, Scheduler_Request_T *p_sch_requ, u32 sch_delay_in_us)
{
    //u32 sch_delay_in_us = 444;//scheduler_simulate_delay_in_us();
    phy_info->scheduler0 = p_sch_requ;

    //DUMPD(g_sch_requ_mas_sch_00->periodical_interval_us);
    phy_info->scheduler0->sim_state = SIM_SCH_STT_WAITING_1ST_WAKUP_TO_GRANT;

    phy_info->air_info[SIM_AIR_TASK_SCH_0].requ_type = SIM_AIR_WAKEUP_REQUEST;
    phy_info->air_info[SIM_AIR_TASK_SCH_0].next_wake_up_time = sch_delay_in_us * phy_info->air_info[SIM_AIR_TASK_SCH_0].clocks_per_bit;
    SimAir_Request(&(phy_info->air_info[SIM_AIR_TASK_SCH_0]));
}

