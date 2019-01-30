#ifdef DFS_SIM_ON
  #include "Everything_App.hpp"
  extern void MAS_PHY_API_Timer_0(Bt_Dev_Info_t *mas_dev, u32 sleep_time_in_us);
  #define controller_timer_insert(dev,time,q,cmd,len,data) MAS_PHY_API_Timer_0((Bt_Dev_Info_t *)(dev),time)

  #define PTR_TO_U32(a) (u8 *)(a)
  extern u8 *MAS__RXLE_PLH_SP;
  extern u8 *MAS__RXLE_PLH_CP;
  extern u8 *MAS__RXLE_PLH_EP;
  #define DIRECT_RFIELD_AGU_RXLE_PLH_SP MAS__RXLE_PLH_SP
  #define DIRECT_RFIELD_AGU_RXLE_PLH_CP MAS__RXLE_PLH_CP
  #define DIRECT_RFIELD_AGU_RXLE_PLH_EP MAS__RXLE_PLH_EP
  extern u8 *MAS__RXLE_PLD_SP;
  extern u8 *MAS__RXLE_PLD_CP;
  extern u8 *MAS__RXLE_PLD_EP;
  #define DIRECT_RFIELD_AGU_RXLE_PLD_SP MAS__RXLE_PLD_SP
  #define DIRECT_RFIELD_AGU_RXLE_PLD_CP MAS__RXLE_PLD_CP
  #define DIRECT_RFIELD_AGU_RXLE_PLD_EP MAS__RXLE_PLD_EP
  extern u8 *MAS__TXLE_PLH_SP;
  extern u8 *MAS__TXLE_PLH_CP_RE;
  extern u8 *MAS__TXLE_PLH_EP;
  #define DIRECT_RFIELD_AGU_TXLE_PLH_SP MAS__TXLE_PLH_SP
  #define DIRECT_RFIELD_AGU_TXLE_PLH_CP_RE MAS__TXLE_PLH_CP_RE
  #define DIRECT_RFIELD_AGU_TXLE_PLH_EP MAS__TXLE_PLH_EP
  extern u8 *MAS__TXLE_PLD_SP;
  extern u8 *MAS__TXLE_PLD_CP_RE;
  extern u8 *MAS__TXLE_PLD_EP;
  #define DIRECT_RFIELD_AGU_TXLE_PLD_SP MAS__TXLE_PLD_SP
  #define DIRECT_RFIELD_AGU_TXLE_PLD_CP_RE MAS__TXLE_PLD_CP_RE
  #define DIRECT_RFIELD_AGU_TXLE_PLD_EP MAS__TXLE_PLD_EP

#else //#ifdef DFS_SIM_ON
  #define POINTER_TO_U32(a) (u32)(a)
  #define PTR_TO_U32(a) (u32)(a)
#endif //#ifdef DFS_SIM_ON

