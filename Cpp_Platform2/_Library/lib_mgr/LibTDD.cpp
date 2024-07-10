
#include "Everything_Lib_Mgr.hpp"

#define dc (2)

/*
master:
    TX TIFS enable after TX DONE always!!
    RX TIFS enable after RX OK if MD_FLAG

                 TX still has data
                 |  RX timeout?
                 |  |  RX CRC error?
                 |  |  |  RX more data?
                 |  |  |  |            [OUT]TX TIFS in TX DONE
                 |  |  |  |                 |  RX TIFS in RX DONE
                 |  |  |  |                 |  |  TX TIFS stop in RX DONE
*/
u8 test[][7] = {{0, 0, 0, 0,                1, 0, 1,},
                {0, 0, 0, 1,                1, 1, 0,},
                {0, 0, 1,dc,                1, 0, 1,},
                {0, 1,dc,dc,                1, 0, 1,},
                {1, 0, 0, 0,                1, 1, 0,},
                {1, 0, 0, 1,                1, 1, 0,},
                {1, 0, 1,dc,                1, 0, 1,},
                {1, 1,dc,dc,                1, 0, 1,},
};

int is_md_mode;
int do_tx_tifs_at_tx_done;
int do_rx_tifs_at_rx_done;
int do_tx_tifs_stop_at_rx_done;
void mas_reset(void)
{
    is_md_mode = 0;
    do_tx_tifs_at_tx_done = 9999;
    do_rx_tifs_at_rx_done = 9999;
    do_tx_tifs_stop_at_rx_done = 9999;
}
void mas_tx_prepare(int is_tx_still_has_data)
{
    if (is_tx_still_has_data) {
        is_md_mode = 1;
    } else {
        is_md_mode = 0;
    }
}

void mas_tx_done(void)
{
    do_tx_tifs_at_tx_done = 1;
}

void mas_rx_ok(int is_rx_more_data)
{
    if (is_rx_more_data) {
        is_md_mode = 1;
    }

    if (is_md_mode == 1) {
        do_rx_tifs_at_rx_done = 1;
        do_tx_tifs_stop_at_rx_done = 0;
    } else if (is_md_mode == 0) {
        do_rx_tifs_at_rx_done = 0;
        do_tx_tifs_stop_at_rx_done = 1;
    } else {
        do_rx_tifs_at_rx_done = 5555;
        do_tx_tifs_stop_at_rx_done = 5555;
    }
}

void mas_rx_time_out(void)
{
    do_rx_tifs_at_rx_done = 0;
    do_tx_tifs_stop_at_rx_done = 1;
}

void mas_rx_crc_error(void)
{
    do_rx_tifs_at_rx_done = 0;
    do_tx_tifs_stop_at_rx_done = 1;
}

int LibTDD_Demo(void)
{
    FOR_I(8) {
        mas_reset();
        mas_tx_prepare(test[i][0]);
        mas_tx_done();
        if (test[i][1]) {
            mas_rx_time_out();
        } else if (test[i][2]) {
            mas_rx_crc_error();
        } else {
            mas_rx_ok(test[i][3]);
        }
        if (test[i][4] != do_tx_tifs_at_tx_done) {
            printf("error in case:[%d][4], result is %d, should be %d.\n", i, do_tx_tifs_at_tx_done, test[i][4]);
            break;
        }
        if (test[i][5] != do_rx_tifs_at_rx_done) {
            printf("error in case:[%d][5], result is %d, should be %d.\n", i, do_rx_tifs_at_rx_done, test[i][5]);
            break;
        }
        if (test[i][6] != do_tx_tifs_stop_at_rx_done) {
            printf("error in case:[%d][6], result is %d, should be %d.\n", i, do_tx_tifs_stop_at_rx_done, test[i][6]);
            break;
        }
    }
    printf("TEST DONE\n");
    return 0;
}

