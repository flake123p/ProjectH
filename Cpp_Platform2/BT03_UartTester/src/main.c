

#include "Everything_App.hpp"

//u16 gCONN_INTERVAL = 0x000A; //0x0006~0x0C80
//u16 gCONN_TIMEOUT = 0x0020; //0x000A~0x0C80
u16 gCONN_INTERVAL = 0x0006; //0x0006~0x0C80
u16 gCONN_TIMEOUT = 0x0C80; //0x000A~0x0C80
u16 gCONN_LATENCY = 1; //0~0x01F3 (1F3=499)
u16 gCONN_MAX_CE_LEN = 0x0020;
u16 gCONN_MIN_CE_LEN = 0x0010;
UART_Info_t gIUT;
UART_Info_t gTester;

u16 gIutConnHdl = 0xFFFF;
u16 gTesterConnHdl = 0xFFFF;

//u32 gLoopMax = 0xFFFFFFFF;
u32 gLoopMax = 5;
u32 gLoopCtr = 0;

File_Profiles_t gCurrLogFile;
char gLogFileName[] = "log00.xls";

void uart_init(void);
void uart_uninit(void);

int EVENT_Interval(HCI_Uart_Packet_t *packet)
{
    int endOfTest = 0;
    u32 msg[6];
    u32 msgCtr = 0;

    printf("intval:%d, latency:%d, loopCtr:%d\n", gCONN_INTERVAL, gCONN_LATENCY, gLoopCtr);
//    DUMPD(gLoopCtr);
//    ARRAYDUMPX2(packet->buf, packet->len);

    if (gLoopCtr == 0) {
        gCurrLogFile.fileName = gLogFileName;
        gCurrLogFile.openMode = "w+b";
        if (LibFileIo_OpenFile(&gCurrLogFile)) {
            BASIC_ASSERT(0);
        }
    }

    FOR_I(0x18) {
        msg[msgCtr] = (msg[msgCtr] & 0xFFFFFF00) | packet->buf[3+i];
        if ((i % 4) == 3) {
            msgCtr++;
        } else {
            msg[msgCtr] = msg[msgCtr] << 8;
        }
    }
#if 1
    fprintf(gCurrLogFile.fp, "%d\t%d\t%d\t%d\t%d\t%d\n",
        msg[1]>>24,
        msg[1]&0x00FFFFFF,
        msg[2],
        msg[3],
        msg[4],
        msg[5]);
#endif
//    ARRAYDUMPX2(msg, 6);

    if (msg[1] == 0xCC000000) {
        endOfTest = 1;
    }

    gLoopCtr++;
    if (gLoopCtr == gLoopMax) {
        endOfTest = 1;
    }

    if (endOfTest) {
        endOfTest = 1;
        LibFileIo_CloseFile(&gCurrLogFile);
        return 1;
    } else {
        return 0;
    }
}


int EVENT_IUT_ContinueReceive(UART_Info_t *info)
{
    int endLoop = 0;
    u16 evtInternal[] = {0x04, 0xFF, 0x18};
    u16 evtConnComplete[] = {0x04, 0x3E, 0x13, 0x01, 0x00, 0x80CC, 0x80CC, 0x8000, 0x8000, 0x80FF, 0x8093, 0x8093, 0x8067, 0x8011, 0x8000, (u8)gCONN_INTERVAL, (u8)(gCONN_INTERVAL>>8), (u8)gCONN_LATENCY, (u8)(gCONN_LATENCY>>8), (u8)gCONN_TIMEOUT, (u8)(gCONN_TIMEOUT>>8), 0x00};

    while (endLoop == 0)
    {
        hci_uart_rx_one_packet(info);
        do {
            if (0 == LibComm_ArrayCompare(evtInternal, sizeof(evtInternal)/sizeof(u16), info->rxPacket->buf, 3, 0)) {
                endLoop = EVENT_Interval(info->rxPacket);
                break;
            }
            if (0 == LibComm_ArrayCompare(evtConnComplete, sizeof(evtConnComplete)/sizeof(u16), info->rxPacket->buf, info->rxPacket->len, 0)) {
                gIutConnHdl = info->rxPacket->buf[6];
                gIutConnHdl = gIutConnHdl << 8;
                gIutConnHdl += info->rxPacket->buf[5];
                gIutConnHdl = gIutConnHdl & 0x0FFF;
                break;
            }
            printf("UNKNOWN PACKET : ");
            ARRAYDUMPX2(info->rxPacket->buf, info->rxPacket->len);
            endLoop = 1;
        } while(0);
        MM_FREE(info->rxPacket);
    }
    return 0;
}

