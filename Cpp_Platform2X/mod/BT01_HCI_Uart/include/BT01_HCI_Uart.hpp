

#ifndef _BT01_HCI_UART_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

#define HCI_UART_RX_BUF_LEN (350)

typedef enum {
    WAIT_HEADER,
    WAIT_MY_DATA_DONE,
    WAIT_EVT_DATA_DONE,
    WAIT_ACL_DATA_DONE,
} HCI_Uart_State_t;

typedef struct HCI_Uart_Packet_t {
    DLList_Head_t entry;
    u32 len;
    u8  buf[HCI_UART_RX_BUF_LEN];
} HCI_Uart_Packet_t;

typedef struct {
    DLList_Head_t restHead;
    u32 restNum;
    HCI_Uart_Packet_t *currPkt;
    HCI_Uart_State_t state;
//    u32 start;
    u32 lenCtr;
    u32 lenMax;
    u8 buf[HCI_UART_RX_BUF_LEN];
} HCI_Uart_t;

typedef struct {
    UART_HANDLE_t hdl;
    char comName[32];
    char aliasName[32];
    uint32_t baudRate;
    u8 rx_buf[4000];
    u32 rx_buf_len;
    u32 rx_done_len;
    HCI_Uart_Packet_t *rxPacket;
    HCI_Uart_t hciUart;
} HciUartInfo_t;

int BT01_HCI_Uart_Demo(void);

u32 hci_uart_rx_one_packet(HciUartInfo_t *uartInfo);
int _RAW_Send_(HciUartInfo_t *info, u8 *buf, u32 len);
int _RAW_Receive_(HciUartInfo_t *info);

void hci_uart_rx_init(HciUartInfo_t *uartInfo);
void hci_uart_rx_uninit(HciUartInfo_t *uartInfo);

class HciUart{
public:
    int is_verbose;
    int is_inited;
    HciUartInfo_t info;

    HciUart(int isVerbose = 0);
    ~HciUart(void);

    int Init(const char *comPortNameFile);
    int SetAliasName(const char *aliasName, int nameLen);
    int RawSend(u8 *buf, u32 len);
    int RawReceive(void);
    int SniffSetting(int val);
};

#define _BT01_HCI_UART_HPP_INCLUDED_
#endif//_BT01_HCI_UART_HPP_INCLUDED_



