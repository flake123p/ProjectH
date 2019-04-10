#ifdef DFS_SIM_ON
#include "Everything_App.hpp"

//0 to 1, or 1 to 0
#define TOGGLE_1_BIT(a) a?0:1
#define PTR_TO_U32(a) (u8 *)(a)
#define T0_SLOT_TIMER_SET(a)             PHY_INFO->T0_SLOT_TIMER=PHY_INFO->clkb+(a)
#define T1_SLOT_TIMER_SET(a)             PHY_INFO->T0_SLOT_TIMER=PHY_INFO->clkb+(a)

#else //#ifdef DFS_SIM_ON
#define POINTER_TO_U32(a) (u32)(a)
#define PTR_TO_U32(a) (u32)(a)
#define T0_SLOT_TIMER_SET(a) pNBBREG2->Task0SlotTimer.Gbyte.LLbyte=pNBBREG2->CLK.Gbyte.LLbyte+(a)
#define T1_SLOT_TIMER_SET(a) pNBBREG2->Task1SlotTimer.Gbyte.LLbyte=pNBBREG2->CLK.Gbyte.LLbyte+(a)

#endif //#ifdef DFS_SIM_ON

#define LC_CONN_STATE_RX_BUF_LEN 4096

#define CONPENSATE_SCH_TIME_MASTER (313*3)
#define CONPENSATE_MASTER_DELAY 0
#define CONPENSATE_SCH_TIME_SLAVE (1240)
#define CONPENSATE_SLAVE_DELAY 0

LL_Info_t g_ll_info_body;
LL_Info_t *g_ll_info = &g_ll_info_body;

void lc_conn_state_ll_info_set(LL_Info_t *ll_info)
{
#ifdef DFS_SIM_ON
        g_ll_info = ll_info;
#else
        ll_info = ll_info;
#endif
}

void lc_conn_state_init(void)
{
#ifdef DFS_SIM_ON
    #define pvPortMalloc_ext malloc
#else
    extern void *pvPortMalloc_ext(u32 xWantedSize);
#endif
    g_ll_info->dev_head = NULL;
    g_ll_info->dev_tail = NULL;

    if (g_ll_info->tx_hdr_buf  == NULL)
        g_ll_info->tx_hdr_buf = (u8 *)pvPortMalloc_ext(4);

    if (g_ll_info->tx_pld_buf == NULL)
        g_ll_info->tx_pld_buf = (u8 *)pvPortMalloc_ext(4); //dummy, useless, because tx buffer is really in tx request descriptor

    if (g_ll_info->rx_hdr_buf == NULL)
        g_ll_info->rx_hdr_buf = (u8 *)pvPortMalloc_ext(10);

    if (g_ll_info->rx_pld_buf == NULL)
        g_ll_info->rx_pld_buf = (u8 *)pvPortMalloc_ext(LC_CONN_STATE_RX_BUF_LEN);

    g_ll_info->rx_pld_buf_curr_index = 0;
}

void lc_conn_state_tx_hold_enable(Bt_Dev_Info_t *dev)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)dev->infrastructure;

    conn_info->tx_hold = 1;

    if (conn_info->tx_ctr == 0) {
        //first packet, need init value
        conn_info->tx_llid = 1; //01b = LL Data PDU: Continuation fragment of an L2CAP message, or an Empty PDU
        conn_info->tx_sn = 0; //set to 0 manully, because tx is holded
        //Ignore nesn, it's in rx hold enable
        //conn_info->nesn = 1;
        conn_info->tx_md = 0;
        conn_info->tx_len = 0;
        conn_info->tx_buf = g_ll_info->tx_pld_buf;
    }
}

void lc_conn_state_tx_hold_disable(Bt_Dev_Info_t *dev)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)dev->infrastructure;

    conn_info->tx_hold = 0;

    if (conn_info->tx_ctr == 0) {
        conn_info->tx_sn = 1; //set to init value manully
    }
}

void lc_conn_state_rx_hold_enable(Bt_Dev_Info_t *dev)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)dev->infrastructure;

    conn_info->rx_hold = 1;

    if (conn_info->rx_ctr == 0) {
        //first packet, need init value
        conn_info->tx_nesn = 0; //set to 0 manully
    }
}

void lc_conn_state_rx_hold_disable(Bt_Dev_Info_t *dev)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)dev->infrastructure;

    conn_info->rx_hold = 0;

    if (conn_info->rx_ctr == 0) {
        conn_info->tx_nesn = 1; //set to init value manully
    }
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

void lc_conn_state_delete_dev_from_list(Bt_Dev_Info_t *dev)
{
    Bt_Dev_Info_t *curr_dev = g_ll_info->dev_head;
    while(curr_dev != NULL)
    {
        Conn_State_Info_t *curr_conn_info = (Conn_State_Info_t *)curr_dev->infrastructure;
        if (curr_dev == dev) {
            Bt_Dev_Info_t *prev_dev = curr_conn_info->prev_dev;
            Bt_Dev_Info_t *next_dev = curr_conn_info->next_dev;
            if (prev_dev != NULL) {
                Conn_State_Info_t *prev_conn_info = (Conn_State_Info_t *)prev_dev->infrastructure;
                prev_conn_info->next_dev = curr_conn_info->next_dev;
            }
            if (next_dev != NULL) {
                Conn_State_Info_t *next_conn_info = (Conn_State_Info_t *)next_dev->infrastructure;
                next_conn_info->prev_dev = curr_conn_info->prev_dev;
            }
            if (curr_dev == g_ll_info->dev_head) {
                g_ll_info->dev_head = curr_conn_info->next_dev;
            }
            if (curr_dev == g_ll_info->dev_tail) {
                g_ll_info->dev_tail = curr_conn_info->prev_dev;
            }
            break;
        }
        else {
            curr_dev = curr_conn_info->next_dev;
        }
    }
}

Bt_Dev_Info_t * lc_conn_state_get_dev_by_conn_handle(u16 conn_hdl)
{
    Bt_Dev_Info_t *curr_dev = g_ll_info->dev_head;
    while(curr_dev != NULL)
    {
        Conn_State_Info_t *curr_conn_info = (Conn_State_Info_t *)curr_dev->infrastructure;
        if (conn_hdl == curr_conn_info->conn_hdl) {
            return curr_dev;
        }
        else {
            curr_dev = curr_conn_info->next_dev;
        }
    }
    return NULL;
}

