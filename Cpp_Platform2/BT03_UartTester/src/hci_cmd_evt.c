

#include "Everything_App.hpp"

int _RAW_Send_(UART_Info_t *info, u8 *buf, u32 len)
{
    printf("[%6s][%6s] %s() start\n", info->comName, info->aliasName, __func__);

    return LibUartHdl_Send(info->hdl, buf, len);
}

int _RAW_Receive_(UART_Info_t *info, int autoFree /*= 1*/)
{
    printf("[%6s][%6s] %s() start\n", info->comName, info->aliasName, __func__);

    hci_uart_rx_one_packet(info);
    ARRAYDUMPX2(info->rxPacket->buf, info->rxPacket->len);

    if (autoFree) {
        MM_FREE(info->rxPacket);
    }
    return 0;
}

int _CMD_(UART_Info_t *info, const char *funcStr, u8 *cmd, u32 cmdLen, u16 *answer, u32 answerLen)
{
    printf("[%6s][%6s] %s() start\n", info->comName, info->aliasName, funcStr);

    LibUartHdl_Send(info->hdl, cmd, cmdLen);

    while (1)
    {
        hci_uart_rx_one_packet(info);
        if (LibComm_ArrayCompare(answer, answerLen, info->rxPacket->buf, info->rxPacket->len)) {
            printf("Drop unknown array:\n");
            ARRAYDUMPX2(info->rxPacket->buf, info->rxPacket->len);
        } else {
            printf("[%6s][%6s] %s ok\n", info->comName, info->aliasName, funcStr);
            MM_FREE(info->rxPacket);
            break;
        }
        MM_FREE(info->rxPacket);
    }
    return 0;
}

int _EVENT_(UART_Info_t *info, const char *funcStr, u16 *answer, u32 answerLen, int isAutoFree)
{
    printf("[%6s][%6s] %s() start\n", info->comName, info->aliasName, funcStr);

    while (1)
    {
        hci_uart_rx_one_packet(info);
        if (LibComm_ArrayCompare(answer, answerLen, info->rxPacket->buf, info->rxPacket->len)) {
            printf("Drop unknown array:\n");
            ARRAYDUMPX2_VERBOS(info->rxPacket->buf, info->rxPacket->len);
        } else {
            printf("[%6s][%6s] %s ok\n", info->comName, info->aliasName, funcStr);
            if (isAutoFree) {
                MM_FREE(info->rxPacket);
            }
            break;
        }
        MM_FREE(info->rxPacket);
    }
    return 0;
}

int CMD_RESET(UART_Info_t *info)
{
    u8 cmd[] = {0x01, 0x03, 0x0C, 0x00};
    u16 answer[] = {0x04, 0x0E, 0x04, 0x01, 0x03, 0x0C, 0x00};

    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}

int CMD_RESET2(UART_Info_t *info)
{
    u8 cmd[] = {0x01, 0x03, 0x0C, 0x00};
    u16 answer[] = {0x04, 0x0E, 0x04, 0x01, 0x03, 0x0C, 0x00};

    LibUartHdl_Send(info->hdl, cmd, sizeof(cmd));
    LibOs_SleepMiliSeconds(500);
    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}


int CMD_Set_Event_Mask(UART_Info_t *info)
{
    u8 cmd[] = {0x01, 0x01, 0x0c, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20}; //set event mask
    u16 answer[] = {0x04, 0x0E, 0x04, 0x01, 0x01, 0x0C, 0x00};

    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}

