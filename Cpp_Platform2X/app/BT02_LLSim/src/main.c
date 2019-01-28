
#include "Everything_App.hpp"

int main(int argc, char *argv[])
{
    Master_Upper_InitSimAir();
    Slave_Upper_InitSimAir();

    Master_Upper_InitTest();
    Slave_Upper_InitTest();

    SimAir_Start();
    SimAir_Uninit();

    return 0;
}