void lc_conn_state_dump_all_dev(void)
{
#ifdef DFS_SIM_ON
    u32 i = 0;
    Bt_Dev_Info_t *curr_dev = g_ll_info->dev_head;
    while(curr_dev != NULL)
    {
        Conn_State_Info_t *curr_conn_info = (Conn_State_Info_t *)curr_dev->infrastructure;
        MASTER_DUMP2(" -- [[ conn state dev dump %d ]]\n", i);
        MASTER_DUMP2(" -- Type:%d, Role:%d, Up_Conn_Hdl:0x%X\n", curr_dev->Type, curr_dev->Role, curr_dev->connection_handle);
        MASTER_DUMP2(" -- -- state:%d, tx_llid:%d, tx_sn:%d, tx_nesn:%d, tx_md:%d\n", curr_conn_info->state, curr_conn_info->tx_llid, curr_conn_info->tx_sn, curr_conn_info->tx_nesn, curr_conn_info->tx_md);
        MASTER_DUMP2(" -- -- tx_len:%d, now_is_new_data:%d, last_tx_is_acked:%d\n", curr_conn_info->tx_len, curr_conn_info->now_is_new_data, curr_conn_info->last_tx_is_acked);
        MASTER_DUMP2(" -- -- tx_ctr:%d, rx_ctr:%d, conn_hdl:0x%X\n", curr_conn_info->tx_ctr, curr_conn_info->rx_ctr, curr_conn_info->conn_hdl);
        MASTER_DUMP2(" -- -- tx_max_len:%d, tx_md_enable:%d, rx_md_enable:%d, tx_hold:%d, rx_hold:%d\n", curr_conn_info->tx_max_len, curr_conn_info->tx_md_enable, curr_conn_info->rx_md_enable, curr_conn_info->tx_hold, curr_conn_info->rx_hold);
        MASTER_DUMP2(" -- -- InitA:0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", \
                      curr_conn_info->conn_ind_payload.InitA[0], \
                      curr_conn_info->conn_ind_payload.InitA[1], \
                      curr_conn_info->conn_ind_payload.InitA[2], \
                      curr_conn_info->conn_ind_payload.InitA[3], \
                      curr_conn_info->conn_ind_payload.InitA[4], \
                      curr_conn_info->conn_ind_payload.InitA[5]);
        MASTER_DUMP2(" -- -- AdvA :0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", \
                      curr_conn_info->conn_ind_payload.AdvA[0], \
                      curr_conn_info->conn_ind_payload.AdvA[1], \
                      curr_conn_info->conn_ind_payload.AdvA[2], \
                      curr_conn_info->conn_ind_payload.AdvA[3], \
                      curr_conn_info->conn_ind_payload.AdvA[4], \
                      curr_conn_info->conn_ind_payload.AdvA[5]);
        MASTER_DUMP2(" -- -- LLData.AA :0x%X, 0x%X, 0x%X, 0x%X\n", \
                      curr_conn_info->conn_ind_payload.LLData.AA[0], \
                      curr_conn_info->conn_ind_payload.LLData.AA[1], \
                      curr_conn_info->conn_ind_payload.LLData.AA[2], \
                      curr_conn_info->conn_ind_payload.LLData.AA[3]);
        MASTER_DUMP2(" -- -- LLData.CRCInit :0x%X, 0x%X, 0x%X\n", \
                      curr_conn_info->conn_ind_payload.LLData.CRCInit[0], \
                      curr_conn_info->conn_ind_payload.LLData.CRCInit[1], \
                      curr_conn_info->conn_ind_payload.LLData.CRCInit[2]);
        MASTER_DUMP2(" -- -- LLData.WinSize:%d  WinOffset:%d  Interval:%d  Latency:%d  Timeout:%d\n", \
                      curr_conn_info->conn_ind_payload.LLData.WinSize, \
                      curr_conn_info->conn_ind_payload.LLData.WinOffset, \
                      curr_conn_info->conn_ind_payload.LLData.Interval, \
                      curr_conn_info->conn_ind_payload.LLData.Latency, \
                      curr_conn_info->conn_ind_payload.LLData.Timeout);
        MASTER_DUMP2(" -- -- LLData.ChM :0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", \
                      curr_conn_info->conn_ind_payload.LLData.ChM[0], \
                      curr_conn_info->conn_ind_payload.LLData.ChM[1], \
                      curr_conn_info->conn_ind_payload.LLData.ChM[2], \
                      curr_conn_info->conn_ind_payload.LLData.ChM[3], \
                      curr_conn_info->conn_ind_payload.LLData.ChM[4]);
        MASTER_DUMP2(" -- -- LLData.Hop:%d  SCA:%d\n", \
                      curr_conn_info->conn_ind_payload.LLData.Hop, \
                      curr_conn_info->conn_ind_payload.LLData.SCA);

        u32 j = 0;
        MASTER_DUMP2(" -- -- -- [[ tx_request dump start : %d ]]\n", j);
        Conn_State_Tx_Request_t *curr_tx_request = curr_conn_info->lm_tx_request;
        while (curr_tx_request != NULL) {
            MASTER_DUMP2(" -- -- -- -- tx_request_active:%d  tx_request_done:%d  tx_len:%d  tx_done_len:%d\n",
                          curr_tx_request->tx_request_active, \
                          curr_tx_request->tx_request_done, \
                          curr_tx_request->tx_len, \
                          curr_tx_request->tx_done_len);
            curr_tx_request = curr_tx_request->next;
            if (curr_tx_request != NULL) {
                j++;
                MASTER_DUMP2(" -- -- -- [[ tx_request dump start : %d ]]\n", j);
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

u8 lc_conn_state_create_dev_by_conn_ind(BT_DEV_ROLE_t role, Adv_Connect_Ind_Payload_t *in_conn_ind, Bt_Dev_Info_t **out_new_dev)
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
    new_conn_info->channel = new_conn_info->conn_ind_payload.LLData.Hop;

    /* LLID :
        01b = LL Data PDU: Continuation fragment of an L2CAP message, or an Empty PDU
    */
#if 1
    new_conn_info->rx_llid = 0xFF; //dummy
    new_conn_info->rx_sn = 0xFF; //dummy
    new_conn_info->rx_nesn = 0xFF; //dummy
    new_conn_info->rx_md = 0xFF; //dummy
    new_conn_info->rx_len = 0; //this initial value is only used in first packet of slave role

    new_conn_info->tx_llid = 0xFF; //dummy
    new_conn_info->tx_sn = 1;   //this initial value is only used in first packet of slave role
    new_conn_info->tx_nesn = 0; //this initial value is only used in first packet of slave role
    new_conn_info->rx_act = LC_CONN_STT_RX_ACT_NEW_BUF_SETTING; //this initial value is only used in first packet of slave role
    new_conn_info->tx_md = 0xFF; //dummy

    new_conn_info->tx_len = 0;
    //new_conn_info->tx_buf = g_tx_pld_cmn_buf_dummy;
    new_conn_info->tx_buf = NULL;

    new_conn_info->now_is_new_data = 0xFF; //dummy
    new_conn_info->last_tx_is_acked = 0xFF; //dummy
#endif
//    new_conn_info->accu_tx_len = 0;
    new_conn_info->window_size_in_us = new_conn_info->conn_ind_payload.LLData.WinSize * 1250;
    new_conn_info->window_widen_size_in_us = lc_conn_state_calculate_window_widen_size_in_us(new_conn_info);
    new_conn_info->timeout_ctr = 6;
    new_conn_info->latency_ctr = 0;

    new_conn_info->tx_ctr = 0;
    new_conn_info->rx_ctr = 0;
    new_conn_info->conn_hdl = lc_conn_state_calculate_new_conn_handle();
    if (new_conn_info->conn_ind_payload.LLData.Interval < 6) {
        new_conn_info->tx_max_len = 50 * new_conn_info->conn_ind_payload.LLData.Interval;
    } else {
        new_conn_info->tx_max_len = 255;
    }
    new_conn_info->tx_md_enable = 0; //disable md for now
    new_conn_info->rx_md_enable = 0; //disable md for now
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

void lc_conn_state_set_timer_for_adding_sch_request(Bt_Dev_Info_t *dev, u32 sleep_time_in_us)
{
    u32 cmd, module;

    if (dev->Role == LE_MASTER)
        module = LE_LC_MASTER;
    else
        module = LE_LC_SLAVE;
    cmd = ( (LC_GROUP_EVENT)                           //Group
           |(MESSAGE_SUBGROUP_LE << SUBGROUP_OFFSET)   //Sub-group
           |(module << LE_MODULE_OFFSET)               //Module
           |(LC_CONN_STT_EVT_SLEEP_TIMESUP) );;        //Message

#ifdef DFS_SIM_ON
    {
        cmd = cmd;
        extern void BT_Phy_API_Timer_0(BT_PHY_Info_t *phy_info, Bt_Dev_Info_t *dev, u32 sleep_time_in_us);
        BT_Phy_API_Timer_0(PHY_INFO, dev, sleep_time_in_us);
    }
#else
    controller_timer_insert((void*)dev, sleep_time_in_us , LC_TASK_QUEUE, cmd, 0, NULL);
#endif //#ifdef DFS_SIM_ON
}

int lc_conn_state_tx_request_add_from_upper(u16 conn_hdl, Conn_State_Tx_Request_t *lm_tx_request)
{
    Bt_Dev_Info_t *dev = lc_conn_state_get_dev_by_conn_handle(conn_hdl);

    if (dev == NULL)
        return 1;

    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)dev->infrastructure;

    Conn_State_Tx_Request_t **curr_tx_request = &(conn_info->lm_tx_request);
    while (1) {
        if (*curr_tx_request == NULL)
        {
            *curr_tx_request = lm_tx_request;
            break;
        }
        else
        {
            curr_tx_request = &((*curr_tx_request)->next);
        }
    }

    return 0;
}

int lc_conn_state_tx_request_clear_from_upper(Bt_Dev_Info_t *dev)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)dev->infrastructure;

    Conn_State_Tx_Request_t *last_tx_request = conn_info->lm_tx_request;
    while (1) {
        if (last_tx_request == NULL)
            break;

        if (last_tx_request->tx_request_done)
            last_tx_request = last_tx_request->next;
        else
            break;
    }

    conn_info->lm_tx_request = last_tx_request;

    return 0;
}


int lc_conn_state_tx_request_done_to_upper(Bt_Dev_Info_t *dev, Conn_State_Tx_Request_t *complete_tx_request)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)dev->infrastructure;

    printf("[TX ACK] 02 %02X %02X %02X %02X ", (u8)conn_info->conn_hdl, (u8)conn_info->conn_hdl>>8, (u8)complete_tx_request->tx_len, (u8)complete_tx_request->tx_len>>8);
    for (u32 i=0; i<complete_tx_request->tx_len; i++) {
        printf("%02X ", complete_tx_request->tx_buf[i]);
    }
    printf("\n");

    //demo
    lc_conn_state_tx_request_clear_from_upper(dev);

    return 0;
}