int CMD_LE_Set_Event_Mask(UART_Info_t *info)
{
    u8 cmd[] = {0x01, 0x01, 0x20, 0x08, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //set LE event mask
    u16 answer[] = {0x04, 0x0E, 0x04, 0x01, 0x01, 0x20, 0x00};

    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}

int CMD_LE_Write_Advertising_Parameters(UART_Info_t *info)
{
    u8 cmd[] = {0x01, 0x06, 0x20, 0x0f, 0x25, 0x00, 0x25, 0x00, 0x00, 0x00, 0x00, 0x88, 0x11, 0x55, 0x67, 0x11, 0x77, 0x07, 0x00};
    u16 answer[] = {0x04, 0x0E, 0x04, 0x01, 0x06, 0x20, 0x00};

    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}

int CMD_LE_Write_Advertise_Enable(UART_Info_t *info)
{
    u8 cmd[] = {0x01, 0x0A, 0x20, 0x01, 0x01};
    u16 answer[] = {0x04, 0x0E, 0x04, 0x01, 0x0A, 0x20, 0x00};

    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}

int CMD_LE_Create_Connection(UART_Info_t *info)
{
    u8 cmd[] = {0x01, 0x0d, 0x20, 0x19, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x13, 0x93, 0x93, 0x67, 0x11, 0xf5, 0x00, (u8)gCONN_INTERVAL, (u8)(gCONN_INTERVAL>>8), (u8)gCONN_INTERVAL, (u8)(gCONN_INTERVAL>>8), (u8)gCONN_LATENCY, (u8)(gCONN_LATENCY>>8), (u8)gCONN_TIMEOUT, (u8)(gCONN_TIMEOUT>>8), (u8)gCONN_MIN_CE_LEN, (u8)(gCONN_MIN_CE_LEN>>8), (u8)gCONN_MAX_CE_LEN, (u8)(gCONN_MAX_CE_LEN>>8)};
    u16 answer[] = {0x04, 0x0F, 0x04, 0x00, 0x01, 0x0D, 0x20};

    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}

int CMD_LE_Create_Connection_WithBdAddr(UART_Info_t *info, u8 *bd_addr)
{
    u8 cmd[] = {0x01, 0x0d, 0x20, 0x19, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5], 0x00, (u8)gCONN_INTERVAL, (u8)(gCONN_INTERVAL>>8), (u8)gCONN_INTERVAL, (u8)(gCONN_INTERVAL>>8), (u8)gCONN_LATENCY, (u8)(gCONN_LATENCY>>8), (u8)gCONN_TIMEOUT, (u8)(gCONN_TIMEOUT>>8), (u8)gCONN_MIN_CE_LEN, (u8)(gCONN_MIN_CE_LEN>>8), (u8)gCONN_MAX_CE_LEN, (u8)(gCONN_MAX_CE_LEN>>8)};
    u16 answer[] = {0x04, 0x0F, 0x04, 0x00, 0x01, 0x0D, 0x20};

    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}


int CMD_Disconnect(UART_Info_t *info, u16 connHdl, u8 reason)
{
    u8 cmd[] = {0x01, 0x06, 0x04, 0x03, (u8)connHdl, (u8)(connHdl>>8), 0x03};
    u16 answer[] = {0x04, 0x0F, 0x04, 0x00, 0x01, 0x06, 0x04};

    cmd[4] = (u8)connHdl;
    cmd[5] = (u8)connHdl>>8;
    cmd[6] = reason;
    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}

int CMD_Disconnect_NoWait(UART_Info_t *info, u16 connHdl, u8 reason)
{
    u8 cmd[] = {0x01, 0x06, 0x04, 0x03, 0x71, 0x00, 0x03};

    cmd[4] = (u8)connHdl;
    cmd[5] = (u8)connHdl>>8;
    cmd[6] = reason;

    LibUartHdl_Send(info->hdl, cmd, sizeof(cmd));
    return 0;
}

int CMD_VENDOR_SetBdAddress(UART_Info_t *info, u8 *addr)
{
    u8 cmd[] = {0x01, 0x0d, 0xfc, 0x06, 0xfe, 0x95, 0x95, 0x65, 0x15, 0x05};
    u16 answer[] = {0x04, 0x0E, 0x04, 0x01, 0x0D, 0xFC, 0x00};

    //ARRAYDUMPX2(addr, 6);
    memcpy((char *)&(cmd[4]), (const char *)addr, 6);
    //ARRAYDUMPX2(cmd, sizeof(cmd));
    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}

/*
All_PHYs
0 The Host has no preference among the transmitter PHYs supported by the Controller
1 The Host has no preference among the receiver PHYs supported by the Controller
All other bits - Reserved for future use

TX_PHYs: 
0 The Host prefers to use the LE 1M transmitter PHY (possibly among others) 
1 The Host prefers to use the LE 2M transmitter PHY (possibly among others) 
2 The Host prefers to use the LE Coded transmitter PHY (possibly among others)

RX_PHYs:
0 The Host prefers to use the LE 1M receiver PHY (possibly among others) 
1 The Host prefers to use the LE 2M receiver PHY (possibly among others) 
2 The Host prefers to use the LE Coded receiver PHY (possibly among others) 

PHY_Options:
0 to 1
    0 = the Host has no preferred coding when transmitting on the LE Coded PHY
    1 = the Host prefers that S=2 coding be used when transmitting on the LE Coded PHY
    2 = the Host prefers that S=8 coding be used when transmitting on the LE Coded PHY
    3 = Reserved for future use
*/
int CMD_LE_Set_PHY(UART_Info_t *info, u16 connHdl, u8 all_phys, u8 tx_phys, u8 rx_phys, u16 phy_options)
{
    u8 cmd[] = {0x01, 0x32, 0x20, 0x07, (u8)connHdl, (u8)(connHdl>>8), all_phys, tx_phys, rx_phys, (u8)phy_options, (u8)(phy_options>>8)};
    u16 answer[] = {0x04, 0x0F, 0x04, 0x00, 0x01, 0x32, 0x20};


    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}

