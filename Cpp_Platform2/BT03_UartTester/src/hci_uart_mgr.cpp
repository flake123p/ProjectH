
#include "Everything_App.hpp"

//extern int gEndOfTest;
LibMT_ThreadInfo_t *gMainTask;
LibMT_ThreadInfo_t *gHciUartMgr;
int gEndOfTest = 0;

void _Msg_To_Main(RX_Msg_t *rxMsg)
{
    //extern LibMT_ThreadInfo_t *gMainTask;

    LibMT_Msg_t *msg = LibMT_MsgGet();
    msg->val = 0xFFFF; //dummy
    msg->hdl = (void *)rxMsg;
    LibMT_MsgToThread(msg, gMainTask);
}

int hci_uart_mgr(LibMT_Msg_t *msg)
{
    switch (msg->val)
    {
        case START_RX: {
            u8 *rxBuf = (u8 *)MM_ALLOC(800);
            u32 rxLen;
            //u32 rxLenOld;
            UART_Info_t *uartInfo = (UART_Info_t *)msg->hdl;
            int state = WAIT_HEADER;
            RX_Msg_t *rxMsg;
            u32 lenCtr;
            u32 lenMax;
            while (1) {
                if (gEndOfTest) {
                    break;
                }
                LibUartHdl_ReceiveEx(uartInfo->hdl, rxBuf, &rxLen, 350);
                SAFE_PRINT("len %d\n", rxLen);
                if (rxLen == 0) {
                    continue;
                }
                FOREACH_I(rxLen) {
                    switch (state)
                    {
                        case WAIT_HEADER: {
                            rxMsg = Get_Rx_Msg();
                            rxMsg->buf[0] = rxBuf[i];
                            if (rxMsg->buf[0] == 0x04) {
                                state = WAIT_EVT_DATA_DONE;
                            } else if (rxMsg->buf[0] == 0x02) {
                                state = WAIT_ACL_DATA_DONE;
                            } else if (rxMsg->buf[0] == 0x05) {
                                state = WAIT_MY_DATA_DONE;
                            } else {
                                //do nothing, drop this byte;
                                Release_Rx_Msg(rxMsg);
                            }
                            lenCtr = 1;
                            lenMax  = 0xFFFF;
                        } break;

                        case WAIT_MY_DATA_DONE: {
                            rxMsg->buf[lenCtr] = rxBuf[i];
                            lenCtr++;
                            if (lenCtr == 10) {
                                rxMsg->len = lenCtr;
                                _Msg_To_Main(rxMsg);
                                state = WAIT_HEADER;
                            }
                        } break;

                        case WAIT_EVT_DATA_DONE: {
                            rxMsg->buf[lenCtr] = rxBuf[i];
                            lenCtr++;
                            if (lenCtr == 3) {
                                lenMax = rxMsg->buf[2];
                            }
                            if (lenMax != 0xFFFF) {
                                if (lenCtr == lenMax+3) {
                                    rxMsg->len = lenCtr;
                                    _Msg_To_Main(rxMsg);
                                    state = WAIT_HEADER;
                                }
                            }
                        } break;

                        case WAIT_ACL_DATA_DONE: {
                            rxMsg->buf[lenCtr] = rxBuf[i];
                            lenCtr++;
                            if (lenCtr == 5) {
                                lenMax = rxMsg->buf[5];
                                lenMax = lenMax << 8;
                                lenMax += rxMsg->buf[4];
                            }
                            if (lenMax != 0xFFFF) {
                                if (lenCtr == lenMax+5) {
                                    rxMsg->len = lenCtr;
                                    _Msg_To_Main(rxMsg);
                                    state = WAIT_HEADER;
                                }
                            }
                        } break;
                    }
                }
            }
            MM_FREE(rxBuf);
        } break;

        default:
            break;
    }
    return 1;  //return true for end of thread
}

int _Cmdx_RESET(UART_Info_t *info)
{
    u8 cmd[] = {0x01, 0x03, 0x0C, 0x00};
    SAFE_PRINT("[%6s][%6s] %s start\n", info->comName, info->aliasName, __func__);
    LibUartHdl_Send(info->hdl, cmd, sizeof(cmd));
    return 0;
}


Lib_DescMT_Head_t *gMsgBufferHead;
RX_Msg_t *Get_Rx_Msg(void)
{
    return (RX_Msg_t *)LibDescMT_GetDesc(gMsgBufferHead);
}

void Release_Rx_Msg(RX_Msg_t *rxMsg)
{
    LibDescMT_ReleaseDesc(gMsgBufferHead, (Lib_Desc_Info_t *)rxMsg);
}
void msg_buf_init(void)
{
    gMsgBufferHead = LibDescMT_CreateList(20, sizeof(RX_Msg_t));
}

void msg_buf_uninit(void)
{
    LibDescMT_DestroyList(gMsgBufferHead);
}

int Main_Task(LibMT_Msg_t *msg)
{
    switch (msg->val)
    {
        case END_OF_THREAD:
            gEndOfTest = 1;
            return 1;

        default: {
            RX_Msg_t *rxMsg = (RX_Msg_t *)msg->hdl;
            ARRAYDUMPX2(rxMsg->buf, rxMsg->len);
            gEndOfTest = 1;
            return 1;
        } break;
    }
    return 0;
}

void thread_init(void)
{
    gMainTask = LibMT_CreateThread(Main_Task);
    gHciUartMgr = LibMT_CreateThread(hci_uart_mgr);
    LibOs_SleepMiliSeconds(10);
    {
        LibMT_Msg_t *msg = LibMT_MsgGet();
        msg->val = START_RX;
        //msg->hdl = (void *)&gIUT;
        LibMT_MsgToThread(msg, gHciUartMgr);
    }
    LibOs_SleepMiliSeconds(10);
}

void thread_uninit(void)
{
    LibMT_WaitMainThreadAndDestroyAll(gMainTask);
}

void hci_uart_rx_init(UART_Info_t *uartInfo)
{
    HCI_Uart_t *hciUart = &(uartInfo->hciUart);

    DLLIST_HEAD_RESET(&(hciUart->restHead));
    hciUart->restNum = 0;
    hciUart->currPkt = NULL;
    hciUart->state = WAIT_HEADER;
    hciUart->lenCtr = 0;
    hciUart->lenMax = 0xFFFFFFFF;
}

void hci_uart_rx_uninit(UART_Info_t *uartInfo)
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


// return number of rest packets in 1 uart rx
u32 hci_uart_rx_one_packet(UART_Info_t *uartInfo)
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
        LibUartHdl_ReceiveEx(uartInfo->hdl, hciUart->buf, &rxDoneLen, 350);
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

