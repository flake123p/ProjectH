

#ifndef _SIM_CHANNEL_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

typedef enum {
    SIM_CHAN_RAM,
    SIM_CHAN_UART,
    SIM_CHAN_USB_HID,

    SIM_CHAN_INVALID,
}SIM_CHAN_TYPES_t;

typedef int (*SimChan_TxFunc)(u8 *txBuf, u32 txBufLen);
typedef int (*SimChan_RxFunc)(u8 *rxBuf, u32 *rxLen);

typedef struct {
    u32 sim_chan_ram_rx_call_max_len;
    const char *uart_com_port_name;
    u32 uart_baud_rate;
}SimChanPara_t;
SimChanPara_t *SimChannel_PreConfigPara(void);

int SimChannel_RamTxBackCallback(u8 *buffer, u32 length);

int SimChannel_Init(u32 ramRxBufSize, SimChan_TxFunc ramTxFunc);
int SimChannel_Uninit(void);
int SimChannel_TypeSet(SIM_CHAN_TYPES_t type);
int SimChannel_TxStart(u8 *buf, u32 len);
int SimChannel_RxStart(u8 *buf, u32 *receivedLength);

int SimChannel_Demo(void);


#define _SIM_CHANNEL_HPP_INCLUDED_
#endif//_SIM_CHANNEL_HPP_INCLUDED_



