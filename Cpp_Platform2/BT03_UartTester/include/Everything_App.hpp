

#ifndef _EVERYTHING_APP_HPP_INCLUDED_


#include "Everything_Lib_Mgr.hpp"

typedef enum {
    WAIT_HEADER,
    WAIT_MY_DATA_DONE,
    WAIT_EVT_DATA_DONE,
    WAIT_ACL_DATA_DONE,
} HCI_Uart_State_t;

typedef struct HCI_Uart_Packet_t {
    DLList_Head_t entry;
    u32 len;
    u8  buf[350];
} HCI_Uart_Packet_t;

typedef struct {
    DLList_Head_t restHead;
    u32 restNum;
    HCI_Uart_Packet_t *currPkt;
    HCI_Uart_State_t state;
//    u32 start;
    u32 lenCtr;
    u32 lenMax;
    u8 buf[350];
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
} UART_Info_t;

typedef struct {
    Lib_Desc_Info_t entry;
    u32 len;
    u8 buf[350];
} RX_Msg_t;

enum {
    END_OF_THREAD,
    START_RX,
};
int hci_uart_mgr(LibMT_Msg_t *msg);

RX_Msg_t *Get_Rx_Msg(void);
void Release_Rx_Msg(RX_Msg_t *rxMsg);

void hci_uart_rx_init(UART_Info_t *uartInfo);
void hci_uart_rx_uninit(UART_Info_t *uartInfo);
// return number of rest packets in 1 uart rx
u32 hci_uart_rx_one_packet(UART_Info_t *uartInfo);

extern u32 gCONN_INTERVAL;
extern u32 gCONN_TIMEOUT;
extern u32 gCONN_LATENCY;
extern u16 gCONN_MAX_CE_LEN;
extern u16 gCONN_MIN_CE_LEN;

int _RAW_Send_(UART_Info_t *info, u8 *buf, u32 len);
int _RAW_Receive_(UART_Info_t *info, int autoFree = 1);

int _CMD_(UART_Info_t *info, const char *funcStr, u8 *cmd, u32 cmdLen, u16 *answer, u32 answerLen);
int _EVENT_(UART_Info_t *info, const char *funcStr, u16 *answer, u32 answerLen);

int CMD_RESET(UART_Info_t *info);
int CMD_RESET2(UART_Info_t *info);
int CMD_Set_Event_Mask(UART_Info_t *info);
int CMD_LE_Set_Event_Mask(UART_Info_t *info);
int CMD_LE_Write_Advertising_Parameters(UART_Info_t *info);
int CMD_LE_Write_Advertise_Enable(UART_Info_t *info);
int CMD_LE_Create_Connection(UART_Info_t *info);
int CMD_LE_Create_Connection_WithBdAddr(UART_Info_t *info, u8 *bd_addr);
int CMD_Disconnect(UART_Info_t *info, u16 connHdl, u8 reason);
int CMD_Disconnect_NoWait(UART_Info_t *info, u16 connHdl, u8 reason);
int CMD_VENDOR_SetBdAddress(UART_Info_t *info, u8 *addr);
int CMD_LE_Set_PHY(UART_Info_t *info, u16 connHdl, u8 all_phys, u8 tx_phys, u8 rx_phys, u16 phy_options);

int EVT_LE_Connection_Complete(UART_Info_t *info, u16 *outConnHdl);
int EVT_LE_Enhanced_Connection_Complete(UART_Info_t *info, u16 *outConnHdl);
int EVT_LE_PHY_Update_Complete_Event(UART_Info_t *info, u16 connHdl);

int EVT_Disconnect_Complete(UART_Info_t *info, u16 connHdl, u16 reasonAnswer);

#define _EVERYTHING_APP_HPP_INCLUDED_
#endif//_EVERYTHING_APP_HPP_INCLUDED_



