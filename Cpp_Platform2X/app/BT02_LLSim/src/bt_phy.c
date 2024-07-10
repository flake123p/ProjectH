
#include "Everything_App.hpp"

LibFileIoClass g_log_bt_phy = LibFileIoClass("log_bt_phy.txt", "w+b");
bool g_log_bt_phy_enable = false;

#define SIM_END_CLKN_NUMBER 120

int BT_Phy_Log_Enable(void)
{
    g_log_bt_phy_enable = true;
    return g_log_bt_phy.FileOpen();
}

int BT_Phy_Log_Disable(void)
{
    g_log_bt_phy_enable = false;
    return g_log_bt_phy.FileClose();
}

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
    LibVCD_ValueChange(info->upper_msg*2, 1);

    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;
    u32 total_tx_bytes = BT_Phy_Start_Tx__Buffer_Copy(info);

    phy_info->air_info[SIM_AIR_TASK_0_TRX].requ_type = SIM_AIR_TX_REQUEST;
    phy_info->air_info[SIM_AIR_TASK_0_TRX].clocks_total = total_tx_bytes * 8 * phy_info->air_info[SIM_AIR_TASK_0_TRX].clocks_per_bit;
    phy_info->air_info[SIM_AIR_TASK_0_TRX].freq = phy_info->LE_HOP_CTRL0_Reg_HLbyteZ & 0x7F;
    SimAir_Request(&(phy_info->air_info[SIM_AIR_TASK_0_TRX]));
}

void BT_Phy_Start_Rx(SimAir_Info_t *info)
{
    LibVCD_ValueChange((info->upper_msg*2)+1, 1);

    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    phy_info->rx_state = BT_PHY_RX_0_COMPARE_PREAMBLE;
    phy_info->rx_work_buf_ctr = 0;

    phy_info->air_info[SIM_AIR_TASK_0_TRX].requ_type = SIM_AIR_RX_REQUEST;
    phy_info->air_info[SIM_AIR_TASK_0_TRX].clocks_total = phy_info->DIRECT_RFIELD_RX_TIMEOUT_VALUEz * phy_info->air_info[SIM_AIR_TASK_0_TRX].clocks_per_bit;
    phy_info->air_info[SIM_AIR_TASK_0_TRX].freq = phy_info->LE_HOP_CTRL0_Reg_HLbyteZ & 0x7F;
    SimAir_Request(&(phy_info->air_info[SIM_AIR_TASK_0_TRX]));
}

void BT_Phy_Wake_CLKB__RF_Task_Check(SimAir_Info_t *info)
{
    u32 log_condition = 0;
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    do {
        if (phy_info->clkb == phy_info->T0_SLOT_TIMER && (phy_info->TXENABLE==1||phy_info->RXENABLE==1)) {
            phy_info->T0_SLOT_TIMER = 0;
            phy_info->is_prepare_rf = 1;
            log_condition = 1;
            break;
        }

        if (phy_info->is_prepare_rf) {
            phy_info->is_prepare_rf = 0; //prepare done

            if (phy_info->TXENABLE == 1)
            {
                BASIC_ASSERT(phy_info->RXENABLE != 1);
                phy_info->TXENABLE = 0;
                //TX
                BT_Phy_Start_Tx(info);
                log_condition = 2;
            }
            else if (phy_info->RXENABLE == 1)
            {
                BASIC_ASSERT(phy_info->TXENABLE != 1);
                phy_info->RXENABLE = 0;
                //RX
                BT_Phy_Start_Rx(info);
                log_condition = 3;
            }
        }
    } while (0);

    if (g_log_bt_phy_enable) {
        if (log_condition == 1) {
            fprintf(g_log_bt_phy.fp, "[%8d][CLKB_RF][%s] PREPARE RF, tx:%d, rx:%d\n", 
                info->response.ref_clock_L,
                SimAir_Handle_ID_String_Get(info->hdl),
                phy_info->TXENABLE,
                phy_info->RXENABLE);
        } else if (log_condition == 2) {
            fprintf(g_log_bt_phy.fp, "[%8d][CLKB_RF][%s] TX START, tx:%d, rx:%d\n", 
                info->response.ref_clock_L,
                SimAir_Handle_ID_String_Get(info->hdl),
                phy_info->TXENABLE,
                phy_info->RXENABLE);
        } else if (log_condition == 3) {
            fprintf(g_log_bt_phy.fp, "[%8d][CLKB_RF][%s] RX START, tx:%d, rx:%d\n", 
                info->response.ref_clock_L,
                SimAir_Handle_ID_String_Get(info->hdl),
                phy_info->TXENABLE,
                phy_info->RXENABLE);
        }
    }
}

