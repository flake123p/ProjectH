

#include "Everything_App.hpp"

int _CMD_(UART_Info_t *info, const char *funcStr, u8 *cmd, u32 cmdLen, u16 *answer, u32 answerLen)
{
    printf("[%6s][%6s] %s start\n", info->comName, info->aliasName, funcStr);

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
    printf("[%6s][%6s] %s start\n", info->comName, info->aliasName, funcStr);

    while (1)
    {
        hci_uart_rx_one_packet(info);
        if (LibComm_ArrayCompare(answer, answerLen, info->rxPacket->buf, info->rxPacket->len)) {
            printf("Drop unknown array:\n");
            ARRAYDUMPX2(info->rxPacket->buf, info->rxPacket->len);
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
    u8 cmd[] = {0x01, 0x01, 0x0c, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20}; //set event mask
    u16 answer[] = {0x04, 0x0E, 0x04, 0x01, 0x01, 0x0C, 0x00};

    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}

int CMD_LE_Set_Event_Mask(UART_Info_t *info)
{
    u8 cmd[] = {0x01, 0x01, 0x20, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //set LE event mask
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
    u8 cmd[] = {0x01, 0x0d, 0x20, 0x19, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0xff, 0x93, 0x93, 0x67, 0x11, 0x00, 0x00, (u8)gCONN_INTERVAL, (u8)(gCONN_INTERVAL>>8), (u8)gCONN_INTERVAL, (u8)(gCONN_INTERVAL>>8), (u8)gCONN_LATENCY, (u8)(gCONN_LATENCY>>8), (u8)gCONN_TIMEOUT, (u8)(gCONN_TIMEOUT>>8), (u8)gCONN_MIN_CE_LEN, (u8)(gCONN_MIN_CE_LEN>>8), (u8)gCONN_MAX_CE_LEN, (u8)(gCONN_MAX_CE_LEN>>8)};
    u16 answer[] = {0x04, 0x0F, 0x04, 0x00, 0x01, 0x0D, 0x20};

    return _CMD_(info, __func__, cmd, sizeof(cmd), answer, sizeof(answer)/sizeof(u16));
}

int CMD_Disconnect(UART_Info_t *info, u16 connHdl, u8 reason)
{
    u8 cmd[] = {0x01, 0x06, 0x04, 0x03, 0x71, 0x00, 0x03};
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

    LibString_Copy((char *)&(cmd[4]), (const char *)addr, 6);
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