void _TestCase_0_CreateConnection(void)
{
    printf("%s() ...\n", __func__);

    gCONN_INTERVAL = 0x0006;
    gCONN_LATENCY = 0;
    gCONN_TIMEOUT = 0x0018;
    gCONN_MAX_CE_LEN = 0x0020;
    gCONN_MIN_CE_LEN = 0x0010;

    CMD_RESET(&gIUT);
    CMD_RESET(&gTester);

    u8 addr[] = {0xff, 0x93, 0x93, 0x67, 0x11, 0x00};
    CMD_VENDOR_SetBdAddress(&gIUT, addr);

    CMD_Set_Event_Mask(&gIUT);
    CMD_LE_Set_Event_Mask(&gIUT);
    CMD_LE_Write_Advertising_Parameters(&gIUT);
    CMD_LE_Write_Advertise_Enable(&gIUT);

    CMD_RESET(&gTester);
    CMD_LE_Create_Connection(&gTester);
    EVT_LE_Connection_Complete(&gTester, &gTesterConnHdl);
    DUMPNX(gTesterConnHdl);

    //EVT_LE_Connection_Complete(&gIUT, &gIutConnHdl);
    //DUMPNX(gIutConnHdl);
    EVENT_IUT_ContinueReceive(&gIUT);

    CMD_Disconnect(&gIUT, gIutConnHdl, 0x13);
    EVT_Disconnect_Complete(&gIUT, gIutConnHdl, 0x16);
    EVT_Disconnect_Complete(&gTester, gTesterConnHdl, 0x13);
}

void _TestCase_0_CreateConnection_Multi(void)
{
    u16 intervalAry[] = {
        6,
        12,
        25,
        50,
        100,
    
        200,
        400,
        800,
        1600,
        3200,
    };

    u16 latencyAry[] = {
        0,
        1,
        5,
        8,
        10,
    };
    for(unsigned int i=8;i<10;i++)
    {
        for(unsigned int j=2;j<5;j++)
        {
            uart_uninit();
            uart_init();

            gCONN_INTERVAL = intervalAry[i];
            gCONN_LATENCY = latencyAry[j];
            gLoopCtr = 0;
            gLoopMax = 50;
            gLogFileName[3] = '0' + i;
            gLogFileName[4] = '0' + j;

            printf("%s() ... i:%d\n", __func__, i);

            CMD_RESET(&gIUT);
            CMD_RESET(&gTester);
            CMD_Set_Event_Mask(&gIUT);
            CMD_LE_Set_Event_Mask(&gIUT);
            CMD_LE_Write_Advertising_Parameters(&gIUT);
            CMD_LE_Write_Advertise_Enable(&gIUT);

            CMD_RESET(&gTester);
            CMD_LE_Create_Connection(&gTester);
            EVT_LE_Connection_Complete(&gTester, &gTesterConnHdl);
            DUMPNX(gTesterConnHdl);

            //EVT_LE_Connection_Complete(&gIUT, &gIutConnHdl);
            //DUMPNX(gIutConnHdl);
            EVENT_IUT_ContinueReceive(&gIUT);

            CMD_Disconnect_NoWait(&gIUT, gIutConnHdl, 0x13);
            LibOs_SleepMiliSeconds(2000);
        }
    }
}

