

#include "SimChannel.hpp"

static SimChanPara_t gSimChanPara = {
    .sim_chan_ram_rx_call_max_len = 1,
    .uart_com_port_name = "COM20",
    .uart_baud_rate = 115200,
};

SimChanPara_t *SimChannel_PreConfigPara(void)
{
    return &gSimChanPara;
}

static int _SimChannel_UART_TxFunc(u8 *buffer, u32 length)
{
    return LibUart_Send(buffer, length);
}

static int _SimChannel_UART_RxFunc(u8 *buffer, u32 *receivedLength)
{
    return LibUart_ReceiveEx(buffer, receivedLength, 80);
}

static LibBufferBasic gSimRamRxBuf;
static u32 gSimRamRxBuf_CurrLen = 0;
static u8 *gSimRamRxBuf_CurrPtr = NULL;
static int _SimChannel_RAM_RxFunc(u8 *buffer, u32 *receivedLength)
{
    BASIC_ASSERT(gSimRamRxBuf.bufPtr != NULL); // please call SimChannel_Init() first.

    u32 copyLen = gSimRamRxBuf_CurrLen;
    if (copyLen > gSimChanPara.sim_chan_ram_rx_call_max_len)
        copyLen = gSimChanPara.sim_chan_ram_rx_call_max_len;

    memcpy(buffer, gSimRamRxBuf_CurrPtr, copyLen);
    gSimRamRxBuf_CurrLen -= copyLen;
    gSimRamRxBuf_CurrPtr += copyLen;

    *receivedLength = copyLen;

    return 0;
}

int SimChannel_RamTxBackCallback(u8 *buffer, u32 length)
{
    BASIC_ASSERT(gSimRamRxBuf.bufPtr != NULL); // please call SimChannel_Init() first.

    memcpy(gSimRamRxBuf.bufPtr, buffer, length);
    gSimRamRxBuf_CurrLen = length;
    gSimRamRxBuf_CurrPtr = (u8 *)gSimRamRxBuf.bufPtr;

    return 0;
}



struct {
    SIM_CHAN_TYPES_t type;
    SimChan_TxFunc   txFunc;
    SimChan_RxFunc   rxFunc;
} SimChan_CbTable[] = {
    {SIM_CHAN_RAM,     NULL                   , _SimChannel_RAM_RxFunc},
    {SIM_CHAN_UART,    _SimChannel_UART_TxFunc, _SimChannel_UART_RxFunc},
    {SIM_CHAN_USB_HID, NULL,         NULL},
};

SIM_CHAN_TYPES_t gSimChanType = SIM_CHAN_INVALID;

int SimChannel_Init(u32 ramRxBufSize, SimChan_TxFunc ramTxFunc)
{
    gSimRamRxBuf.Init(ramRxBufSize);

    SimChan_CbTable[SIM_CHAN_RAM].txFunc = ramTxFunc;

    LibUart_InitComPort(gSimChanPara.uart_com_port_name, gSimChanPara.uart_baud_rate, 3);

    return 0;
}

int SimChannel_Uninit(void)
{
    LibUart_UninitComPort();

    return 0;
}

int SimChannel_TypeSet(SIM_CHAN_TYPES_t type)
{
    gSimChanType = type;

    return 0;
}

int SimChannel_TxStart(u8 *buf, u32 len)
{
    BASIC_ASSERT(gSimChanType < SIM_CHAN_INVALID);
    BASIC_ASSERT(SimChan_CbTable[gSimChanType].txFunc != NULL);

    return SimChan_CbTable[gSimChanType].txFunc(buf, len);
}

int SimChannel_RxStart(u8 *buf, u32 *receivedLength)
{
    BASIC_ASSERT(gSimChanType < SIM_CHAN_INVALID);
    BASIC_ASSERT(SimChan_CbTable[gSimChanType].rxFunc != NULL);

    return SimChan_CbTable[gSimChanType].rxFunc(buf, receivedLength);
}

static int _Demo_TxSink(u8 *txBuf, u32 txBufLen)
{
    ARRAYDUMPX2(txBuf, txBufLen);

    u8 txBackBuf[] = {
        /* CCE */
        /* 7*/0x04, 0x0E, 0x04, 0x01, 0x03, 0x0C, 0x00,
    };
    SimChannel_RamTxBackCallback(txBackBuf, sizeof(txBackBuf));

    return 0;
}

static int _Demo_HCI_Event_RxStart(u8 *buf, u32 *receivedLength)
{
    u32 tempRxLen;
    u8 *currBufPtr = buf;
    u32 currBufCtr = 0;

    while (currBufCtr < 3) {
        SimChannel_RxStart(currBufPtr, &tempRxLen);
        currBufPtr += tempRxLen;
        currBufCtr += tempRxLen;
    }

    u32 hciEventLen = buf[2];

    while (currBufCtr < 3 + hciEventLen) {
        SimChannel_RxStart(currBufPtr, &tempRxLen);
        currBufPtr += tempRxLen;
        currBufCtr += tempRxLen;
    }

    *receivedLength = currBufCtr;

    return 0;
}

int SimChannel_Demo(void)
{
    printf("This is %s()\n", __func__);

    SimChanPara_t *para = SimChannel_PreConfigPara();
    para->sim_chan_ram_rx_call_max_len = 1;
    para->uart_com_port_name = "COM26";
    para->uart_baud_rate = 115200;
    SimChannel_Init(1000, _Demo_TxSink);
    SimChannel_TypeSet(SIM_CHAN_RAM);
    //SimChannel_TypeSet(SIM_CHAN_UART);

    u8 testData[] = {
        /* CMD R */
        /* 4*/0x01, 0x03, 0x0C, 0x00,
    };

    SimChannel_TxStart(testData, sizeof(testData));

    u8 rxBuf[10];
    u32 rxLen;
    SimChannel_RxStart(rxBuf, &rxLen);
    ARRAYDUMPX2(rxBuf, rxLen); // Not receive enough bytes. Need keep receiving!!

    // Rewrite correct procedure.
    SimChannel_TxStart(testData, sizeof(testData));
    _Demo_HCI_Event_RxStart(rxBuf, &rxLen);
    ARRAYDUMPX2(rxBuf, rxLen); // Not receive enough bytes. Need keep receiving!!

    SimChannel_Uninit();
    return 0;
}

