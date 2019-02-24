#ifdef DFS_SIM_ON
#include "Everything_App.hpp"
#else //#ifdef DFS_SIM_ON

#endif //#ifdef DFS_SIM_ON

LL_Info_t *g_ll_info;

void lc_conn_state_ll_info_set(LL_Info_t *ll_info)
{
    g_ll_info = ll_info;
}

void lc_conn_state_init(void)
{
#ifdef DFS_SIM_ON
    #define pvPortMalloc_ext malloc
#else
    extern void *pvPortMalloc_ext(u32 xWantedSize);
#endif
    if (g_ll_info->tx_hdr_buf  == NULL)
        g_ll_info->tx_hdr_buf = (u8 *)pvPortMalloc_ext(4);

    if (g_ll_info->tx_pld_buf == NULL)
        g_ll_info->tx_pld_buf = (u8 *)pvPortMalloc_ext(4); //dummy, useless, because tx buffer is really in tx request descriptor

    if (g_ll_info->rx_hdr_buf == NULL)
        g_ll_info->rx_hdr_buf = (u8 *)pvPortMalloc_ext(10);

    if (g_ll_info->rx_pld_buf == NULL)
        g_ll_info->rx_pld_buf = (u8 *)pvPortMalloc_ext(4096);
}

static u16 lc_conn_state_calculate_new_conn_handle(void)
{
    static u16 new_conn_hdl = 0x70;
    Bt_Dev_Info_t *curr_dev = g_ll_info->dev_head;

    new_conn_hdl = new_conn_hdl >= 0x0FFF ? 0 : 1+new_conn_hdl; //so first conn hdl is 0x71
    while(curr_dev != NULL)
    {
        Conn_State_Info_t *curr_conn_info = (Conn_State_Info_t *)curr_dev->infrastructure;
        if (new_conn_hdl == curr_conn_info->conn_hdl) {
            new_conn_hdl = new_conn_hdl >= 0x0FFF ? 0 : 1+new_conn_hdl;
            curr_dev = g_ll_info->dev_head; //start over while loop
        }
        else {
            curr_dev = curr_conn_info->next_dev;
        }
    }
    return new_conn_hdl;
}

static void lc_conn_state_push_dev_to_list_tail(Bt_Dev_Info_t *dev)
{
    if (g_ll_info->dev_head == NULL)
    {
        g_ll_info->dev_head = dev;
        g_ll_info->dev_tail = dev;
    }
    else
    {
        Conn_State_Info_t *old_tail_conn_info = (Conn_State_Info_t *)g_ll_info->dev_tail->infrastructure;
        Conn_State_Info_t *curr_conn_info = (Conn_State_Info_t *)dev->infrastructure;

        old_tail_conn_info->next_dev = dev;
        curr_conn_info->prev_dev = g_ll_info->dev_tail;

        g_ll_info->dev_tail = dev;
    }
}

