
#include "Everything_App.hpp"

int main(int argc, char *argv[])
{
    Slave_Upper_InitSimAir();
    Master_Upper_InitSimAir();

    SimAir_Start();
    SimAir_Uninit();

    return 0;
}

