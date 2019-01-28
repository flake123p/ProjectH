#ifdef DFS_SIM_ON
#include "Everything_App.hpp"
#else //#ifdef DFS_SIM_ON

#endif //#ifdef DFS_SIM_ON

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
    new_conn_info->conn_hdl = 0x71;
    new_conn_info->state = LC_CONN_STT_ESTABLISHED;

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

    *out_new_dev = new_dev;
    return  0;
}