static void lc_conn_state_dump_all_dev(void)
{
#ifdef DFS_SIM_ON
    u32 i = 0;
    Bt_Dev_Info_t *curr_dev = g_ll_info->dev_head;
    while(curr_dev != NULL)
    {
        Conn_State_Info_t *curr_conn_info = (Conn_State_Info_t *)curr_dev->infrastructure;
        MAS_INT_DUMP2(" -- [[ conn state dev dump %d ]]\n", i);
        MAS_INT_DUMP2(" -- Type:%d, Role:%d, Up_Conn_Hdl:0x%X\n", curr_dev->Type, curr_dev->Role, curr_dev->connection_handle);
        MAS_INT_DUMP2(" -- -- state:%d, llid:%d, sn:%d, nesn:%d, md:%d\n", curr_conn_info->state, curr_conn_info->llid, curr_conn_info->sn, curr_conn_info->nesn, curr_conn_info->md);
        MAS_INT_DUMP2(" -- -- tx_len:%d, last_rx_nesn_is_match:%d, last_tx_is_acked:%d, accu_tx_len:%d\n", curr_conn_info->tx_len, curr_conn_info->last_rx_nesn_is_match, curr_conn_info->last_tx_is_acked, curr_conn_info->accu_tx_len);
        MAS_INT_DUMP2(" -- -- tx_ctr:%d, rx_ctr:%d, conn_hdl:0x%X\n", curr_conn_info->tx_ctr, curr_conn_info->rx_ctr, curr_conn_info->conn_hdl);
        MAS_INT_DUMP2(" -- -- tx_max_len:%d, tx_md_enable:%d, rx_md_enable:%d, tx_hold:%d, rx_hold:%d\n", curr_conn_info->tx_max_len, curr_conn_info->tx_md_enable, curr_conn_info->rx_md_enable, curr_conn_info->tx_hold, curr_conn_info->rx_hold);
        MAS_INT_DUMP2(" -- -- InitA:0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", \
                      curr_conn_info->conn_ind_payload.InitA[0], \
                      curr_conn_info->conn_ind_payload.InitA[1], \
                      curr_conn_info->conn_ind_payload.InitA[2], \
                      curr_conn_info->conn_ind_payload.InitA[3], \
                      curr_conn_info->conn_ind_payload.InitA[4], \
                      curr_conn_info->conn_ind_payload.InitA[5]);
        MAS_INT_DUMP2(" -- -- AdvA :0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", \
                      curr_conn_info->conn_ind_payload.AdvA[0], \
                      curr_conn_info->conn_ind_payload.AdvA[1], \
                      curr_conn_info->conn_ind_payload.AdvA[2], \
                      curr_conn_info->conn_ind_payload.AdvA[3], \
                      curr_conn_info->conn_ind_payload.AdvA[4], \
                      curr_conn_info->conn_ind_payload.AdvA[5]);
        MAS_INT_DUMP2(" -- -- LLData.AA :0x%X, 0x%X, 0x%X, 0x%X\n", \
                      curr_conn_info->conn_ind_payload.LLData.AA[0], \
                      curr_conn_info->conn_ind_payload.LLData.AA[1], \
                      curr_conn_info->conn_ind_payload.LLData.AA[2], \
                      curr_conn_info->conn_ind_payload.LLData.AA[3]);
        MAS_INT_DUMP2(" -- -- LLData.CRCInit :0x%X, 0x%X, 0x%X\n", \
                      curr_conn_info->conn_ind_payload.LLData.CRCInit[0], \
                      curr_conn_info->conn_ind_payload.LLData.CRCInit[1], \
                      curr_conn_info->conn_ind_payload.LLData.CRCInit[2]);
        MAS_INT_DUMP2(" -- -- LLData.WinSize:%d  WinOffset:%d  Interval:%d  Latency:%d  Timeout:%d\n", \
                      curr_conn_info->conn_ind_payload.LLData.WinSize, \
                      curr_conn_info->conn_ind_payload.LLData.WinOffset, \
                      curr_conn_info->conn_ind_payload.LLData.Interval, \
                      curr_conn_info->conn_ind_payload.LLData.Latency, \
                      curr_conn_info->conn_ind_payload.LLData.Timeout);
        MAS_INT_DUMP2(" -- -- LLData.ChM :0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", \
                      curr_conn_info->conn_ind_payload.LLData.ChM[0], \
                      curr_conn_info->conn_ind_payload.LLData.ChM[1], \
                      curr_conn_info->conn_ind_payload.LLData.ChM[2], \
                      curr_conn_info->conn_ind_payload.LLData.ChM[3], \
                      curr_conn_info->conn_ind_payload.LLData.ChM[4]);
        MAS_INT_DUMP2(" -- -- LLData.Hop:%d  SCA:%d\n", \
                      curr_conn_info->conn_ind_payload.LLData.Hop, \
                      curr_conn_info->conn_ind_payload.LLData.SCA);

        u32 j = 0;
        MAS_INT_DUMP2(" -- -- -- [[ tx_request dump start : %d ]]\n", j);
        Conn_State_Tx_Request_t *curr_tx_request = curr_conn_info->lm_tx_request;
        while (curr_tx_request != NULL) {
            MAS_INT_DUMP2(" -- -- -- tx_request_active:%d  tx_request_done:%d  tx_len:%d\n",
                          curr_tx_request->tx_request_active, \
                          curr_tx_request->tx_request_done, \
                          curr_tx_request->tx_len);
            curr_tx_request = curr_tx_request->next;
            if (curr_tx_request != NULL) {
                j++;
                MAS_INT_DUMP2(" -- -- -- [[ tx_request dump start : %d ]]\n", j);
            }
        }

        curr_dev = curr_conn_info->next_dev;
        i++;
    }
#endif
}