int EVT_LE_Connection_Complete(UART_Info_t *info, u16 *outConnHdl)
{
    int ret;
    u16 answer[] = {0x04, 0x3E, 0x13, 0x01, 0x00, 0x80CC, 0x80CC, 0x8000, 0x8000, 0x80FF, 0x8093, 0x8093, 0x8067, 0x8011, 0x8000, (u8)gCONN_INTERVAL, (u8)(gCONN_INTERVAL>>8), (u8)gCONN_LATENCY, (u8)(gCONN_LATENCY>>8), (u8)gCONN_TIMEOUT, (u8)(gCONN_TIMEOUT>>8), 0x00};

    ret = _EVENT_(info, __func__, answer, sizeof(answer)/sizeof(u16), 0);

    *outConnHdl = info->rxPacket->buf[6];
    *outConnHdl = *outConnHdl << 8;
    *outConnHdl += info->rxPacket->buf[5];
    *outConnHdl = *outConnHdl & 0x0FFF;

    MM_FREE(info->rxPacket);
    return ret;
}

int EVT_LE_Enhanced_Connection_Complete(UART_Info_t *info, u16 *outConnHdl)
{
    int ret;
    u16 answer[] = {0x04, 0x3E, 0x1F, 0x0A, 0x00, 0x8000, 0x8000, 0x8000, 0x00, 0x80FF, 0x8093, 0x8093, 0x8067, 0x8011, 0x80FF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (u8)gCONN_INTERVAL, (u8)(gCONN_INTERVAL>>8), 0x8000, 0x8000, 0x8018, 0x8000, 0x8007};

    ret = _EVENT_(info, __func__, answer, sizeof(answer)/sizeof(u16), 0);

    *outConnHdl = info->rxPacket->buf[6];
    *outConnHdl = *outConnHdl << 8;
    *outConnHdl += info->rxPacket->buf[5];
    *outConnHdl = *outConnHdl & 0x0FFF;

    MM_FREE(info->rxPacket);
    return ret;
}

/*
TX_PHY: 
0x01 The transmitter PHY for the connection is LE 1M 
0x02 The transmitter PHY for the connection is LE 2M
0x03 The transmitter PHY for the connection is LE Coded
RX_PHY: 
0x01 The receiver PHY for the connection is LE 1M
0x02 The receiver PHY for the connection is LE 2M
0x03 The receiver PHY for the connection is LE Coded
*/
int EVT_LE_PHY_Update_Complete_Event(UART_Info_t *info, u16 connHdl)
{
    const char *phyStr[] = {"", "1M", "2M", "Coded"};
    int ret;
    u16 answer[] = {0x04, 0x3E, 0x06, 0x0C, 0x00, (u16)((u8)connHdl), (u16)(connHdl>>8), 0x8000, 0x8000};

    ret = _EVENT_(info, __func__, answer, sizeof(answer)/sizeof(u16), 0);

    BASIC_ASSERT(info->rxPacket->buf[7] <= 0x03);
    BASIC_ASSERT(info->rxPacket->buf[8] <= 0x03);
    printf("TX_PHY=%s, RX_PHY=%s\n", phyStr[info->rxPacket->buf[7]], phyStr[info->rxPacket->buf[8]]);

    MM_FREE(info->rxPacket);
    return ret;
}

int EVT_Disconnect_Complete(UART_Info_t *info, u16 connHdl, u16 reasonAnswer)
{
    int ret;
    u16 answer[] = {0x04, 0x05, 0x04, 0x00, 0x8000, 0x8000, 0x8000};

    answer[4] = (u8)connHdl;
    answer[5] = (u8)(connHdl>>8);
    answer[6] = reasonAnswer;

    ret = _EVENT_(info, __func__, answer, sizeof(answer)/sizeof(u16), 0);

    MM_FREE(info->rxPacket);
    return ret;
}