void lc_conn_state_tx_request_update(Conn_State_Info_t *conn_info)
{
    Conn_State_Tx_Request_t *scan_tx_request = conn_info->curr_tx_request;
    if (scan_tx_request == NULL)
        scan_tx_request = conn_info->lm_tx_request;
    while (1) {
        if (scan_tx_request != NULL)
        {
            if (scan_tx_request->tx_request_active)
            {
                if (scan_tx_request->tx_request_done == 0)
                {
                    conn_info->curr_tx_request = scan_tx_request;
                    //DUMPA(scan_tx_request->tx_buf);
                    //DUMPA(conn_info->role);
                    break;
                }
                else
                {
                    scan_tx_request = scan_tx_request->next;
                }
            }
            else
            {
                scan_tx_request = scan_tx_request->next;
            }
        }
        else
        {
            conn_info->curr_tx_request = NULL;
            break;
        }
    }
}

void lc_conn_state_tx_request_ack_check(Bt_Dev_Info_t *dev)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)dev->infrastructure;

    if (conn_info->last_tx_is_acked) {
        if (conn_info->curr_tx_request != NULL) {
            if (conn_info->curr_tx_request->tx_done_len == conn_info->curr_tx_request->tx_len) {
                conn_info->curr_tx_request->tx_request_done = 1;
                Conn_State_Tx_Request_t *complete_tx_request = conn_info->curr_tx_request;
                //renew curr_tx_request
                lc_conn_state_tx_request_update(conn_info);

                lc_conn_state_tx_request_done_to_upper(dev, complete_tx_request);
            }
        }
    }
}

static void lc_conn_state_assign_tx_buf(u8 *tx_plh, u32 tx_plh_len, u8 *tx_pld, u32 tx_pld_len)
{
    DIRECT_RFIELD_AGU_TXLE_PLH_SP = PTR_TO_U32(&(tx_plh[0]));
    DIRECT_RFIELD_AGU_TXLE_PLH_CP_RE = PTR_TO_U32(&(tx_plh[0]));
    DIRECT_RFIELD_AGU_TXLE_PLH_EP = PTR_TO_U32(&(tx_plh[tx_plh_len]));

    DIRECT_RFIELD_AGU_TXLE_PLD_SP = PTR_TO_U32(&(tx_pld[0]));
    DIRECT_RFIELD_AGU_TXLE_PLD_CP_RE = PTR_TO_U32(&(tx_pld[0]));
    DIRECT_RFIELD_AGU_TXLE_PLD_EP = PTR_TO_U32(&(tx_pld[tx_pld_len]));
}

static void lc_conn_state_assign_rx_header_buf(u8 *rx_plh, u32 rx_plh_len)
{
    DIRECT_RFIELD_AGU_RXLE_PLH_SP = PTR_TO_U32(&(rx_plh[0]));
    DIRECT_RFIELD_AGU_RXLE_PLH_CP = PTR_TO_U32(&(rx_plh[0]));
    DIRECT_RFIELD_AGU_RXLE_PLH_EP = PTR_TO_U32(&(rx_plh[rx_plh_len]));
}