static void lc_conn_state_assign_rx_buf(u8 *rx_plh, u32 rx_plh_len, u8 *rx_pld, u32 rx_pld_len)
{
    DIRECT_RFIELD_AGU_RXLE_PLH_SP = PTR_TO_U32(&(rx_plh[0]));
    DIRECT_RFIELD_AGU_RXLE_PLH_CP = PTR_TO_U32(&(rx_plh[0]));
    DIRECT_RFIELD_AGU_RXLE_PLH_EP = PTR_TO_U32(&(rx_plh[rx_plh_len]));

    DIRECT_RFIELD_AGU_RXLE_PLD_SP = PTR_TO_U32(&(rx_pld[0]));
    DIRECT_RFIELD_AGU_RXLE_PLD_CP = PTR_TO_U32(&(rx_pld[0]));
    DIRECT_RFIELD_AGU_RXLE_PLD_EP = PTR_TO_U32(&(rx_pld[rx_pld_len]));
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
static u8 *g_tx_plh_cmn_buf_dummy = NULL;
static u8 *g_tx_pld_cmn_buf_dummy = NULL;
#define LC_CONN_STATE_TX_BUF_DUMMY_LEN 10
static u8 *g_rx_plh_cmn_buf = NULL;
static u8 *g_rx_pld_cmn_buf = NULL;
#define LC_CONN_STATE_RX_BUF_LEN 4096
static u32 g_rx_pld_cmn_buf_curr_index = 0; //update in payload rx ok, turn around in the start of rf prepare
void lc_conn_state_init(void)
{
#ifdef DFS_SIM_ON
    #define pvPortMalloc_ext malloc
#else
    extern void *pvPortMalloc_ext(u32 xWantedSize);
#endif
    if (g_tx_plh_cmn_buf_dummy == NULL)
        g_tx_plh_cmn_buf_dummy = (u8 *)pvPortMalloc_ext(12);

    if (g_tx_pld_cmn_buf_dummy == NULL)
        g_tx_pld_cmn_buf_dummy = (u8 *)pvPortMalloc_ext(LC_CONN_STATE_TX_BUF_DUMMY_LEN);

    if (g_rx_plh_cmn_buf == NULL)
        g_rx_plh_cmn_buf = (u8 *)pvPortMalloc_ext(12);

    if (g_rx_pld_cmn_buf == NULL)
        g_rx_pld_cmn_buf = (u8 *)pvPortMalloc_ext(LC_CONN_STATE_RX_BUF_LEN);

}

void lc_conn_state_rf_buf_set(Bt_Dev_Info_t *dev)
{
    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)dev->infrastructure;

    //Tx setting
    u8 tx_llid, tx_nesn, tx_sn, tx_md, tx_len;
    tx_sn = conn_info->sn;
    tx_nesn = conn_info->nesn;
    if (conn_info->tx_request != NULL) {
        tx_len = conn_info->tx_request->tx_len;
    }
    else {
        tx_len = 0;
        tx_llid = 1; //01b = LL Data PDU: Continuation fragment of an L2CAP message, or an Empty PDU
    }
    //g_tx_plh_cmn_buf_dummy = 
    lc_conn_state_assign_tx_buf(g_tx_plh_cmn_buf_dummy, 2, g_tx_pld_cmn_buf_dummy, LC_CONN_STATE_TX_BUF_DUMMY_LEN);

    //Rx setting
    if (g_rx_pld_cmn_buf_curr_index == 0) {
        lc_conn_state_assign_rx_buf(g_rx_plh_cmn_buf, 2, g_rx_pld_cmn_buf, LC_CONN_STATE_RX_BUF_LEN);
    }
    else {
        // turn around when payload buffer is small than 255
        if (g_rx_pld_cmn_buf_curr_index + 255 > LC_CONN_STATE_RX_BUF_LEN) {
            g_rx_pld_cmn_buf_curr_index = 0;
            lc_conn_state_assign_rx_buf(g_rx_plh_cmn_buf, 2, g_rx_pld_cmn_buf, LC_CONN_STATE_RX_BUF_LEN);
        }
        else {
            //do nothing, let rf use left buffer
        }
    }
}

Bt_Dev_Info_t *g_conn_state_dev_head = NULL;
Bt_Dev_Info_t *g_conn_state_dev_tail = NULL;
static void lc_conn_state_push_dev_to_list_tail(Bt_Dev_Info_t *dev)
{
    if (g_conn_state_dev_head == NULL)
    {
        g_conn_state_dev_head = dev;
        g_conn_state_dev_tail = dev;
    }
    else
    {
        Conn_State_Info_t *old_tail_conn_info = (Conn_State_Info_t *)g_conn_state_dev_tail->infrastructure;
        Conn_State_Info_t *curr_conn_info = (Conn_State_Info_t *)dev->infrastructure;

        old_tail_conn_info->next_dev = dev;
        curr_conn_info->prev_dev = g_conn_state_dev_tail;

        g_conn_state_dev_tail = dev;
    }
}

static void lc_conn_state_delete_dev_from_list(Bt_Dev_Info_t *dev)
{
    Bt_Dev_Info_t *curr_dev = g_conn_state_dev_head;
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
            if (curr_dev == g_conn_state_dev_head) {
                g_conn_state_dev_head = curr_conn_info->next_dev;
            }
            if (curr_dev == g_conn_state_dev_tail) {
                g_conn_state_dev_tail = curr_conn_info->prev_dev;
            }
        }
        else {
            curr_dev = curr_conn_info->next_dev;
        }
    }
}

static u16 lc_conn_state_calculate_new_conn_handle(void)
{
    static u16 new_conn_hdl = 0x70;
    Bt_Dev_Info_t *curr_dev = g_conn_state_dev_head;

    new_conn_hdl = new_conn_hdl >= 0x0FFF ? 0 : 1+new_conn_hdl; //so first conn hdl is 0x71
    while(curr_dev != NULL)
    {
        Conn_State_Info_t *curr_conn_info = (Conn_State_Info_t *)curr_dev->infrastructure;
        if (new_conn_hdl == curr_conn_info->conn_hdl) {
            new_conn_hdl = new_conn_hdl >= 0x0FFF ? 0 : 1+new_conn_hdl;
            curr_dev = g_conn_state_dev_head;
        }
        else {
            curr_dev = curr_conn_info->next_dev;
        }
    }
    return new_conn_hdl;
}

