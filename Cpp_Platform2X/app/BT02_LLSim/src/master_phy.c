
#include "Everything_App.hpp"

extern SimAir_Info_t g_master_info[SIM_AIR_TASK_NUMBER];

Bt_Dev_Info_t *g_dev_mas_timer_00;
u32 g_sleep_time_in_us;

void Master_Timer_0(Bt_Dev_Info_t *mas_dev, u32 sleep_time_in_us)
{
    g_dev_mas_timer_00 = mas_dev;
    g_sleep_time_in_us = sleep_time_in_us;
    g_master_info[SIM_AIR_TASK_TIMER_0].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info[SIM_AIR_TASK_TIMER_0].next_wake_up_time = sleep_time_in_us * g_master_info[SIM_AIR_TASK_TIMER_0].clocks_per_bit;
    SimAir_Request(&(g_master_info[SIM_AIR_TASK_TIMER_0]));
}

int Master_Phy_Wake_Timer0(SimAir_Info_t *info)
{
    MASTER_DUMP1(" timer0 wake up, sleep_time_in_us=%d\n", g_sleep_time_in_us);

    printf("%s() -- %d\n", __func__, info->response.ref_clock_L);
    {
        extern void lc_mas_conn_state_machine(Bt_Dev_Info_t *mas_dev, LC_CONNECTION_STATE_EVENT_t evt);
        lc_mas_conn_state_machine(g_dev_mas_timer_00, LC_CONN_STT_EVT_SLEEP_TIMESUP);
    }

    return 0;
}

int Master_Phy_Wake_TRx0(SimAir_Info_t *info)
{
    printf("%s() -- %d\n", __func__, info->response.ref_clock_L);

    return 0;
}

int Master_Phy_Rx_0(SimAir_Info_t *info)
{
    printf("%s() -- %d, resp_type:%d, rx_done_bits:%d\n", __func__, info->response.ref_clock_L, info->response.resp_type, info->response.rx_done_bits);

    return 0;
}

int Master_Phy_Tx_0(SimAir_Info_t *info)
{
    printf("%s() -- %d\n", __func__, info->response.ref_clock_L);

    return 0;
}

#define CLOCKS_PER_BT_CLOCK 3125
#define TOTAL_SIM_BT_CLOCKS 30
u32 g_sim_time_in_clock = TOTAL_SIM_BT_CLOCKS * CLOCKS_PER_BT_CLOCK;
u32 g_sim_time_curr = 0;
u32 g_sim_timesup = 0;
u32 g_master_clkn = 0;
u32 g_master_clkb = 0;
int Master_Phy_Wake_CLKN(SimAir_Info_t *info)
{
    g_master_clkn++;
    //printf("%s() -- %d, clkn:%d\n", __func__, info->response.ref_clock_L, g_master_clkn);
    //MASTER_DUMP("N[ MAS ][ CLK N ][%8d][%5d]\n", info->response.ref_clock_L, g_master_clkn);
    MASTER_DUMPn1(" clkn:%d\n", g_master_clkn);

    BASIC_ASSERT(CLOCKS_PER_BT_CLOCK == SimAir_TimeStamp_Low_Get() - g_sim_time_curr)
    g_sim_time_curr += CLOCKS_PER_BT_CLOCK;
    if (g_sim_time_curr >= g_sim_time_in_clock) {
        g_sim_timesup = 1;
    }

    if (g_sim_timesup)
        return 0;

    g_master_info[SIM_AIR_TASK_CLKN].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info[SIM_AIR_TASK_CLKN].next_wake_up_time = CLOCKS_PER_BT_CLOCK;
    SimAir_Request(&(g_master_info[SIM_AIR_TASK_CLKN]));

    return 0;
}

int Master_Phy_Wake_CLKB(SimAir_Info_t *info)
{
    g_master_clkb++;
    //printf("%s() -- %d, clkb:%d\n", __func__, info->response.ref_clock_L, g_master_clkb);
    //MASTER_DUMP("B[ MAS ][ CLK B ][%8d][%5d]\n", info->response.ref_clock_L, g_master_clkb);
    MASTER_DUMPb1(" clkb:%d\n", g_master_clkb);

    if (g_sim_timesup)
        return 0;

    g_master_info[SIM_AIR_TASK_CLKB].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info[SIM_AIR_TASK_CLKB].next_wake_up_time = CLOCKS_PER_BT_CLOCK;
    SimAir_Request(&(g_master_info[SIM_AIR_TASK_CLKB]));
    return 0;
}

Bt_Dev_Info_t *g_dev_mas_sch_00;
Scheduler_Request_T *g_sch_requ_mas_sch_00;
void master_sch_0_add_request(Bt_Dev_Info_t *mas_dev, Scheduler_Request_T *p_sch_requ)
{
    u32 sch_delay_in_us = scheduler_simulate_delay_in_us();
    g_dev_mas_sch_00 = mas_dev;
    g_sch_requ_mas_sch_00 = p_sch_requ;

    //DUMPD(g_sch_requ_mas_sch_00->periodical_interval_us);
    g_sch_requ_mas_sch_00->sim_state = SIM_SCH_STT_WAITING_1ST_WAKUP_TO_GRANT;

    g_master_info[SIM_AIR_TASK_SCH_0].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info[SIM_AIR_TASK_SCH_0].next_wake_up_time = sch_delay_in_us * g_master_info[SIM_AIR_TASK_SCH_0].clocks_per_bit;
    //DUMPD(g_master_info[SIM_AIR_TASK_SCH_0].next_wake_up_time);
    MASTER_DUMP2(" Sch 0 add request, sch_delay_in_us = %d\n", sch_delay_in_us);
    SimAir_Request(&(g_master_info[SIM_AIR_TASK_SCH_0]));
}

int Master_Phy_Wake_Sch0(SimAir_Info_t *info)
{
    if (g_sim_timesup)
        return 0;

    //printf(" [ MAS ][ Sch 0]%s() -- %d\n", __func__, info->response.ref_clock_L);
    //MASTER_DUMP(" [ MAS ][ Sch 0 ][%8d]\n", info->response.ref_clock_L);
    MASTER_DUMP1(" Sch 0 wake up\n");

    //if (g_sch_requ_mas_sch_00->sim_state == SIM_SCH_STT_WAITING_1ST_WAKUP_TO_GRANT)
    {
        extern void lc_mas_conn_state_machine(Bt_Dev_Info_t *mas_dev, LC_CONNECTION_STATE_EVENT_t evt);
        lc_mas_conn_state_machine(g_dev_mas_sch_00, LC_CONN_STT_EVT_SCH_GRANT);
    }

    g_master_info[SIM_AIR_TASK_SCH_0].requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info[SIM_AIR_TASK_SCH_0].next_wake_up_time = g_sch_requ_mas_sch_00->periodical_interval_us * g_master_info[SIM_AIR_TASK_SCH_0].clocks_per_bit;
    SimAir_Request(&(g_master_info[SIM_AIR_TASK_SCH_0]));

    return 0;
}