static void lc_conn_state_assign_rx_payload_buf(u8 *rx_pld, u32 rx_pld_len)
{
    DIRECT_RFIELD_AGU_RXLE_PLD_SP = PTR_TO_U32(&(rx_pld[0]));
    DIRECT_RFIELD_AGU_RXLE_PLD_CP = PTR_TO_U32(&(rx_pld[0]));
    DIRECT_RFIELD_AGU_RXLE_PLD_EP = PTR_TO_U32(&(rx_pld[rx_pld_len]));
}

void lc_conn_state_rx_timeout_check(Conn_State_Info_t *conn_info)
{
    conn_info->now_is_new_data = 0;
    conn_info->last_tx_is_acked = 0;
}

void lc_conn_state_rx_ack_check(Bt_Dev_Info_t *dev)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)dev->infrastructure;

    u8 rx_header = g_ll_info->rx_hdr_buf[0];
    u8 rx_length = g_ll_info->rx_hdr_buf[1];
    conn_info->rx_sn = rx_header & 0x08 ? 1 : 0;
    conn_info->rx_nesn = rx_header & 0x04 ? 1 : 0;
    conn_info->rx_len = rx_length;

    BASIC_ASSERT(conn_info->tx_sn < 2);
    BASIC_ASSERT(conn_info->tx_nesn < 2);

    if (conn_info->tx_nesn == conn_info->rx_sn) {
        conn_info->now_is_new_data = 1;
        if (conn_info->rx_hold) {
            //ignore this new data
        } else {
            conn_info->rx_ctr++;
            //g_ll_info->rx_pld_buf_curr_index += rx_length;
        }
    } else {
        conn_info->now_is_new_data = 0;
    }

    if (conn_info->tx_sn != conn_info->rx_nesn) {
        conn_info->last_tx_is_acked = 1;
        lc_conn_state_tx_request_ack_check(dev);
    } else {
        conn_info->last_tx_is_acked = 0;
    }
}

//only used in master
void lc_conn_state_tx_prepare_0_sn_update_first_packet(Conn_State_Info_t *conn_info)
{
    conn_info->tx_sn = 0;
    conn_info->tx_nesn = 0;

    if (conn_info->tx_hold)
        conn_info->tx_act = LC_CONN_STT_TX_ACT_EMPTY_PACKET;
    else
        conn_info->tx_act = LC_CONN_STT_TX_ACT_NEW_DATA;

    //Regardless of rx_hold, must prepare for first rx packet
    conn_info->rx_act = LC_CONN_STT_RX_ACT_NEW_BUF_SETTING;
}

void lc_conn_state_tx_prepare_0_sn_update(Conn_State_Info_t *conn_info)
{
    //NESN update
    conn_info->rx_act = LC_CONN_STT_RX_ACT_KEEP_BUF_SETTING;
    if (conn_info->now_is_new_data) {
        if (conn_info->rx_hold) {
            //do nothing, use old nesn. Keep receiveing new data, and drop this new data.
        } else {
            conn_info->rx_act = LC_CONN_STT_RX_ACT_NEW_BUF_SETTING;
            conn_info->tx_nesn = TOGGLE_1_BIT(conn_info->tx_nesn);
        }
    } else {
        //do nothing, just drop this old data. Regardless of rx_hold parameter.
    }
    BASIC_ASSERT(conn_info->tx_nesn < 2);

    //SN update
    if (conn_info->last_tx_is_acked) {
        if (conn_info->tx_hold) {
            //do nothing, use old sn. But send no data.
            conn_info->tx_act = LC_CONN_STT_TX_ACT_EMPTY_PACKET;
        } else {
            conn_info->tx_act = LC_CONN_STT_TX_ACT_NEW_DATA;
            conn_info->tx_sn = TOGGLE_1_BIT(conn_info->tx_sn);
        }
    } else {
        //do nothing, use old sn. Keep sending old data.
        conn_info->tx_act = LC_CONN_STT_TX_ACT_OLD_DATA;
    }
    BASIC_ASSERT(conn_info->tx_sn < 2);
}

void lc_conn_state_tx_prepare_1_data_update(Conn_State_Info_t *conn_info)
{
    if (conn_info->tx_act == LC_CONN_STT_TX_ACT_NEW_DATA) {
        lc_conn_state_tx_request_update(conn_info); //update "conn_info->curr_tx_request"
        if (conn_info->curr_tx_request == NULL) {
            //nothing to send
            conn_info->tx_act = LC_CONN_STT_TX_ACT_EMPTY_PACKET;
        }
    }

    switch (conn_info->tx_act) {
        case LC_CONN_STT_TX_ACT_EMPTY_PACKET: {
            conn_info->tx_llid = 1; //01b = LL Data PDU: Continuation fragment of an L2CAP message, or an Empty PDU
            conn_info->tx_md = 0;
            conn_info->tx_len = 0;
            conn_info->tx_buf = g_ll_info->tx_pld_buf; //dummy, no use
        } break;

        case LC_CONN_STT_TX_ACT_OLD_DATA: {
            //old tx
            //llid is the same
            //sn is the same
            //nesn may be updated by "lc_conn_state_tx_prepare_0_sn_update()"
            //md is the same
            //tx_len is the same
        } break;

        case LC_CONN_STT_TX_ACT_NEW_DATA: {
            u32 is_remain_tx_data = 0;
            u32 tx_len_this_ll_packet = conn_info->curr_tx_request->tx_len - conn_info->curr_tx_request->tx_done_len;
            if (conn_info->curr_tx_request->tx_done_len == 0) { /* first segment */
                conn_info->tx_llid = 2; //10b = LL Data PDU: Start of an L2CAP message or a complete L2CAP message with no fragmentation.
            }
            else {
                //continue packet
                conn_info->tx_llid = 1; //01b = LL Data PDU: Continuation fragment of an L2CAP message, or an Empty PDU
            }
            conn_info->tx_buf = &(conn_info->curr_tx_request->tx_buf[conn_info->curr_tx_request->tx_done_len]);
            if (tx_len_this_ll_packet > conn_info->tx_max_len) {
                tx_len_this_ll_packet = conn_info->tx_max_len;
                is_remain_tx_data = 1;
            }
            conn_info->tx_len = tx_len_this_ll_packet;
            conn_info->curr_tx_request->tx_done_len += tx_len_this_ll_packet;
            // more data calculate
            if (0 == conn_info->tx_md_enable) {
                conn_info->tx_md = 0;
            }
            else {
                if (is_remain_tx_data) {
                    conn_info->tx_md = 1;
                }
                else {
                    //check is there next tx request
                    if (conn_info->curr_tx_request->next != NULL)
                        conn_info->tx_md = 1;
                    else
                        conn_info->tx_md = 0;
                }
            }
        } break;

        default:
            BASIC_ASSERT(0);
            break;
    }

    BASIC_ASSERT(conn_info->tx_md < 2);
    BASIC_ASSERT(conn_info->tx_sn < 2);
    BASIC_ASSERT(conn_info->tx_nesn < 2);
    BASIC_ASSERT(conn_info->tx_llid < 4);
    g_ll_info->tx_hdr_buf[0] = (conn_info->tx_md << 4) | (conn_info->tx_sn << 3) | (conn_info->tx_nesn << 2) | conn_info->tx_llid;
    g_ll_info->tx_hdr_buf[1] = conn_info->tx_len;
    lc_conn_state_assign_tx_buf(g_ll_info->tx_hdr_buf, 2, conn_info->tx_buf, conn_info->tx_len);
}