static Bt_Dev_Info_t * lc_conn_state_get_dev_by_conn_handle(u16 conn_hdl)
{
    Bt_Dev_Info_t *curr_dev = g_conn_state_dev_head;
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

    DUMPD(in_conn_ind->LLData.Interval);
    DUMPD(in_conn_ind->LLData.WinOffset);

    //copy to new dev
    new_conn_info->conn_ind_payload = *in_conn_ind;

    //20180116_IJ: WBNF-63 transmitWindowSize shall be at least 1.25ms
    if(new_conn_info->conn_ind_payload.LLData.WinSize == 0x00)
    {
       new_conn_info->conn_ind_payload.LLData.WinSize = 0x01;//at least 1.25ms
    }

    new_conn_info->sn = 0;
    new_conn_info->nesn = 0;
    new_conn_info->conn_hdl = lc_conn_state_calculate_new_conn_handle();
    new_conn_info->state = LC_CONN_STT_ESTABLISHED;
    new_conn_info->prev_dev = NULL;
    new_conn_info->next_dev = NULL;
    new_conn_info->remain_tx_len = 0;
    new_conn_info->tx_request = NULL;

    new_dev->connection_handle = new_conn_info->conn_hdl; //for upper

#if 0
    gCurrBtDev = p_bt_dev;
    gCurrConnInfo = (Conn_Info_t*)(p_bt_dev->infrastructure);
    {
        //Copy connection indication packet payload from advertiser to slave
        Adv_Dev_t                   *p_adv_dev = (Adv_Dev_t*)dev_from_adv->infrastructure;
        Adv_Connect_Ind_Payload_t   *p_conn_ind_payload = (Adv_Connect_Ind_Payload_t *)p_adv_dev->p_rx_pdu_payload;
        gCurrConnInfo->conn_ind_payload = *p_conn_ind_payload;

        //Determine connection role
        gCurrConnInfo->Role = LE_ROLE_SLAVE;

        //20180116_IJ: WBNF-63 transmitWindowSize shall be at least 1.25ms
        if(gCurrConnInfo->conn_ind_payload.LLData.WinSize == 0x00)
        {
           gCurrConnInfo->conn_ind_payload.LLData.WinSize = 0x01;//at least 1.25ms
        }

        gCurrConnInfo->conn_hdl = 0x81;
        gCurrConnInfo->sn = 0xFF;
        gCurrConnInfo->nesn = 0xFF;
    }
#endif

    lc_conn_state_push_dev_to_list_tail(new_dev);
    *out_new_dev = new_dev;
    return  0;
}

