

#include "Everything_App.hpp"

#define BUILD_2_ACL       (0)
#define ENABLE_UART_SNIFF (0)


//u16 gCONN_INTERVAL = 0x000A; //0x0006~0x0C80
//u16 gCONN_TIMEOUT = 0x0020; //0x000A~0x0C80
u32 gCONN_INTERVAL = 0x0006; //0x0006~0x0C80
u32 gCONN_TIMEOUT = 0x0C80; //0x000A~0x0C80
u32 gCONN_LATENCY = 1; //0~0x01F3 (1F3=499)
u16 gCONN_MAX_CE_LEN = 0x0020;
u16 gCONN_MIN_CE_LEN = 0x0010;
UART_Info_t gIUT;
UART_Info_t gTester;
UART_Info_t gTester2;

u32 gSkipAdv = 0;

u32 gIUT_BD_ADDR[6];

u16 gIutConnHdl = 0xFFFF;
u16 gIutConnHdl2 = 0xFFFF;
u16 gTesterConnHdl = 0xFFFF;
u16 gTester2ConnHdl = 0xFFFF;

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

    //gCONN_INTERVAL = 0x0028;
    //gCONN_INTERVAL = 0x0006;
    //gCONN_LATENCY = 0;
    //gCONN_TIMEOUT = 0x0050;
    gCONN_MAX_CE_LEN = 0x0020;
    gCONN_MIN_CE_LEN = 0x0010;

    //CMD_RESET(&gIUT);
    CMD_RESET(&gTester);

    u8 addr[] = {(u8)gIUT_BD_ADDR[0], (u8)gIUT_BD_ADDR[1], (u8)gIUT_BD_ADDR[2], (u8)gIUT_BD_ADDR[3], (u8)gIUT_BD_ADDR[4], (u8)gIUT_BD_ADDR[5]};

    if (gSkipAdv == 0) {
        CMD_VENDOR_SetBdAddress(&gIUT, addr);

        CMD_Set_Event_Mask(&gIUT);
        CMD_LE_Set_Event_Mask(&gIUT);
    }
    CMD_Set_Event_Mask(&gTester);
    CMD_LE_Set_Event_Mask(&gTester);

    if (gSkipAdv == 0) {
        CMD_LE_Write_Advertising_Parameters(&gIUT);
        CMD_LE_Write_Advertise_Enable(&gIUT);
    }
    CMD_LE_Create_Connection_WithBdAddr(&gTester, addr);
    //ARRAYDUMPX2(addr, 6);
    //EVT_LE_Connection_Complete(&gTester2, &gTesterConnHdl);
    EVT_LE_Enhanced_Connection_Complete(&gTester, &gTesterConnHdl);
    DUMPNX(gTesterConnHdl);

    //EVT_LE_Connection_Complete(&gIUT, &gIutConnHdl);
    EVT_LE_Enhanced_Connection_Complete(&gIUT, &gIutConnHdl);
    DUMPNX(gIutConnHdl);

    return;
#if 0
    //EVENT_IUT_ContinueReceive(&gIUT);
    while(1) {
        _RAW_Receive_(&gTester2);
    }
    return;
#endif

//Change PHY
#if 0
    LibOs_SleepMiliSeconds(500);
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
    CMD_LE_Set_PHY(&gIUT, gIutConnHdl, 0, 4, 4, 0);
    EVT_LE_PHY_Update_Complete_Event(&gIUT, gIutConnHdl);
    LibOs_SleepMiliSeconds(500);
#endif

//ACL
#if 0
    {
        #define ACL_LEN (27+4)
        u8 buf[ACL_LEN + 5] = {0x02, (u8)gTesterConnHdl, (u8)(gTesterConnHdl>>8), ACL_LEN, 0};
        FOR_I(ACL_LEN) {
            buf[i+5]=0xA0+i;
        }
        _RAW_Send_(&gTester, buf, sizeof(buf));
        _RAW_Receive_(&gIUT);
        _RAW_Receive_(&gIUT);
        _RAW_Receive_(&gTester);
    }