void lc_conn_state_rx_prepare(Conn_State_Info_t *conn_info)
{
    lc_conn_state_assign_rx_header_buf(g_ll_info->rx_hdr_buf, 2);
    switch (conn_info->rx_act) {
        case LC_CONN_STT_RX_ACT_KEEP_BUF_SETTING: {
            //ignore last rx payload
            lc_conn_state_assign_rx_payload_buf(&(g_ll_info->rx_pld_buf[g_ll_info->rx_pld_buf_curr_index]), LC_CONN_STATE_RX_BUF_LEN-g_ll_info->rx_pld_buf_curr_index);
        } break;

        case LC_CONN_STT_RX_ACT_NEW_BUF_SETTING: {
            g_ll_info->rx_pld_buf_curr_index += conn_info->rx_len;
            // turn around when payload buffer is small than 255
            if (g_ll_info->rx_pld_buf_curr_index + 255 > LC_CONN_STATE_RX_BUF_LEN) {
                g_ll_info->rx_pld_buf_curr_index = 0;
            }
            conn_info->rx_len = 0;
            lc_conn_state_assign_rx_payload_buf(&(g_ll_info->rx_pld_buf[g_ll_info->rx_pld_buf_curr_index]), LC_CONN_STATE_RX_BUF_LEN-g_ll_info->rx_pld_buf_curr_index);
        } break;

        default:
            BASIC_ASSERT(0);
            break;
    }
}


void lc_conn_state_connection_event_init(Bt_Dev_Info_t *dev)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)dev->infrastructure;
#ifdef PLATFORM
    #if (PLATFORM == PLATFORM_CHIMERA)
        ISR_INT00_TABLE[IRQ_LE_PL_RCV_OK].isr_entry = LE_ISR_SLA_LE_PL_RCV_OK_Handler;
        ISR_INT00_TABLE[IRQ_LE_PL_RCV_ERR].isr_entry = LE_ISR_SLA_LE_PL_RCV_ERR_Handler;
        ISR_INT00_TABLE[IRQ_LE_CORREL_OK_8MHZ].isr_entry = LE_ISR_SLA_LE_CORREL_OK_8MHZ_Handler;
        ISR_INT00_TABLE[IRQ_LE_CORREL_ERR].isr_entry = LE_ISR_SLA_LE_CORREL_ERR_Handler;
    #endif
#endif
    //lc_conn_state_rf_buf_set(dev);

    LE_PHY_ACCESS_CODE_LLbyte=conn_info->conn_ind_payload.LLData.AA[0];
    LE_PHY_ACCESS_CODE_LHbyte=conn_info->conn_ind_payload.LLData.AA[1];
    LE_PHY_ACCESS_CODE_HLbyte=conn_info->conn_ind_payload.LLData.AA[2];
    LE_PHY_ACCESS_CODE_HHbyte=conn_info->conn_ind_payload.LLData.AA[3];
    //enable test packet format and disable whiten, enable LE xxx
    LE_Control_Reg_LLbyte=0x01;

    //fixed channel
    //LE_HOP_CTRL0_Reg_HLbyte = (0x80|(conn_info->conn_ind_payload.LLData.Hop));
    LE_HOP_CTRL0_Reg_HLbyte = 0x80|(conn_info->channel);

    //channel map all enable
    //NBB_Reg.LE_HOP_CTRL1_Reg.Gword.Lword=0xff;
    //NBB_Reg.LE_HOP_CTRL1_Reg.Gword.Hword=0xff;
    //NBB_Reg.LE_HOP_CTRL2_Reg.Gbyte.LLbyte=0x1f;

    //CRC24 initial value of RF test packet is 0x555555
    LE_PHY_CRC24_LL=conn_info->conn_ind_payload.LLData.CRCInit[0];
    LE_PHY_CRC24_LH=conn_info->conn_ind_payload.LLData.CRCInit[1];
    LE_PHY_CRC24_HL=conn_info->conn_ind_payload.LLData.CRCInit[2];
}

void lc_conn_state_disconnect(Bt_Dev_Info_t *dev)
{
    //Conn_State_Info_t *conn_info = (Conn_State_Info_t *)dev->infrastructure;
    lc_conn_state_delete_dev_from_list(dev);

    //TODO: delete device
}

void lc_conn_state_machine(Bt_Dev_Info_t *dev, LC_CONNECTION_STATE_EVENT_t evt)
{
    if (dev->Role == LE_MASTER) {
        lc_mas_state_machine(dev, evt);
    }
    else {
        lc_sla_state_machine(dev, evt);
    }
}