static void lc_conn_state_dump_all_dev(void)
{
#ifdef DFS_SIM_ON
    u32 i = 0;
    Bt_Dev_Info_t *curr_dev = g_conn_state_dev_head;
    while(curr_dev != NULL)
    {
        Conn_State_Info_t *curr_conn_info = (Conn_State_Info_t *)curr_dev->infrastructure;
        MAS_INT_DUMP2(" -- [[ conn state dev dump %d ]]\n", i);
        MAS_INT_DUMP2(" -- Type:%d, Role:%d, Up_Conn_Hdl:0x%X\n", curr_dev->Type, curr_dev->Role, curr_dev->connection_handle);
        MAS_INT_DUMP2(" -- sn:%d, nesn:%d, conn_hdl:0x%X, state:%d\n", curr_conn_info->sn, curr_conn_info->nesn, curr_conn_info->conn_hdl, curr_conn_info->state);
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
        MAS_INT_DUMP2(" -- -- LLData.CRCInit :0x%X, 0x%X, 0x%X, 0x%X\n", \
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

        MAS_INT_DUMP2(" -- -- -- remain_tx_len:%d\n", curr_conn_info->remain_tx_len);
        u32 j = 0;
        MAS_INT_DUMP2(" -- -- -- [[ tx_request dump start : %d ]]\n", j);
        Conn_State_Tx_Request_From_Upper_t *curr_tx_request = curr_conn_info->tx_request;
        while (curr_tx_request != NULL) {
            MAS_INT_DUMP2(" -- -- -- tx_request_active:%d  tx_request_done:%d  tx_len:%d\n",
                          curr_tx_request->tx_request_active, \
                          curr_tx_request->tx_request_done, \
                          curr_tx_request->tx_len);
            curr_tx_request = (Conn_State_Tx_Request_From_Upper_t *)curr_tx_request->lc_hdl;
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

#define CONPENSATE_SCH_TIME 350
#define CONPENSATE_MASTER_DELAY 0

void lc_mas_set_timer_for_adding_request(Bt_Dev_Info_t *mas_dev)
{
    u32 cmd;
    cmd = ( (LC_GROUP_EVENT)                           //Group
           |(MESSAGE_SUBGROUP_LE << SUBGROUP_OFFSET)   //Sub-group
           |(LE_LC_MASTER << LE_MODULE_OFFSET)         //Module
           |(LC_CONN_STT_EVT_SLEEP_TIMESUP) );;        //Message
    cmd = cmd;

    Conn_State_Info_t *conn_info = (Conn_State_Info_t *)mas_dev->infrastructure;

    u32 sleep_time_in_us = 1250 + (1250 * conn_info->conn_ind_payload.LLData.WinOffset) - CONPENSATE_SCH_TIME + CONPENSATE_MASTER_DELAY;

    controller_timer_insert((void*)mas_dev, sleep_time_in_us , LC_TASK_QUEUE, cmd, 0, NULL);
}

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
        extern void MAS_PHY_API_Sch_0_Add_Request(Scheduler_Request_T *p_sch_requ);
        MAS_PHY_API_Sch_0_Add_Request(p_sch_requ);
    }
#else
    value = (u32)p_requ_addr_list;
    Controller_TaskCmd(NULL, (RC_GROUP_SCHEDULER | ADD_REQUEST), 4, (u8*)&value, RC_TASK_QUEUE, 0);
#endif
}

void lc_mas_init_connection_event(Bt_Dev_Info_t *mas_dev)
{
#ifdef PLATFORM
    #if (PLATFORM == PLATFORM_CHIMERA)
        ISR_INT00_TABLE[IRQ_LE_PL_RCV_OK].isr_entry = LE_ISR_SLA_LE_PL_RCV_OK_Handler;
        ISR_INT00_TABLE[IRQ_LE_PL_RCV_ERR].isr_entry = LE_ISR_SLA_LE_PL_RCV_ERR_Handler;
        ISR_INT00_TABLE[IRQ_LE_CORREL_OK_8MHZ].isr_entry = LE_ISR_SLA_LE_CORREL_OK_8MHZ_Handler;
        ISR_INT00_TABLE[IRQ_LE_CORREL_ERR].isr_entry = LE_ISR_SLA_LE_CORREL_ERR_Handler;
    #endif
#endif
    lc_conn_state_rf_buf_set(mas_dev);

#ifdef DFS_SIM_ON
//{
  //extern void MAS_PHY_API_Sch_0_Add_Request(Bt_Dev_Info_t *mas_dev, Scheduler_Request_T *p_sch_requ);
  //MAS_PHY_API_Sch_0_Add_Request(mas_dev, p_sch_requ);
//}
#else
//value = (u32)p_requ_addr_list;
//Controller_TaskCmd(NULL, (RC_GROUP_SCHEDULER | ADD_REQUEST), 4, (u8*)&value, RC_TASK_QUEUE, 0);
#endif
}

void lc_mas_start_connection_event(Bt_Dev_Info_t *mas_dev)
{
#ifdef DFS_SIM_ON
//{
  //extern void MAS_PHY_API_Sch_0_Add_Request(Bt_Dev_Info_t *mas_dev, Scheduler_Request_T *p_sch_requ);
  //MAS_PHY_API_Sch_0_Add_Request(mas_dev, p_sch_requ);
//}
#else
//value = (u32)p_requ_addr_list;
//Controller_TaskCmd(NULL, (RC_GROUP_SCHEDULER | ADD_REQUEST), 4, (u8*)&value, RC_TASK_QUEUE, 0);
#endif
}

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
                    lc_mas_set_timer_for_adding_request(mas_dev);
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
                    MASTER_DUMP2(" ENABLE TX & RX_TIFS\n");
                    lc_mas_init_connection_event(mas_dev);
                    lc_mas_start_connection_event(mas_dev);
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

static Adv_Connect_Ind_Payload_t *lc_mas_extract_conn_ind_from_ini(Bt_Dev_Info_t *dev)
{
#ifdef DFS_SIM_ON
    return (Adv_Connect_Ind_Payload_t *)dev->infrastructure;
#else //#ifdef DFS_SIM_ON
    TODO
#endif //#ifdef DFS_SIM_ON
}

void lc_mas_handle_conn_ind(Bt_Dev_Info_t *dev_from_ini)
{
    Bt_Dev_Info_t *new_mas_dev;
    Adv_Connect_Ind_Payload_t *p_conn_ind_from_ini = lc_mas_extract_conn_ind_from_ini(dev_from_ini);

    lc_conn_state_create_dev_by_conn_ind(LE_MASTER, p_conn_ind_from_ini, &new_mas_dev);
    lc_conn_state_dump_all_dev();

    lc_mas_conn_state_machine(new_mas_dev, LC_CONN_STT_EVT_JUST_SENT_CONN_IND);

    //Dump_Conn_Info(new_mas_dev);
}