void _TestCase_0_CreateConnection_MAS(void)
{
    printf("%s() ...\n", __func__);

    gCONN_INTERVAL = 0x0006;
    gCONN_LATENCY = 0;
    gCONN_TIMEOUT = 0x0018;
    gCONN_MAX_CE_LEN = 0x0020;
    gCONN_MIN_CE_LEN = 0x0010;

    CMD_RESET(&gIUT);
    CMD_RESET(&gTester);

    CMD_Set_Event_Mask(&gIUT);
    CMD_LE_Set_Event_Mask(&gIUT);
    CMD_Set_Event_Mask(&gTester);
    CMD_LE_Set_Event_Mask(&gTester);

    u8 addr[] = {0xff, 0x93, 0x93, 0x67, 0x11, 0x00};
    CMD_VENDOR_SetBdAddress(&gTester, addr);
    CMD_LE_Write_Advertising_Parameters(&gTester);
    CMD_LE_Write_Advertise_Enable(&gTester);

    CMD_LE_Create_Connection(&gIUT);
    EVT_LE_Connection_Complete(&gIUT, &gIutConnHdl);
    DUMPNX(gIutConnHdl);

}

void _TestCase_1_ResetALL(void)
{
    printf("%s() ...\n", __func__);

    CMD_RESET(&gIUT);
    CMD_RESET(&gTester);
}



void uart_init(void)
{
    gIUT.hdl = LibUartHdl_HandleCreate();
    LibString_Copy(gIUT.aliasName, "IUT   ", 6);
    gIUT.rx_buf_len = 350;
    LibUartMgr_GetComPortConfigFrom_INI_File("Uart_IUT.ini", gIUT.comName, &gIUT.baudRate);
    LibUartHdl_InitComPort(gIUT.hdl, gIUT.comName, gIUT.baudRate);
    //LibUartHdl_SniffSetting(gIUT.hdl, 1);

    gTester.hdl = LibUartHdl_HandleCreate();
    LibString_Copy(gTester.aliasName, "Tester", 6);
    gTester.rx_buf_len = 350;
    LibUartMgr_GetComPortConfigFrom_INI_File("Uart_Tester.ini", gTester.comName, &gTester.baudRate);
    LibUartHdl_InitComPort(gTester.hdl, gTester.comName, gTester.baudRate);

    hci_uart_rx_init(&gIUT);
    hci_uart_rx_init(&gTester);
}

void uart_uninit(void)
{
    LibUartHdl_UninitComPort(gIUT.hdl);
    LibUartHdl_HandleDestroy(gIUT.hdl);
    LibUartHdl_UninitComPort(gTester.hdl);
    LibUartHdl_HandleDestroy(gTester.hdl);

    hci_uart_rx_uninit(&gIUT);
    hci_uart_rx_uninit(&gTester);
}






void UartTester_Main(int test_case_index)
{
    //msg_buf_init();
    uart_init();
    //thread_init();

#if 1
    switch (test_case_index) {
        case 0:
            _TestCase_0_CreateConnection_MAS();
            break;
        case 1:
            _TestCase_1_ResetALL();
            break;
        case 2:
            _TestCase_0_CreateConnection();
            break;
        default:
            printf("unknown test_case_index:%d\n", test_case_index);
            break;
    }
#endif

    //thread_uninit();
    uart_uninit();
    //msg_buf_uninit();
}

int main(int argc, char *argv[])
{
    Lib_Init(LIB_MT_ENABLE);

    int test_case_index;
    do {
        if (argc == 1) {
            test_case_index = 0;
        } else if (argc == 2) {
            test_case_index = argv[1][0] - '0';
        } else {
            printf("too many arguments! do nothing ...\n");
            break;
        }
        DUMPND(test_case_index);
        UartTester_Main(test_case_index);
    } while (0);

    Lib_Uninit();
    return 0;
}


