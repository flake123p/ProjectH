

#include "Everything_App.hpp"

UART_HANDLE_t gUartIUT;

void UartTester_Main(int test_case_index)
{
    char comName[50];
    uint32_t baudRate;
    gUartIUT = LibUartHdl_HandleCreate();

    LibUartMgr_GetComPortConfigFrom_INI_File("Uart_IUT.ini", comName, &baudRate);
    LibUartHdl_InitComPort(gUartIUT, comName, baudRate);
    LibUartHdl_SniffSetting(gUartIUT, 1);
    u8 cmd_r[] = {0x01, 0x03, 0x0C, 0x00};
    u8 rx_buf[80];
    u32 rx_len;
    LibUartHdl_Send(gUartIUT, cmd_r, sizeof(cmd_r));
    LibUartHdl_ReceiveEx(gUartIUT, rx_buf, &rx_len, 80);
    DUMPND(rx_len);

    LibUartHdl_UninitComPort(gUartIUT);
    LibUartHdl_HandleDestroy(gUartIUT);
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


