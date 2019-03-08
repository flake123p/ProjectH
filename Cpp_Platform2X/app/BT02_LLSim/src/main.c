
#include "Everything_App.hpp"

LibFileIoClass g_dump_master = LibFileIoClass("dump_master.txt", "w+b");
LibFileIoClass g_dump_slave = LibFileIoClass("dump_slave.txt", "w+b");
LibFileIoClass g_dump_all = LibFileIoClass("dump_all.txt", "w+b");

void Init_Dump_Files(void)
{
    g_dump_master.FileOpen();
    g_dump_slave.FileOpen();
    g_dump_all.FileOpen();
}

int main(int argc, char *argv[])
{
    SimAir_Log_Enable(false, true);
    BT_Phy_Log_Enable();

    Init_Dump_Files();

    Peer0_InitSimAir();
    Peer1_InitSimAir();

    Peer0_StartTest();
    Peer1_StartTest();

    SimAir_Start();
    SimAir_Uninit();

    SimAir_Log_Disable();
    BT_Phy_Log_Disable();
    return 0;
}