#define ___MASTER______________________________
#define ___MASTER_____________________________
#define ___MASTER____________________________
void lc_mas_add_request(Bt_Dev_Info_t *mas_dev)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)mas_dev->infrastructure;

    u32 value;
    Request_ADDR_List_t *p_requ_addr_list = Scheduler_APIGetRequestAddressList();
    Request_ADDR_T      *p_requ_addr =      Scheduler_APIGetRequestAddress();
    //Scheduler_Request_T *p_sch_requ = &g_sch_requ;
    Scheduler_Request_T *p_sch_requ =       Scheduler_APIGetRequest();
    p_requ_addr->Request_ADDR = POINTER_TO_U32(p_sch_requ);
    TAILQ_INSERT_TAIL(p_requ_addr_list, p_requ_addr, next);

    //TAILQ_FOREACH(p_adv_sch_req_addr, gp_sch_req_addr_list, next)
    {
        //p_adv_sch_req = (Scheduler_Request_T*)p_adv_sch_req_addr->Request_ADDR;
        //ASSERT_Reboot(p_adv_sch_req->dev != NULL);
        //p_adv_dev = (Adv_Dev_t*)p_adv_sch_req->dev->infrastructure;
        //ASSERT_Reboot((p_adv_dev->adv_event_type) != EXT_PERIODIC); //TBD, has not implemented lm_adv_add_sch_req for Periodic

        p_sch_requ->ABORT_callback_task_queue = LC_TASK_QUEUE;
        p_sch_requ->ABORT_callback_message_id = ( (LC_GROUP_EVENT)                           //Group
                                                 |(MESSAGE_SUBGROUP_LE << SUBGROUP_OFFSET)   //Sub-group
                                                 |(LE_LC_SLAVE << LE_MODULE_OFFSET)          //Module
                                                 |(LC_SLA_MSG_HANDLE_ABORT) );;              //Message

        p_sch_requ->ACK_callback_task_queue = LC_TASK_QUEUE;
        p_sch_requ->ACK_callback_message_id =   ( (LC_GROUP_EVENT)                           //Group
                                                 |(MESSAGE_SUBGROUP_LE << SUBGROUP_OFFSET)   //Sub-group
                                                 |(LE_LC_SLAVE << LE_MODULE_OFFSET)          //Module
                                                 |(LC_SLA_MSG_HANDLE_GRANT) );;              //Message;

        p_sch_requ->NACK_callback_task_queue = INVALID_TASK_QUEUE;
        p_sch_requ->NACK_callback_message_id = 0;

        p_sch_requ->Clear_Request_callback_task_queue = BBM_TASK_QUEUE;
        p_sch_requ->Clear_Request_callback_message_id = ( (BB_MANAGER_MSG_GROUP_LMP)                     //Group
                                                         |(MESSAGE_SUBGROUP_LE << SUBGROUP_OFFSET)       //Sub-group
                                                         |(LE_LM_SLAVE << LE_MODULE_OFFSET)              //Module
                                                         |(LM_SLA_MSG_HANDLE_CALLBACK_OF_CLEAR_REQ) );;  //Message;

        p_sch_requ->active = REQUEST_ACTIVE;

//#if LE_ADV_DECIDE_SEC_EVENT_ANCHOR_WORKAROUND
#if 0
if(p_adv_dev->event.is_preiodical == TRUE)
{
    u32 current_clknu;
    u32 offset_in_us = 1250;//temp use 600us
    u32 offset_in_clknu;

    current_clknu = bt_hal_read_clknu();

    offset_in_clknu = CLK_Trans_Us_To_CLKU(offset_in_us);

    p_adv_sch_req->anchor = CLK_CLKU_Add(current_clknu, offset_in_clknu);
}
#else
        //ASSERT_Reboot(0);
#endif
#if 0
{
    u32 current_clknu;
    u32 offset_in_us = 1250 - /*compensate*/0;//temp use 600us
    u32 offset_in_clknu;

    current_clknu = bt_hal_read_clknu();

    offset_in_clknu = CLK_Trans_Us_To_CLKU(offset_in_us);

    p_sch_requ->anchor = CLK_CLKU_Add(current_clknu, offset_in_clknu);
}
#else
    p_sch_requ->anchor = 0;
#endif
        //p_sch_requ->anchor_clku;

        p_sch_requ->CLK_type = CLK_TYPE_CLKN;
        p_sch_requ->anchor_unit = ANCHOR_UNIT_CLKU;
        p_sch_requ->bandwidth_unit = BANDWIDTH_UNIT_US;
        p_sch_requ->data_len = 0;
        p_sch_requ->data = NULL;
        //p_sch_requ->granted_bandwidth;
        p_sch_requ->is_periodical = true;
        p_sch_requ->request_type = REQUEST_TYPE_PREEMPTIVE;
        p_sch_requ->max_random_delay_us = 0;
        p_sch_requ->max_request_bandwidth = 1000;
        p_sch_requ->min_request_bandwidth = 1000;
        //p_sch_requ->next;
        p_sch_requ->periodical_interval_us = conn_info->conn_ind_payload.LLData.Interval * 1250;
        p_sch_requ->priority = 9;
    }

    p_sch_requ->dev = mas_dev;

#ifdef DFS_SIM_ON
    value = 0;
    value = value;
    free(p_requ_addr_list);
    free(p_requ_addr);
    {
        extern void BT_Phy_API_Sch_0_Add_Request(BT_PHY_Info_t *phy_info, Scheduler_Request_T *p_sch_requ, u32 sch_delay_in_us);
        MASTER_DUMP2(" Sch 0 add request ... interval:%d\n", p_sch_requ->periodical_interval_us);
        BT_Phy_API_Sch_0_Add_Request(PHY_INFO, p_sch_requ, 447 /*scheduler_simulate_delay_in_us()*/);
    }
#else
    value = (u32)p_requ_addr_list;
    Controller_TaskCmd(NULL, (RC_GROUP_SCHEDULER | ADD_REQUEST), 4, (u8*)&value, RC_TASK_QUEUE, 0);
#endif
}

void lc_mas_remove_request(Bt_Dev_Info_t *mas_dev)
{
#ifdef DFS_SIM_ON
    {
        extern void BT_Phy_API_Sch_0_Remove_Request(BT_PHY_Info_t *phy_info);
        MASTER_DUMP2(" Sch 0 remove request\n");
        BT_Phy_API_Sch_0_Remove_Request(PHY_INFO);
    }
#else
    //value = (u32)p_requ_addr_list;
    //Controller_TaskCmd(NULL, (RC_GROUP_SCHEDULER | ADD_REQUEST), 4, (u8*)&value, RC_TASK_QUEUE, 0);
#endif

}

void lc_mas_connection_event_start(Bt_Dev_Info_t *mas_dev)
{
    //Conn_State_Info_t *conn_info = (Conn_State_Info_t *)mas_dev->infrastructure;

    //why
    LET0T1STOP;
    
    //why again?
    //le_sla_assign_rx_buf(gp_cmn_buf_rx_payload_curr, gp_cmn_buf_rx_payload_left_size);
    
    //why again?
    //le_sla_assign_tx_buf(gp_cmn_buf_tx_payload_curr, TX_TOTAL_SIZE);
    
    DIRECT_RFIELD_DISABLE_TIMEOUT = 0;
    DIRECT_RFIELD_RX_TIMEOUT_VALUE = 100;//conn_info->window_size_in_us;
    T1_SLOT_TIMER_SET(1);

    T1TXENABLE;

    T0RXENABLE_TIFS;
    SET_RF_DEV(mas_dev);
}

