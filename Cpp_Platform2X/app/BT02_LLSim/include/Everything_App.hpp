

#ifndef _EVERYTHING_APP_HPP_INCLUDED_


#include "Everything_Lib_Mgr.hpp"

#include "SimAir.hpp"

#include "cmn_le_lm_sla.h"
#include "cmn_le_lm_mas.h"
#include "scheduler.h"

void Slave_Upper_InitSimAir(void);
void Slave_Upper_InitTest(void);
void Master_Upper_InitSimAir(void);
void Master_Upper_InitTest(void);

enum
{
    SIM_AIR_TASK_CLKN,
    SIM_AIR_TASK_CLKB,
    SIM_AIR_TASK_0_TRX,
    SIM_AIR_TASK_1_TRX,
    SIM_AIR_TASK_SCH_0,
    SIM_AIR_TASK_SCH_1,
    SIM_AIR_TASK_SCH_2,
    SIM_AIR_TASK_SCH_3,
    SIM_AIR_TASK_TIMER_0,
    SIM_AIR_TASK_TIMER_1,
    SIM_AIR_TASK_TIMER_2,
    SIM_AIR_TASK_TIMER_3,

    SIM_AIR_TASK_NUMBER,
};

extern LibFileIoClass g_dump_master;
extern LibFileIoClass g_dump_slave;
extern LibFileIoClass g_dump_all;
#define MASTER_DUMP(...) fprintf(g_dump_master.fp, ##__VA_ARGS__);fprintf(g_dump_all.fp, ##__VA_ARGS__);
#define SLAVE_DUMP(...) fprintf(g_dump_slave.fp, ##__VA_ARGS__);fprintf(g_dump_all.fp, ##__VA_ARGS__);
#define NORMALIZE_TIME1 info->response.ref_clock_L/10,info->response.ref_clock_L%10
#define NORMALIZE_TIME2 SimAir_TimeStamp_Low_Get()/10,SimAir_TimeStamp_Low_Get()%10

#define _EVERYTHING_APP_HPP_INCLUDED_
#endif//_EVERYTHING_APP_HPP_INCLUDED_