static u32 lc_conn_state_calculate_window_widen_size_in_us(Conn_State_Info_t *conn_info)
{
    u32 window_widen_max;
    u32 window_widen_size = 0;
    u32 sca_in_ppm = 0;

    if (conn_info->role == LC_CONN_ROLE_MASTER)
    {
        return 0;
    }

    switch(conn_info->conn_ind_payload.LLData.SCA)
    {
        case 0: sca_in_ppm=500; break; /*251~500*/
        case 1: sca_in_ppm=250; break; /*151~250*/
        case 2: sca_in_ppm=150; break; /*101~150*/
        case 3: sca_in_ppm=100; break; /* 76~100*/
        case 4: sca_in_ppm=75; break;  /* 51~ 75*/
        case 5: sca_in_ppm=50; break;  /* 31~ 50*/
        case 6: sca_in_ppm=30; break;  /* 21~ 30*/
        case 7: sca_in_ppm=20; break;  /*  0~ 20*/
        default: break;
    }

    window_widen_size = conn_info->conn_ind_payload.LLData.Interval * 1250 * (/*masterSCA*/sca_in_ppm + /*slaveSCA*/500);
    if (window_widen_size % 1000000)
    {
        window_widen_size = (window_widen_size / 1000000) + 1;
    }
    else
    {
        window_widen_size = window_widen_size / 1000000;
    }

    //Useless???
    window_widen_max = (conn_info->conn_ind_payload.LLData.Interval * 1250 / 2) - /*T_IFS*/150;
    if (window_widen_size >= window_widen_max)
    {
        window_widen_size = window_widen_max - 1;
    }

    return window_widen_size;
}

static u8 lc_conn_state_create_dev_by_conn_ind(BT_DEV_ROLE_t role, Adv_Connect_Ind_Payload_t *in_conn_ind, Bt_Dev_Info_t **out_new_dev)
{
    Bt_Dev_Info_t   *new_dev = NULL;

    new_dev = (Bt_Dev_Info_t *)dev_init(DEVICE_TYPE_BLE, role, (u16)(sizeof(Conn_State_Info_t)));

    //If the Advertising_Handle does not identify an existing advertising set and the
    //Controller is unable to support a new advertising set at present, the Controller
    //shall return the error code Memory Capacity Exceeded (0x07).
    if(new_dev == NULL)
    {
        return  ERROR_CODES_Memory_Capacity_Exceeded;
    }

    Conn_State_Info_t *new_conn_info = (Conn_State_Info_t *)new_dev->infrastructure;

    //copy to new dev
    new_conn_info->conn_ind_payload = *in_conn_ind;

    //20180116_IJ: WBNF-63 transmitWindowSize shall be at least 1.25ms
    if(new_conn_info->conn_ind_payload.LLData.WinSize == 0x00)
    {
       new_conn_info->conn_ind_payload.LLData.WinSize = 0x01;//at least 1.25ms
    }

    if (role == LE_MASTER)
        new_conn_info->role = LC_CONN_ROLE_MASTER;
    else if (role == LE_SLAVE)
        new_conn_info->role = LC_CONN_ROLE_SLAVE;

    new_conn_info->state = LC_CONN_STT_ESTABLISHED;

    //new_conn_info->llid = 1; //01b = LL Data PDU: Continuation fragment of an L2CAP message, or an Empty PDU
    new_conn_info->llid = 0; //dummy
    new_conn_info->sn = 1; //the first sn will be 0, bacause last_tx_is_acked is init to 1, sn will then turn to 0.
    new_conn_info->nesn = 1; //the first sn will be 0, bacause last_rx_nesn_is_match is init to 1, nesn will then turn to 0.
    new_conn_info->md = 0;
    new_conn_info->tx_len = 0;
    //new_conn_info->tx_buf = g_tx_pld_cmn_buf_dummy;
    new_conn_info->tx_buf = NULL;
    new_conn_info->last_rx_nesn_is_match = 1;
    new_conn_info->last_tx_is_acked = 1;
    new_conn_info->accu_tx_len = 0;
    new_conn_info->window_size_in_us = new_conn_info->conn_ind_payload.LLData.WinSize * 1250;
    new_conn_info->window_widen_size_in_us = lc_conn_state_calculate_window_widen_size_in_us(new_conn_info);

    new_conn_info->tx_ctr = 0;
    new_conn_info->rx_ctr = 0;
    new_conn_info->conn_hdl = lc_conn_state_calculate_new_conn_handle();
    new_conn_info->tx_max_len = 255;
    new_conn_info->tx_md_enable = 1;
    new_conn_info->rx_md_enable = 1;
    new_conn_info->tx_hold = 0;
    new_conn_info->rx_hold = 0;

    new_conn_info->curr_tx_request = NULL;
    new_conn_info->lm_tx_request = NULL;

    new_conn_info->prev_dev = NULL;
    new_conn_info->next_dev = NULL;

    new_dev->connection_handle = new_conn_info->conn_hdl; //for upper

    lc_conn_state_push_dev_to_list_tail(new_dev);
    *out_new_dev = new_dev;
    return  0;
}