void lc_mas_state_machine(Bt_Dev_Info_t *mas_dev, LC_CONNECTION_STATE_EVENT_t evt)
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
                    u32 sleep_time_in_us = 1250 + (1250 * conn_info->conn_ind_payload.LLData.WinOffset) - CONPENSATE_SCH_TIME_MASTER + CONPENSATE_MASTER_DELAY;
                    lc_conn_state_set_timer_for_adding_sch_request(mas_dev, sleep_time_in_us);
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
                    lc_mas_add_request(mas_dev);
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
                    MASTER_DUMP2(" 1st ENABLE TX & RX_TIFS\n");
                    lc_conn_state_connection_event_init(mas_dev);
                    lc_conn_state_tx_prepare_0_sn_update_first_packet(conn_info);
                    lc_conn_state_tx_prepare_1_data_update(conn_info);
                    lc_conn_state_rx_prepare(conn_info);
                    lc_mas_connection_event_start(mas_dev);
                    conn_info->state = LC_CONN_STT_ON_CONNECTION_EVENT;
                } break;

                default: {
                } break;
            }
        } break;

        case LC_CONN_STT_ON_CONNECTION_EVENT: {
            switch (evt)
            {
                case LC_CONN_STT_EVT_SCH_ABORT: {
                    lc_conn_state_dump_all_dev();
                } break;

                case LC_CONN_STT_EVT_SCH_GRANT: {
                    MASTER_DUMP2(" xxx ENABLE TX & RX_TIFS\n");
                    conn_info->channel = (conn_info->channel + conn_info->conn_ind_payload.LLData.Hop) % 37;
                    lc_conn_state_connection_event_init(mas_dev);
                    lc_conn_state_tx_prepare_0_sn_update(conn_info);
                    lc_conn_state_tx_prepare_1_data_update(conn_info);
                    lc_conn_state_rx_prepare(conn_info);
                    lc_mas_connection_event_start(mas_dev);
                } break;

                case LC_CONN_STT_EVT_PL_RCV_ERR:
                case LC_CONN_STT_EVT_RX_TIMEOUT: {
                    MASTER_DUMP2(" RX_TIMEOUT !!!\n");
                    lc_conn_state_rx_timeout_check(conn_info);
                    conn_info->timeout_ctr--;
                    if (conn_info->timeout_ctr == 0) {
                        MASTER_DUMP2(" DISCONNECT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                        lc_mas_remove_request(mas_dev);
                        //lc_conn_state_disconnect(mas_dev);
                        conn_info->state = LC_CONN_STT_DISCONNECTED;
                        lc_conn_state_disconnect(mas_dev);
                        return;
                    }
                } break;

                case LC_CONN_STT_EVT_PL_RCV_OK: {
                    #ifdef DFS_SIM_ON
                    {
                        u32 len = g_ll_info->rx_hdr_buf[1];
                        u8 *mas_rx_payload = &(g_ll_info->rx_pld_buf[g_ll_info->rx_pld_buf_curr_index]);
                        printf("[MAS][Header] : 0x%02X, 0x%02X\n", g_ll_info->rx_hdr_buf[0], g_ll_info->rx_hdr_buf[1]);
                        ARRAYDUMPX3(mas_rx_payload, len);
                    }
                    #endif
                    lc_conn_state_rx_ack_check(mas_dev);
                } break;

                default: {
                } break;
            }
        } break;

        case LC_CONN_STT_DISCONNECTED: {
        } break;

        default: {
            printf("conn_info->state = %d\n", conn_info->state);
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

    lc_mas_state_machine(new_mas_dev, LC_CONN_STT_EVT_JUST_SENT_CONN_IND);

    //Dump_Conn_Info(new_mas_dev);
    DUMPX(new_mas_dev->connection_handle);

    //test
    {
        u16 conn_hdl = new_mas_dev->connection_handle;
        static u8 tx_buf[300];
        static Conn_State_Tx_Request_t tx_request;
        tx_request.tx_request_active = 1;
        tx_request.tx_request_done = 0;
        tx_request.tx_done_len = 0;
        tx_request.tx_len = 300;
        tx_request.tx_buf = tx_buf;
        tx_request.next = NULL;
        for (u32 i=0; i<tx_request.tx_len; i++) {
            tx_request.tx_buf[i] = i / 8;
        }
        int ret = lc_conn_state_tx_request_add_from_upper(conn_hdl, &tx_request);
        BASIC_ASSERT(ret == 0);
        MASTER_DUMP2("============ tx_request_add ============\n");
        lc_conn_state_dump_all_dev();
        //DUMPA(tx_buf);
        //ARRAYDUMPX3(tx_buf, 300);
    }
    //test
    {
        u16 conn_hdl = new_mas_dev->connection_handle;
        static u8 tx_buf[300];
        static Conn_State_Tx_Request_t tx_request;
        tx_request.tx_request_active = 1;
        tx_request.tx_request_done = 0;
        tx_request.tx_done_len = 0;
        tx_request.tx_len = 32;
        tx_request.tx_buf = tx_buf;
        tx_request.next = NULL;
        for (u32 i=0; i<tx_request.tx_len; i++) {
            tx_request.tx_buf[i] = 0xFF - i / 8;
        }
        int ret = lc_conn_state_tx_request_add_from_upper(conn_hdl, &tx_request);
        BASIC_ASSERT(ret == 0);
        MASTER_DUMP2("============ tx_request_add ============\n");
        lc_conn_state_dump_all_dev();
        //DUMPA(tx_buf);
        //ARRAYDUMPX3(tx_buf, 300);
    }
}

#define ___SLAVE______________________________
#define ___SLAVE_____________________________
#define ___SLAVE____________________________

void lc_sla_connection_event_start(Bt_Dev_Info_t *sla_dev)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)sla_dev->infrastructure;

    //why
    LET0T1STOP;
    
    //why again?
    //le_sla_assign_rx_buf(gp_cmn_buf_rx_payload_curr, gp_cmn_buf_rx_payload_left_size);
    
    //why again?
    //le_sla_assign_tx_buf(gp_cmn_buf_tx_payload_curr, TX_TOTAL_SIZE);
    
    DIRECT_RFIELD_DISABLE_TIMEOUT = 0;
    //DIRECT_RFIELD_RX_TIMEOUT_VALUE = 100;//conn_info->window_size_in_us;
    DIRECT_RFIELD_RX_TIMEOUT_VALUE = conn_info->window_size_in_us + conn_info->window_widen_size_in_us;
    T0_SLOT_TIMER_SET(1);
    T0RXENABLE;
    T1TXENABLE_TIFS;
    SET_RF_DEV(sla_dev);
}

void lc_sla_state_machine(Bt_Dev_Info_t *sla_dev, LC_CONNECTION_STATE_EVENT_t evt)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)sla_dev->infrastructure;
#ifdef DFS_SIM_ON
    LC_CONNECTION_STATE_t old_state = conn_info->state;