/*
slave:
    RX TIFS enable after RX OK always!!
    TX TIFS enable after TX DONE if md flag ON

                  RX timeout?
                  |  RX CRC error?
                  |  |  RX more data?
                  |  |  |  TX still has data
                  |  |  |  |                [OUT]RX TIFS in RX DONE
                  |  |  |  |                     |  TX STOP
                  |  |  |  |                     |  |  TX TIFS in TX DONE
                  |  |  |  |                     |  |  |  RX TIFS stop in TX DONE
*/
u8 testS[][8] = {{0, 0, 0, 0,                    1, 0, 0, 1,},
                 {0, 0, 0, 1,                    1, 0, 1, 0,},
                 {0, 0, 1, 0,                    1, 0, 1, 0,},
                 {0, 0, 1, 1,                    1, 0, 1, 0,},
                 {0, 1,dc, 0,                    0, 0, 0, 1,},
                 {0, 1,dc, 1,                    0, 0, 0, 1,},
                 {1,dc,dc,dc,                    0, 1,dc,dc,},
};

int is_crc_error;
int do_tx_stop;
int do_rx_tifs_stop_at_tx_done;
void sla_reset(void)
{
    is_crc_error = 9999;
    is_md_mode = 9999;
    do_tx_tifs_at_tx_done = 9999;
    do_rx_tifs_at_rx_done = 9999;
    do_rx_tifs_stop_at_tx_done = 9999;
    do_tx_stop = 9999;
}

void sla_rx_time_out(void)
{
    is_md_mode = 0;
    do_rx_tifs_at_rx_done = 0;
    do_tx_stop = 1;
    is_crc_error = 0;
}

void sla_rx_crc_error(void)
{
    is_md_mode = 0;
    do_rx_tifs_at_rx_done = 0;
    do_tx_stop = 0;
    is_crc_error = 1;
}

void sla_rx_ok(int is_rx_more_data)
{
    if (is_rx_more_data)
        is_md_mode = 1;
    else
        is_md_mode = 0;

    do_rx_tifs_at_rx_done = 1;
    do_tx_stop = 0;
    is_crc_error = 0;
}

void sla_tx_prepare(int is_tx_still_has_data)
{
    if (do_tx_stop)
        return;

    BASIC_ASSERT(is_crc_error == 1 || is_crc_error == 0);
    if (is_tx_still_has_data && is_crc_error != 1) {
        is_md_mode = 1;
    } else {
        // nothing happened, don't change is_md_mode
    }
}

void sla_tx_done(void)
{
    if (do_tx_stop)
        return;

    if (is_md_mode) {
        do_tx_tifs_at_tx_done = 1;
        do_rx_tifs_stop_at_tx_done = 0;
    } else {
        do_tx_tifs_at_tx_done = 0;
        do_rx_tifs_stop_at_tx_done = 1;
    }
}

int LibTDD_Demo2(void)
{
    FOR_I(7) {
        sla_reset();
        if (testS[i][0]) {
            sla_rx_time_out();
        } else if (testS[i][1]) {
            sla_rx_crc_error();
        } else {
            sla_rx_ok(testS[i][2]);
        }
        sla_tx_prepare(testS[i][3]);
        sla_tx_done();
        if (testS[i][4] != do_rx_tifs_at_rx_done) {
            printf("error in case:[%d][4], result is %d, should be %d.\n", i, do_rx_tifs_at_rx_done, testS[i][4]);
            break;
        }
        if (testS[i][5] != do_tx_stop) {
            printf("error in case:[%d][5], result is %d, should be %d.\n", i, do_tx_stop, testS[i][5]);
            break;
        }
        if (testS[i][6] != dc && testS[i][6] != do_tx_tifs_at_tx_done) {
            printf("error in case:[%d][6], result is %d, should be %d.\n", i, do_tx_tifs_at_tx_done, testS[i][6]);
            break;
        }
        if (testS[i][7] != dc && testS[i][7] != do_rx_tifs_stop_at_tx_done) {
            printf("error in case:[%d][7], result is %d, should be %d.\n", i, do_rx_tifs_stop_at_tx_done, testS[i][7]);
            break;
        }
    }
    printf("TEST2 DONE\n");
    return 0;
}