
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
    Init_Dump_Files();

    Master_Upper_InitSimAir();
    Slave_Upper_InitSimAir();

    {
        extern void lc_conn_state_init(void);
        lc_conn_state_init();
    }

    Master_Upper_InitTest();
    Slave_Upper_InitTest();

    SimAir_Start();
    SimAir_Uninit();

    return 0;
}