#endif
    switch (conn_info->state)
    {
        case LC_CONN_STT_ESTABLISHED: {
            switch (evt)
            {
                case LC_CONN_STT_EVT_JUST_RECEIVED_CONN_IND: {
                    u32 sleep_time_in_us = 1250 + (1250 * conn_info->conn_ind_payload.LLData.WinOffset) - CONPENSATE_SCH_TIME_SLAVE + CONPENSATE_SLAVE_DELAY;
                    lc_conn_state_set_timer_for_adding_sch_request(sla_dev, sleep_time_in_us);
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
                    lc_mas_add_request(sla_dev);
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
                    SLAVE_DUMP2(" 1st ENABLE TX & RX_TIFS\n");
                    lc_conn_state_connection_event_init(sla_dev);
                    lc_conn_state_rx_prepare(conn_info);
                    lc_sla_connection_event_start(sla_dev);
                    conn_info->state = LC_CONN_STT_ON_CONNECTION_EVENT;
                } break;

                default: {
                } break;
            }
        } break;

        case LC_CONN_STT_OFF_CONNECTION_EVENT: {
            switch (evt)
            {
                case LC_CONN_STT_EVT_SCH_GRANT: {
                    SLAVE_DUMP2(" xxx ENABLE TX & RX_TIFS, latency_ctr=%d\n", conn_info->latency_ctr);
                    //Skip check : Latency
                    // If there is payload receive ok
                    if (conn_info->rx_ctr) {
                        conn_info->latency_ctr++;
                        if (conn_info->latency_ctr -1 != conn_info->conn_ind_payload.LLData.Latency) {
                            break;
                        } else {
                            conn_info->latency_ctr = 0;
                        }
                    }
                    conn_info->channel = (conn_info->channel + conn_info->conn_ind_payload.LLData.Hop) % 37;
                    lc_conn_state_connection_event_init(sla_dev);
                    lc_conn_state_rx_prepare(conn_info);
                    lc_sla_connection_event_start(sla_dev);
                    conn_info->state = LC_CONN_STT_ON_CONNECTION_EVENT;
                } break;

                default:
                    break;
            }
        } break;

        case LC_CONN_STT_ON_CONNECTION_EVENT: {
            switch (evt)
            {
                case LC_CONN_STT_EVT_SCH_GRANT: {
                    SLAVE_DUMP2(" xxx ENABLE TX & RX_TIFS, latency_ctr=%d\n", conn_info->latency_ctr);
                    //Skip check : Latency
                    // If there is payload receive ok
                    if (conn_info->rx_ctr) {
                        conn_info->latency_ctr++;
                        if (conn_info->latency_ctr -1 != conn_info->conn_ind_payload.LLData.Latency) {
                            break;
                        } else {
                            conn_info->latency_ctr = 0;
                        }
                    }
                    conn_info->channel = (conn_info->channel + conn_info->conn_ind_payload.LLData.Hop) % 37;
                    lc_conn_state_connection_event_init(sla_dev);
                    lc_conn_state_rx_prepare(conn_info);
                    lc_sla_connection_event_start(sla_dev);
                    conn_info->state = LC_CONN_STT_ON_CONNECTION_EVENT;
                } break;

                case LC_CONN_STT_EVT_PL_RCV_ERR:
                case LC_CONN_STT_EVT_RX_TIMEOUT: {
                    SLAVE_DUMP2(" RX_TIMEOUT !!!\n");
                    lc_conn_state_rx_timeout_check(conn_info);
                    conn_info->timeout_ctr--;
                    if (conn_info->timeout_ctr == 0) {
                        SLAVE_DUMP2(" DISCONNECT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                        lc_mas_remove_request(sla_dev);
                        //lc_conn_state_disconnect(mas_dev);
                        conn_info->state = LC_CONN_STT_DISCONNECTED;
                        lc_conn_state_disconnect(sla_dev);
                        return;
                    }
                    //conn_info->state = LC_CONN_STT_OFF_CONNECTION_EVENT;
                } break;

                case LC_CONN_STT_EVT_PL_RCV_OK: {
                    #ifdef DFS_SIM_ON
                    {
                        u32 len = g_ll_info->rx_hdr_buf[1];
                        u8 *sla_rx_payload = &(g_ll_info->rx_pld_buf[g_ll_info->rx_pld_buf_curr_index]);
                        printf("[SLA][Header] : 0x%02X, 0x%02X\n", g_ll_info->rx_hdr_buf[0], g_ll_info->rx_hdr_buf[1]);
                        ARRAYDUMPX3(sla_rx_payload, len);
                    }
                    #endif
                    conn_info->timeout_ctr = (conn_info->conn_ind_payload.LLData.Timeout * 8 / conn_info->conn_ind_payload.LLData.Interval) + 1;
                    lc_conn_state_rx_ack_check(sla_dev);
                    lc_conn_state_tx_prepare_0_sn_update(conn_info);
                    lc_conn_state_tx_prepare_1_data_update(conn_info);
                } break;

                case LC_CONN_STT_EVT_TX_DONE: {
                    //conn_info->state = LC_CONN_STT_OFF_CONNECTION_EVENT;
                } break;

                default: {
                } break;
            }
        } break;

        case LC_CONN_STT_ON_CONNECTION_EVENT_MD: {
        } break;

//        case LC_CONN_STT_DISCONNECTED: {
//        } break;

        default: {
            ASSERT_Reboot(0);
        } break;
    }
#ifdef DFS_SIM_ON
    printf("%s(), old_state:%d, new_state:%d, evt:%d, time:%d\n", __func__, old_state, conn_info->state, evt, SimAir_TimeStamp_Low_Get());
    SLAVE_DUMP2(" state machine old:%d, new:%d, evt:%d\n", \
                old_state, \
                conn_info->state, \
                evt);
#endif
}

void lc_sla_handle_conn_ind(Bt_Dev_Info_t *dev_from_adv)
{
    Bt_Dev_Info_t *new_sla_dev;
    Adv_Connect_Ind_Payload_t *p_conn_ind_from_adv = (Adv_Connect_Ind_Payload_t *)dev_from_adv->infrastructure;

    lc_conn_state_create_dev_by_conn_ind(LE_SLAVE, p_conn_ind_from_adv, &new_sla_dev);
    lc_conn_state_dump_all_dev();

    lc_sla_state_machine(new_sla_dev, LC_CONN_STT_EVT_JUST_RECEIVED_CONN_IND);

    DUMPX(new_sla_dev->connection_handle);
    //Dump_Conn_Info(new_mas_dev);

    //test
    {
        u16 conn_hdl = new_sla_dev->connection_handle;
        static u8 tx_buf[300];
        static Conn_State_Tx_Request_t tx_request;
        tx_request.tx_request_active = 1;
        tx_request.tx_request_done = 0;
        tx_request.tx_done_len = 0;
        tx_request.tx_len = 300;
        tx_request.tx_buf = tx_buf;
        tx_request.next = NULL;
        for (u32 i=0; i<tx_request.tx_len; i++) {
            if (i<tx_request.tx_len/2)
                tx_request.tx_buf[i] = i % 8 + 0x80;
            else
                tx_request.tx_buf[i] = i % 8 + 0x90;
        }
        int ret = lc_conn_state_tx_request_add_from_upper(conn_hdl, &tx_request);
        BASIC_ASSERT(ret == 0);
        SLAVE_DUMP2("============ tx_request_add ============\n");
        lc_conn_state_dump_all_dev();
        //DUMPA(tx_buf);
        //ARRAYDUMPX3(tx_buf, 300);
    }

}