#define ___MASTER______________________________
#define ___MASTER_____________________________
#define ___MASTER____________________________
void lc_mas_conn_state_machine(Bt_Dev_Info_t *mas_dev, LC_CONNECTION_STATE_EVENT_t evt)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)mas_dev->infrastructure;
#ifdef DFS_SIM_ON
    LC_CONNECTION_STATE_t old_state = conn_info->state;
#endif
    switch (conn_info->state)
    {
        case LC_CONN_STT_ESTABLISHED: {
            switch (evt)
            {
                case LC_CONN_STT_EVT_JUST_SENT_CONN_IND: {
//                    lc_mas_set_timer_for_adding_requestXX(mas_dev);
                    conn_info->state = LC_CONN_STT_SLEEPING_FOR_ADDING_REQUEST;
                } break;

                default: {
                } break;
            }
        } break;

        case LC_CONN_STT_SLEEPING_FOR_ADDING_REQUEST: {
            switch (evt)
            {
                case LC_CONN_STT_EVT_SLEEP_TIMESUP: {
//                    lc_mas_add_requestXX(mas_dev);
                    conn_info->state = LC_CONN_STT_WAITING_FOR_1ST_GRANT;
                } break;

                default: {
                } break;
            }
        } break;

        case LC_CONN_STT_WAITING_FOR_1ST_GRANT: {
            switch (evt)
            {
                case LC_CONN_STT_EVT_SCH_GRANT: {
                    MASTER_DUMP2(" ENABLE TX & RX_TIFS\n");
//                    lc_mas_connection_event_initXX(mas_dev);
//                    lc_mas_connection_event_startXX(mas_dev);
                    conn_info->state = LC_CONN_STT_ON_CONNECTION_EVENT;
                } break;

                default: {
                } break;
            }
        } break;

        case LC_CONN_STT_ON_CONNECTION_EVENT: {
            switch (evt)
            {
                default: {
                } break;
            }
        } break;

        default: {
            ASSERT_Reboot(0);
        } break;
    }
#ifdef DFS_SIM_ON
        printf("%s(), old_state:%d, new_state:%d, evt:%d, time:%d\n", __func__, old_state, conn_info->state, evt, SimAir_TimeStamp_Low_Get());
        MASTER_DUMP2(" state machine old:%d, new:%d, evt:%d\n", \
                    old_state, \
                    conn_info->state, \
                    evt);
#endif
}

void lc_mas_handle_conn_ind(Bt_Dev_Info_t *dev_from_ini)
{
    Bt_Dev_Info_t *new_mas_dev;
    Adv_Connect_Ind_Payload_t *p_conn_ind_from_ini = (Adv_Connect_Ind_Payload_t *)dev_from_ini->infrastructure;

    lc_conn_state_create_dev_by_conn_ind(LE_MASTER, p_conn_ind_from_ini, &new_mas_dev);
    lc_conn_state_dump_all_dev();

    lc_mas_conn_state_machine(new_mas_dev, LC_CONN_STT_EVT_JUST_SENT_CONN_IND);

    //Dump_Conn_Info(new_mas_dev);
}

