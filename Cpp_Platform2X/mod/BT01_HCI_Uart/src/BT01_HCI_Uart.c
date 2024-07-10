
#include "Everything_BT01_HCI_Uart.hpp"

int BT01_HCI_Uart_Demo(void)
{
    printf("This is %s()!!\n", __func__);

    return 0;
}

// return number of rest packets in 1 uart rx
u32 hci_uart_rx_one_packet(HciUartInfo_t *uartInfo)
{
    int isAtLastOnePacket;
    HCI_Uart_Packet_t *packet;
    u32 rxDoneLen;
    HCI_Uart_t *hciUart = &(uartInfo->hciUart);

    if (hciUart->restNum) {
        hciUart->restNum--;
        if (DLLIST_IS_NOT_EMPTY(&(hciUart->restHead))) {
            packet = (HCI_Uart_Packet_t *)DLLIST_FIRST(&(hciUart->restHead));
            DLLIST_REMOVE_FIRST(&(hciUart->restHead));
        } else {
            BASIC_ASSERT(0);
        }
        uartInfo->rxPacket = packet;
        return hciUart->restNum;
    }

    isAtLastOnePacket = 0;
    while (isAtLastOnePacket == 0)
    {
        LibUartHdl_ReceiveEx(uartInfo->hdl, hciUart->buf, &rxDoneLen, HCI_UART_RX_BUF_LEN);
        FOREACH_I(rxDoneLen)
        {
            switch (hciUart->state)
            {
                case WAIT_HEADER: {
                    u8 header = hciUart->buf[0];
                    BASIC_ASSERT(hciUart->currPkt == NULL);
                    hciUart->currPkt = (HCI_Uart_Packet_t *)MM_ALLOC(sizeof(HCI_Uart_Packet_t));
                    hciUart->lenCtr = 0;
                    hciUart->lenMax = 0xFFFFFFFF;
                    hciUart->currPkt->buf[0] = header;
                    hciUart->lenCtr++;
                    hciUart->currPkt->len = hciUart->lenCtr;

                    if (header == 0x04) {
                        hciUart->state = WAIT_EVT_DATA_DONE;
                    } else if (header == 0x02) {
                        hciUart->state = WAIT_ACL_DATA_DONE;
                    } else if (header == 0x05) {
                        hciUart->state = WAIT_MY_DATA_DONE;
                    } else {
                        //do nothing, drop this byte;
                        MM_FREE(hciUart->currPkt);
                        hciUart->currPkt = NULL;
                    }
                } break;

                case WAIT_MY_DATA_DONE: {
                    hciUart->currPkt->buf[hciUart->lenCtr] = hciUart->buf[i];
                    hciUart->lenCtr++;
                    hciUart->currPkt->len = hciUart->lenCtr;
                    if (hciUart->lenCtr == 10) {
                        isAtLastOnePacket = 1;
                        DLLIST_INSERT_LAST(&(hciUart->restHead), hciUart->currPkt);
                        hciUart->currPkt = NULL;
                        hciUart->restNum++;
                        hciUart->state = WAIT_HEADER;
                    }
                } break;

                case WAIT_EVT_DATA_DONE: {
                    hciUart->currPkt->buf[hciUart->lenCtr] = hciUart->buf[i];
                    hciUart->lenCtr++;
                    hciUart->currPkt->len = hciUart->lenCtr;
                    if (hciUart->lenCtr == 3) {
                        hciUart->lenMax = hciUart->currPkt->buf[2];
                    }
                    if (hciUart->lenMax != 0xFFFFFFFF) {
                        if (hciUart->lenCtr == hciUart->lenMax+3) {
                            isAtLastOnePacket = 1;
                            DLLIST_INSERT_LAST(&(hciUart->restHead), hciUart->currPkt);
                            hciUart->currPkt = NULL;
                            hciUart->restNum++;
                            hciUart->state = WAIT_HEADER;
                        }
                    }
                } break;

                case WAIT_ACL_DATA_DONE: {
                    hciUart->currPkt->buf[hciUart->lenCtr] = hciUart->buf[i];
                    hciUart->lenCtr++;
                    hciUart->currPkt->len = hciUart->lenCtr;
                    if (hciUart->lenCtr == 5) {
                        hciUart->lenMax = hciUart->currPkt->buf[4];
                        hciUart->lenMax = hciUart->lenMax << 8;
                        hciUart->lenMax += hciUart->currPkt->buf[3];
                    }
                    if (hciUart->lenMax != 0xFFFFFFFF) {
                        if (hciUart->lenCtr == hciUart->lenMax+5) {
                            isAtLastOnePacket = 1;
                            DLLIST_INSERT_LAST(&(hciUart->restHead), hciUart->currPkt);
                            hciUart->currPkt = NULL;
                            hciUart->restNum++;
                            hciUart->state = WAIT_HEADER;
                        }
                    }
                } break;
            }
        }
    }
    //send out complete one
    if (hciUart->restNum) {
        hciUart->restNum--;
        if (DLLIST_IS_NOT_EMPTY(&(hciUart->restHead))) {
            packet = (HCI_Uart_Packet_t *)DLLIST_FIRST(&(hciUart->restHead));
            DLLIST_REMOVE_FIRST(&(hciUart->restHead));
        } else {
            BASIC_ASSERT(0);
        }
        uartInfo->rxPacket = packet;
        return hciUart->restNum;
    }

    BASIC_ASSERT(0);
    return 0;  //return true for end of thread
}