#endif

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
    //LibUartHdl_SniffSetting(gIUT.hdl, 1);

    CMD_RESET(&gIUT);
    CMD_RESET(&gTester);

    u8 addr[] = {0xff, 0x93, 0x93, 0x67, 0x11, 0xFF};
    CMD_VENDOR_SetBdAddress(&gIUT, addr);

    CMD_Set_Event_Mask(&gIUT);
    CMD_LE_Set_Event_Mask(&gIUT);
    CMD_Set_Event_Mask(&gTester);
    CMD_LE_Set_Event_Mask(&gTester);

    CMD_LE_Write_Advertising_Parameters(&gTester);
    CMD_LE_Write_Advertise_Enable(&gTester);

    {
        u8 addr2[] = {0x70, 0x2D, 0xFB, 0x2E, 0xF6, 0xD4};
        CMD_LE_Create_Connection_WithBdAddr(&gIUT, addr2);
    }
    EVT_LE_Enhanced_Connection_Complete(&gIUT, &gIutConnHdl);
    DUMPNX(gIutConnHdl);

    //EVT_LE_Connection_Complete(&gTester, &gTesterConnHdl);
    EVT_LE_Enhanced_Connection_Complete(&gTester, &gTesterConnHdl);
    DUMPNX(gTesterConnHdl);

    //EVENT_IUT_ContinueReceive(&gIUT);

//Change PHY
#if 1
    LibOs_SleepMiliSeconds(500);
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
    //LibUartHdl_SniffSetting(gIUT.hdl, 1);
    CMD_LE_Set_PHY(&gIUT, gIutConnHdl, 0, 0, 4, 0);
    EVT_LE_PHY_Update_Complete_Event(&gIUT, gIutConnHdl);
    LibOs_SleepMiliSeconds(500);
#endif

//ACL
#if 1
    {
        u8 buf[27 + 5] = {0x02, (u8)gTesterConnHdl, (u8)(gTesterConnHdl>>8), 27, 0};
        FOR_I(27) {
            buf[i+5]=0xA0+i;
        }
        _RAW_Send_(&gTester, buf, sizeof(buf));
        _RAW_Receive_(&gIUT);
        _RAW_Receive_(&gTester);
    }
#endif

    CMD_Disconnect(&gIUT, gIutConnHdl, 0x13);
    EVT_Disconnect_Complete(&gIUT, gIutConnHdl, 0x16);
    EVT_Disconnect_Complete(&gTester, gTesterConnHdl, 0x13);
}


void _TestCase_1_ResetALL(void)
{
    printf("%s() ...\n", __func__);

    CMD_RESET(&gIUT);
    CMD_RESET(&gTester);
    CMD_RESET(&gTester2);
}