int BT_Phy_Wake_CLKB(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    phy_info->clkb++;

    if (phy_info->sim_destroy) {
        (*(Void_CB_t)(info->upper_cb))();
        lc_conn_state_machine(phy_info->timer0_dev, LC_CONN_STT_EVT_SCH_ABORT);
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

int BT_Phy_Wake_CLKN(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    phy_info->clkn++;
    if (phy_info->clkn >= SIM_END_CLKN_NUMBER)
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

int BT_Phy_Wake_Scheduler(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    if (phy_info->sim_destroy)
        return 0;

    if (phy_info->scheduler0_enable == false)
        return 0;

    (*(Void_CB_t)(info->upper_cb))();
    lc_conn_state_machine(phy_info->scheduler0->dev, LC_CONN_STT_EVT_SCH_GRANT);

    phy_info->air_info[SIM_AIR_TASK_SCH_0].requ_type = SIM_AIR_WAKEUP_REQUEST;
    phy_info->air_info[SIM_AIR_TASK_SCH_0].next_wake_up_time = phy_info->scheduler0->periodical_interval_us * phy_info->air_info[SIM_AIR_TASK_SCH_0].clocks_per_bit;
    SimAir_Request(&(phy_info->air_info[SIM_AIR_TASK_SCH_0]));

    return 0;
}

int BT_Phy_Wake_Timer(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    (*(Void_CB_t)(info->upper_cb))();
    lc_conn_state_machine(phy_info->timer0_dev, LC_CONN_STT_EVT_SLEEP_TIMESUP);

    return 0;
}

int BT_Phy_Wake_TIFS_Timer(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    if (phy_info->RXENABLE == 2)
    {
        phy_info->RXENABLE = 0;
        BT_Phy_Start_Rx(info);
        BASIC_ASSERT(phy_info->TXENABLE != 2);
    }

    if (phy_info->TXENABLE == 2)
    {
        phy_info->TXENABLE = 0;
        BT_Phy_Start_Tx(info);
        BASIC_ASSERT(phy_info->RXENABLE != 2);
    }

    return 0;

}

int BT_Phy_Tx(SimAir_Info_t *info)
{
    BT_PHY_Info_t *phy_info = (BT_PHY_Info_t *)info->upper_hdl;

    if (info->response.resp_type == SIM_AIR_TX_DONE)
    {
        LibVCD_ValueChange((info->upper_msg*2)+0, 0);
        printf("LC_CONN_STT_EVT_TX_DONE\n");
        (*(Void_CB_t)(info->upper_cb))();
        lc_conn_state_machine(phy_info->rf_dev, LC_CONN_STT_EVT_TX_DONE);

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
        case SIM_AIR_RX_DONE:{
            LibVCD_ValueChange((info->upper_msg*2)+1, 0);
            if (phy_info->rx_state != BT_PHY_RX_4_COMPARE_CRC) {
                //RX timeout
                (*(Void_CB_t)(info->upper_cb))();
                lc_conn_state_machine(phy_info->rf_dev, LC_CONN_STT_EVT_RX_TIMEOUT);
                return 0;
            }
        } /* FALL THROUGH HERE !!!!! */
        case SIM_AIR_RXING: {
            u8 curr_rx_bit;
            u8 curr_rx_bit_adjusted;
            u32 index_h = (info->response.rx_done_bits-1) / 8;
            u32 index_l = (info->response.rx_done_bits-1) % 8;
            curr_rx_bit = info->rx_buf[index_h];
            curr_rx_bit = (curr_rx_bit >> index_l) & 0x01;
            switch (phy_info->rx_state) {
                case BT_PHY_RX_0_COMPARE_PREAMBLE: {
                    curr_rx_bit_adjusted = curr_rx_bit ? 0x80 : 0x00;
                    phy_info->rx_work_buf[0] = (phy_info->rx_work_buf[0] >> 1) | curr_rx_bit_adjusted;
                    phy_info->rx_work_buf_ctr++;
                    if (phy_info->rx_work_buf_ctr >= 8) {
                        if (phy_info->rx_work_buf[0] == 0x55) {
                            phy_info->rx_state = BT_PHY_RX_1_COMPARE_ACCESS_CODE;
                            phy_info->rx_work_buf_ctr = 0;
                        }
                    }
                } break;
                case BT_PHY_RX_1_COMPARE_ACCESS_CODE: {
                    index_h = phy_info->rx_work_buf_ctr / 8;
                    index_l = phy_info->rx_work_buf_ctr % 8;
                    if (index_l == 0)
                        phy_info->rx_work_buf[index_h] = 0;
                    curr_rx_bit_adjusted = curr_rx_bit << index_l;
                    phy_info->rx_work_buf[index_h] |= curr_rx_bit_adjusted;
                    phy_info->rx_work_buf_ctr++;
                    if (phy_info->rx_work_buf_ctr == 32) {
                        bool access_code_is_match = false;
                        if (phy_info->rx_work_buf[0] == phy_info->ACCESS_CODE[0])
                        if (phy_info->rx_work_buf[1] == phy_info->ACCESS_CODE[1])
                        if (phy_info->rx_work_buf[2] == phy_info->ACCESS_CODE[2])
                        if (phy_info->rx_work_buf[3] == phy_info->ACCESS_CODE[3])
                            access_code_is_match = true;

                        if (access_code_is_match) {
                            phy_info->rx_state = BT_PHY_RX_2_STORE_HEADER;
                            //interrupt : correlation ok
                            (*(Void_CB_t)(info->upper_cb))();
                            lc_conn_state_machine(phy_info->rf_dev, LC_CONN_STT_EVT_CORR_OK);
                        } else {
                            phy_info->rx_state = BT_PHY_RX_0_COMPARE_PREAMBLE;
                        }

                        phy_info->rx_work_buf_ctr = 0;
                    }
                } break;
                case BT_PHY_RX_2_STORE_HEADER: {
                    index_h = phy_info->rx_work_buf_ctr / 8;
                    index_l = phy_info->rx_work_buf_ctr % 8;
                    if (index_l == 0)
                        phy_info->RXLE_PLH_CP[index_h] = 0;
                    curr_rx_bit_adjusted = curr_rx_bit << index_l;
                    phy_info->RXLE_PLH_CP[index_h] |= curr_rx_bit_adjusted;
                    phy_info->rx_work_buf_ctr++;

                    if (phy_info->rx_work_buf_ctr == 16) {
                        u32 rest_len = phy_info->RXLE_PLH_CP[1] + 3/*CRC*/;
                        //adjust rx total time
                        info->clocks_total = info->response.clocks_elapsed + (rest_len * 8 * info->clocks_per_bit);

                        phy_info->rx_payload_len_in_bits = phy_info->RXLE_PLH_CP[1] * 8;

                        if (phy_info->rx_payload_len_in_bits != 0)
                            phy_info->rx_state = BT_PHY_RX_3_STORE_PAYLOAD;
                        else
                            phy_info->rx_state = BT_PHY_RX_4_COMPARE_CRC;
                        phy_info->rx_work_buf_ctr = 0;
                        phy_info->RXLE_PLH_CP += 2;
                    }
                } break;
                case BT_PHY_RX_3_STORE_PAYLOAD: {
                    index_h = phy_info->rx_work_buf_ctr / 8;
                    index_l = phy_info->rx_work_buf_ctr % 8;
                    if (index_l == 0)
                        phy_info->RXLE_PLD_CP[index_h] = 0;
                    curr_rx_bit_adjusted = curr_rx_bit << index_l;
                    phy_info->RXLE_PLD_CP[index_h] |= curr_rx_bit_adjusted;
                    phy_info->rx_work_buf_ctr++;

                    if (phy_info->rx_work_buf_ctr == phy_info->rx_payload_len_in_bits) {
                        phy_info->rx_state = BT_PHY_RX_4_COMPARE_CRC;
                        phy_info->rx_work_buf_ctr = 0;
                        phy_info->RXLE_PLD_CP += (phy_info->rx_payload_len_in_bits/8);
                    }
                } break;
                case BT_PHY_RX_4_COMPARE_CRC: {
                    index_h = phy_info->rx_work_buf_ctr / 8;
                    index_l = phy_info->rx_work_buf_ctr % 8;
                    if (index_l == 0)
                        phy_info->rx_work_buf[index_h] = 0;
                    curr_rx_bit_adjusted = curr_rx_bit << index_l;
                    phy_info->rx_work_buf[index_h] |= curr_rx_bit_adjusted;
                    phy_info->rx_work_buf_ctr++;
                    if (phy_info->rx_work_buf_ctr == 24) {
                        bool crc24_is_match = false;
                        if (phy_info->rx_work_buf[0] == phy_info->LE_PHY_CRC24[0])
                        if (phy_info->rx_work_buf[1] == phy_info->LE_PHY_CRC24[1])
                        if (phy_info->rx_work_buf[2] == phy_info->LE_PHY_CRC24[2])
                            crc24_is_match = true;

                        BASIC_ASSERT(info->response.resp_type == SIM_AIR_RX_DONE);

                        LC_CONNECTION_STATE_EVENT_t evt = LC_CONN_STT_EVT_PL_RCV_ERR;
                        if (crc24_is_match) {
                            evt = LC_CONN_STT_EVT_PL_RCV_OK;
                        }

                        (*(Void_CB_t)(info->upper_cb))();
                        lc_conn_state_machine(phy_info->rf_dev, evt);

                        if (crc24_is_match) {
                            //TIFS TX check
                            if (phy_info->TXENABLE == 2)
                            {
                                phy_info->air_info[SIM_AIR_TASK_TIFS_0].requ_type = SIM_AIR_WAKEUP_REQUEST;
                                phy_info->air_info[SIM_AIR_TASK_TIFS_0].next_wake_up_time = 150 * phy_info->air_info[SIM_AIR_TASK_TIFS_0].clocks_per_bit;
                                SimAir_Request(&(phy_info->air_info[SIM_AIR_TASK_TIFS_0]));
                            }
                        }
                    }
                    BASIC_ASSERT(phy_info->rx_work_buf_ctr <= 24);
                } break;
                default:
                    break;
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
    phy_info->scheduler0_enable = true;

    //DUMPD(g_sch_requ_mas_sch_00->periodical_interval_us);
    phy_info->scheduler0->sim_state = SIM_SCH_STT_WAITING_1ST_WAKUP_TO_GRANT;

    phy_info->air_info[SIM_AIR_TASK_SCH_0].requ_type = SIM_AIR_WAKEUP_REQUEST;
    phy_info->air_info[SIM_AIR_TASK_SCH_0].next_wake_up_time = sch_delay_in_us * phy_info->air_info[SIM_AIR_TASK_SCH_0].clocks_per_bit;
    SimAir_Request(&(phy_info->air_info[SIM_AIR_TASK_SCH_0]));
}

void BT_Phy_API_Sch_0_Remove_Request(BT_PHY_Info_t *phy_info)
{
    phy_info->scheduler0_enable = false;
}