int _RAW_Send_(HciUartInfo_t *info, u8 *buf, u32 len)
{
    printf("[%-6s][%-6s] %s() start\n", info->comName, info->aliasName, __func__);

    return LibUartHdl_Send(info->hdl, buf, len);
}

int _RAW_Receive_(HciUartInfo_t *info)
{
    printf("[%-6s][%-6s] %s() start\n", info->comName, info->aliasName, __func__);

    hci_uart_rx_one_packet(info);
    ARRAYDUMPX2(info->rxPacket->buf, info->rxPacket->len);

    MM_FREE(info->rxPacket);
    return 0;
}

void hci_uart_rx_init(HciUartInfo_t *uartInfo)
{
    HCI_Uart_t *hciUart = &(uartInfo->hciUart);

    DLLIST_HEAD_RESET(&(hciUart->restHead));
    hciUart->restNum = 0;
    hciUart->currPkt = NULL;
    hciUart->state = WAIT_HEADER;
    hciUart->lenCtr = 0;
    hciUart->lenMax = 0xFFFFFFFF;
}

void hci_uart_rx_uninit(HciUartInfo_t *uartInfo)
{
    HCI_Uart_t *hciUart = &(uartInfo->hciUart);
    HCI_Uart_Packet_t *prev_desc;
    HCI_Uart_Packet_t *curr_desc;

    DLLIST_WHILE_START(&(hciUart->restHead), curr_desc, HCI_Uart_Packet_t)
    {
        prev_desc = curr_desc;
        DLLIST_WHILE_NEXT(curr_desc, HCI_Uart_Packet_t);
        printf("[%6s][%6s] Drop unused packet:\n", uartInfo->comName, uartInfo->aliasName);
        ARRAYDUMPX2(prev_desc->buf, prev_desc->len);
        MM_FREE(prev_desc);
    }
}

HciUart::HciUart(int isVerbose /*= 0*/)
{
    is_inited = 0;
    is_verbose = isVerbose;

    if (is_verbose)
        printf("%s()\n", __func__);
}

HciUart::~HciUart(void)
{
    if (is_verbose)
        printf("%s()\n", __func__);

    if (is_inited) {
        LibUartHdl_UninitComPort(info.hdl);
        LibUartHdl_HandleDestroy(info.hdl);

        hci_uart_rx_uninit(&info);
    }
}

int HciUart::Init(const char *comPortNameFile)
{
    info.hdl = LibUartHdl_HandleCreate();
    info.rx_buf_len = HCI_UART_RX_BUF_LEN;
    LibUartMgr_GetComPortConfigFrom_INI_File(comPortNameFile, info.comName, &info.baudRate);
    LibUartHdl_InitComPort(info.hdl, info.comName, info.baudRate);
    //LibUartHdl_SniffSetting(gUartInfo.hdl, 1);

    SetAliasName("default", 6);

    hci_uart_rx_init(&info);

    is_inited = 1;
    return 0;
}

int HciUart::SetAliasName(const char *aliasName, int nameLen)
{
    LibString_Copy(info.aliasName, aliasName, nameLen);
    return 0;
}

int HciUart::RawSend(u8 *buf, u32 len)
{
    return _RAW_Send_(&info, buf, len);
}

int HciUart::RawReceive(void)
{
    return _RAW_Receive_(&info);
}

int HciUart::SniffSetting(int val)
{
    return LibUartHdl_SniffSetting(info.hdl, val);
}