void uart_init(void)
{
    gIUT.hdl = LibUartHdl_HandleCreate();
    //LibString_Copy(gIUT.aliasName, "IUT   ", 6);
    gIUT.rx_buf_len = 350;
    //LibUartMgr_GetComPortConfigFrom_INI_File("Uart_IUT.ini", gIUT.comName, &gIUT.baudRate);
    LibUartHdl_InitComPort(gIUT.hdl, gIUT.comName, gIUT.baudRate);
    //LibUartHdl_SniffSetting(gIUT.hdl, 1);

    gTester.hdl = LibUartHdl_HandleCreate();
    //LibString_Copy(gTester.aliasName, "Tstr_1", 6);
    gTester.rx_buf_len = 350;
    ///LibUartMgr_GetComPortConfigFrom_INI_File("Uart_Tester.ini", gTester.comName, &gTester.baudRate);
    LibUartHdl_InitComPort(gTester.hdl, gTester.comName, gTester.baudRate);
    //LibUartHdl_SniffSetting(gTester.hdl, 1);

/*
    gTester2.hdl = LibUartHdl_HandleCreate();
    LibString_Copy(gTester2.aliasName, "Tstr_2", 6);
    gTester2.rx_buf_len = 350;
    LibUartMgr_GetComPortConfigFrom_INI_File("Uart_Tester2.ini", gTester2.comName, &gTester2.baudRate);
    LibUartHdl_InitComPort(gTester2.hdl, gTester2.comName, gTester2.baudRate);
    //LibUartHdl_SniffSetting(gTester2.hdl, 1);
*/
#if ENABLE_UART_SNIFF
    LibUartHdl_SniffSetting(gIUT.hdl, 1);
    LibUartHdl_SniffSetting(gTester.hdl, 1);
//    LibUartHdl_SniffSetting(gTester2.hdl, 1);
#endif

    hci_uart_rx_init(&gIUT);
    hci_uart_rx_init(&gTester);
//    hci_uart_rx_init(&gTester2);
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



void ACL_2_Conn(void)
{
#define ACL_LENX (27+0)
    int increment = 37;
    int ctr = 0;
    printf("%s() ...\n", __func__);

    while (1) {
//ACL
#if 1
        if (gIutConnHdl != 0xFF)
        {
            u8 buf[ACL_LENX + 5] = {0x02, (u8)gIutConnHdl, (u8)(gIutConnHdl>>8), ACL_LENX, 0};
            FOR_I(ACL_LENX) {
                buf[i+5]=(u8)(increment*i+i);
            }
            _RAW_Send_(&gIUT, buf, sizeof(buf));
            _RAW_Receive_(&gIUT);
            _RAW_Receive_(&gTester, 0);
            if (gTester.rxPacket->len != ACL_LENX+5) {
                printf("len error!\n");
                BASIC_ASSERT(0);
            }
            if (0 != memcmp(&(buf[5]), &(gTester.rxPacket->buf[5]), ACL_LENX)) {
                printf("data error!\n");
                BASIC_ASSERT(0);
            }
            MM_FREE(gTester.rxPacket);
            increment += 97;
        }
        if (gTesterConnHdl != 0xFF)
        {
            u8 buf[ACL_LENX + 5] = {0x02, (u8)gTesterConnHdl, (u8)(gTesterConnHdl>>8), ACL_LENX, 0};
            FOR_I(ACL_LENX) {
                buf[i+5]=(u8)(increment*i+i);
            }
            _RAW_Send_(&gTester, buf, sizeof(buf));
            _RAW_Receive_(&gTester);
            _RAW_Receive_(&gIUT, 0);
            if (gIUT.rxPacket->len != ACL_LENX+5) {
                printf("len error!\n");
                BASIC_ASSERT(0);
            }
            if (0 != memcmp(&(buf[5]), &(gIUT.rxPacket->buf[5]), ACL_LENX)) {
                printf("data error!\n");
                BASIC_ASSERT(0);
            }
            MM_FREE(gIUT.rxPacket);
            increment += 97;
        }
#endif
#if 1
        if (gIutConnHdl2 != 0xFF)
        {
            u8 buf[ACL_LENX + 5] = {0x02, (u8)gIutConnHdl2, (u8)(gIutConnHdl2>>8), ACL_LENX, 0};
            FOR_I(ACL_LENX) {
                buf[i+5]=(u8)(increment*i+i);
            }
            _RAW_Send_(&gIUT, buf, sizeof(buf));
            _RAW_Receive_(&gIUT);
            _RAW_Receive_(&gTester2, 0);
            if (gTester2.rxPacket->len != ACL_LENX+5) {
                printf("len error!\n");
                BASIC_ASSERT(0);
            }
            if (0 != memcmp(&(buf[5]), &(gTester2.rxPacket->buf[5]), ACL_LENX)) {
                printf("data error!\n");
                BASIC_ASSERT(0);
            }
            MM_FREE(gTester2.rxPacket);
            increment += 97;
        }
        if (gTester2ConnHdl != 0xFF)
        {
            u8 buf[ACL_LENX + 5] = {0x02, (u8)gTester2ConnHdl, (u8)(gTester2ConnHdl>>8), ACL_LENX, 0};
            FOR_I(ACL_LENX) {
                buf[i+5]=(u8)(increment*i+i);
            }
            _RAW_Send_(&gTester2, buf, sizeof(buf));
            _RAW_Receive_(&gTester2);
            _RAW_Receive_(&gIUT, 0);
            if (gIUT.rxPacket->len != ACL_LENX+5) {
                printf("len error!\n");
                BASIC_ASSERT(0);
            }
            if (0 != memcmp(&(buf[5]), &(gIUT.rxPacket->buf[5]), ACL_LENX)) {
                printf("data error!\n");
                BASIC_ASSERT(0);
            }
            MM_FREE(gIUT.rxPacket);
            increment += 97;
        }
#endif
        NEWLINE;
        LibTime_PrintLocalTime();
        DUMPND(ctr);
        ctr++;
        LibOs_SleepMiliSeconds(200);
    }


}



void UartTester_Main(int test_case_index)
{
    //msg_buf_init();
    uart_init();
    //thread_init();

#if BUILD_2_ACL
    ACL_2_Conn();
#else
    _TestCase_0_CreateConnection();
#endif
#if 0
    switch (test_case_index) {
        case 0:
            _TestCase_0_CreateConnection();
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

#define HEX2INT(x,c) if(c>='a'&&c<='f'){x=c-'a'+10;}else if(c>='A'&&c<='F'){x=c-'A'+10;}else{x=c-'0';}
int main(int argc, char *argv[])
{
    Lib_Init(LIB_MT_ENABLE);

    do {
        if (argc != 4) {
            //printf("arguments error! do nothing ...\n");
            //break;
        }
        //DUMPND(test_case_index);

        #if BUILD_2_ACL
        if (argc == 5)
        {
            int temp, temp2;
            HEX2INT(temp, argv[1][0]);
            HEX2INT(temp2, argv[1][1]);
            gIutConnHdl = (temp << 4) + temp2;
            HEX2INT(tebmp, argv[2][0]);
            HEX2INT(temp2, argv[2][1]);
            gTesterConnHdl = (temp << 4) + temp2;

            HEX2INT(temp, argv[3][0]);
            HEX2INT(temp2, argv[3][1]);
            gIutConnHdl2 = (temp << 4) + temp2;
            HEX2INT(temp, argv[4][0]);
            HEX2INT(temp2, argv[4][1]);
            gTester2ConnHdl = (temp << 4) + temp2;
            DUMPNX(gIutConnHdl);
            DUMPNX(gIutConnHdl2);
            DUMPNX(gTesterConnHdl);
            DUMPNX(gTester2ConnHdl);
        } else {
            printf("argc is not 5, exit.\n");
            break;
        }
        #else
        if (argc != 2) {
            printf("ERROR: argc=%d.   Example: aout.exe t1.ini\n", argc);
            break;
        }
        else
        {
            LibFile_INI ini_file(argv[1], "r+b");
            EXIT_CHK( rc, ini_file.StartParse() );
            //ini_file.Dump();
            EXIT_CHK( rc, ini_file.GetValueU32("[info]", "CONN_INTERVAL", gCONN_INTERVAL) );
            DUMPNX(gCONN_INTERVAL);
            EXIT_CHK( rc, ini_file.GetValueU32("[info]", "CONN_LATENCY", gCONN_LATENCY) );
            DUMPNX(gCONN_LATENCY);
            EXIT_CHK( rc, ini_file.GetValueU32("[info]", "CONN_TIMEOUT", gCONN_TIMEOUT) );
            DUMPNX(gCONN_TIMEOUT);

            EXIT_CHK( rc, ini_file.GetValueString("[info]", "IUT_NAME", gIUT.aliasName); );
            DUMPNS(gIUT.aliasName);
            EXIT_CHK( rc, ini_file.GetValueString("[info]", "IUT_ComPortName", gIUT.comName); );
            DUMPNS(gIUT.comName);
            EXIT_CHK( rc, ini_file.GetValueU32("[info]", "IUT_BaudRate", gIUT.baudRate); );
            DUMPND(gIUT.baudRate);

            EXIT_CHK( rc, ini_file.GetValueString("[info]", "TESTER_NAME", gTester.aliasName); );
            DUMPNS(gTester.aliasName);
            EXIT_CHK( rc, ini_file.GetValueString("[info]", "TESTER_ComPortName", gTester.comName); );
            DUMPNS(gTester.comName);
            EXIT_CHK( rc, ini_file.GetValueU32("[info]", "TESTER_BaudRate", gTester.baudRate); );
            DUMPND(gTester.baudRate);

            EXIT_CHK( rc, ini_file.GetValueU32("[info]", "SKIP_ADV", gSkipAdv); );
            DUMPND(gSkipAdv);
        }
        {
            LibFile_INI ini_file("bt.ini", "r+b");
            EXIT_CHK( rc, ini_file.StartParse() );

            EXIT_CHK( rc, ini_file.GetValueU32("[bt_info]", "IUT_BD_ADDR_0", gIUT_BD_ADDR[0]) );
            EXIT_CHK( rc, ini_file.GetValueU32("[bt_info]", "IUT_BD_ADDR_1", gIUT_BD_ADDR[1]) );
            EXIT_CHK( rc, ini_file.GetValueU32("[bt_info]", "IUT_BD_ADDR_2", gIUT_BD_ADDR[2]) );
            EXIT_CHK( rc, ini_file.GetValueU32("[bt_info]", "IUT_BD_ADDR_3", gIUT_BD_ADDR[3]) );
            EXIT_CHK( rc, ini_file.GetValueU32("[bt_info]", "IUT_BD_ADDR_4", gIUT_BD_ADDR[4]) );
            EXIT_CHK( rc, ini_file.GetValueU32("[bt_info]", "IUT_BD_ADDR_5", gIUT_BD_ADDR[5]) );
            ARRAYDUMPX2(gIUT_BD_ADDR, 6);
        }
        #endif
        UartTester_Main(0);
        
    } while (0);

    Lib_Uninit();
    return 0;
}


