

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
#define NORMALIZE_TIME1 info->response.ref_clock_L/10,info->response.ref_clock_L%10
#define NORMALIZE_TIME2 SimAir_TimeStamp_Low_Get()/10,SimAir_TimeStamp_Low_Get()%10
#define HEAD(a,b) a ## "[" ## b ## "][%7d.%d]"
#define DUMP_TO_ALL_DUMP(fp, ...) fprintf(fp, ##__VA_ARGS__);fprintf(g_dump_all.fp, ##__VA_ARGS__)
//#define MAS_TO_ALL_DUMP(...) DUMP_TO_ALL_DUMP(g_dump_master.fp, ##__VA_ARGS__)
//#define SLA_TO_ALL_DUMP(...) DUMP_TO_ALL_DUMP(g_dump_slave.fp, ##__VA_ARGS__)
#define MAS_DUMP(fp, h1, h2, format, ...) DUMP_TO_ALL_DUMP(fp, HEAD(h1, h2) format, ##__VA_ARGS__)

#define MASTER_DUMP(a, ...)  fprintf(g_dump_master.fp, "-[MAS][%7d.%d]" a, ##__VA_ARGS__);fprintf(g_dump_all.fp, "-[MAS][%7d.%d]" a, ##__VA_ARGS__);
#define MASTER_DUMPn(a, ...) fprintf(g_dump_master.fp, "N[MAS][%7d.%d]" a, ##__VA_ARGS__);fprintf(g_dump_all.fp, "N[MAS][%7d.%d]" a, ##__VA_ARGS__);
#define MASTER_DUMPb(a, ...) fprintf(g_dump_master.fp, "B[MAS][%7d.%d]" a, ##__VA_ARGS__);fprintf(g_dump_all.fp, "B[MAS][%7d.%d]" a, ##__VA_ARGS__);
#define MASTER_DUMP1(a, ...)  fprintf(g_dump_master.fp, "-[MAS][%7d.%d]" a, NORMALIZE_TIME1, ##__VA_ARGS__);fprintf(g_dump_all.fp, "-[MAS][%7d.%d]" a, NORMALIZE_TIME1, ##__VA_ARGS__);
#define MASTER_DUMPn1(a, ...) fprintf(g_dump_master.fp, "N[MAS][%7d.%d]" a, NORMALIZE_TIME1, ##__VA_ARGS__);fprintf(g_dump_all.fp, "N[MAS][%7d.%d]" a, NORMALIZE_TIME1, ##__VA_ARGS__);
#define MASTER_DUMPb1(a, ...) fprintf(g_dump_master.fp, "B[MAS][%7d.%d]" a, NORMALIZE_TIME1, ##__VA_ARGS__);fprintf(g_dump_all.fp, "B[MAS][%7d.%d]" a, NORMALIZE_TIME1, ##__VA_ARGS__);
#define MASTER_DUMP2(a, ...)  fprintf(g_dump_master.fp, "-[MAS][%7d.%d]" a, NORMALIZE_TIME2, ##__VA_ARGS__);fprintf(g_dump_all.fp, "-[MAS][%7d.%d]" a, NORMALIZE_TIME2, ##__VA_ARGS__);
#define MASTER_DUMPn2(a, ...) fprintf(g_dump_master.fp, "N[MAS][%7d.%d]" a, NORMALIZE_TIME2, ##__VA_ARGS__);fprintf(g_dump_all.fp, "N[MAS][%7d.%d]" a, NORMALIZE_TIME2, ##__VA_ARGS__);
#define MASTER_DUMPb2(a, ...) fprintf(g_dump_master.fp, "B[MAS][%7d.%d]" a, NORMALIZE_TIME2, ##__VA_ARGS__);fprintf(g_dump_all.fp, "B[MAS][%7d.%d]" a, NORMALIZE_TIME2, ##__VA_ARGS__);
#define SLAVE_DUMP(a, ...)  fprintf(g_dump_slave.fp, "-[SLA][%7d.%d]" a, ##__VA_ARGS__);fprintf(g_dump_all.fp, "-[SLA][%7d.%d]" a, ##__VA_ARGS__);
#define SLAVE_DUMPn(a, ...) fprintf(g_dump_slave.fp, "N[SLA][%7d.%d]" a, ##__VA_ARGS__);fprintf(g_dump_all.fp, "N[SLA][%7d.%d]" a, ##__VA_ARGS__);
#define SLAVE_DUMPb(a, ...) fprintf(g_dump_slave.fp, "B[SLA][%7d.%d]" a, ##__VA_ARGS__);fprintf(g_dump_all.fp, "B[SLA][%7d.%d]" a, ##__VA_ARGS__);
#define SLAVE_DUMP1(a, ...)  fprintf(g_dump_master.fp, "-[SLA][%7d.%d]" a, NORMALIZE_TIME1, ##__VA_ARGS__);fprintf(g_dump_all.fp, "-[SLA][%7d.%d]" a, NORMALIZE_TIME1, ##__VA_ARGS__);
#define SLAVE_DUMPn1(a, ...) fprintf(g_dump_master.fp, "N[SLA][%7d.%d]" a, NORMALIZE_TIME1, ##__VA_ARGS__);fprintf(g_dump_all.fp, "N[SLA][%7d.%d]" a, NORMALIZE_TIME1, ##__VA_ARGS__);
#define SLAVE_DUMPb1(a, ...) fprintf(g_dump_master.fp, "B[SLA][%7d.%d]" a, NORMALIZE_TIME1, ##__VA_ARGS__);fprintf(g_dump_all.fp, "B[SLA][%7d.%d]" a, NORMALIZE_TIME1, ##__VA_ARGS__);
#define SLAVE_DUMP2(a, ...)  fprintf(g_dump_master.fp, "-[SLA][%7d.%d]" a, NORMALIZE_TIME2, ##__VA_ARGS__);fprintf(g_dump_all.fp, "-[SLA][%7d.%d]" a, NORMALIZE_TIME2, ##__VA_ARGS__);
#define SLAVE_DUMPn2(a, ...) fprintf(g_dump_master.fp, "N[SLA][%7d.%d]" a, NORMALIZE_TIME2, ##__VA_ARGS__);fprintf(g_dump_all.fp, "N[SLA][%7d.%d]" a, NORMALIZE_TIME2, ##__VA_ARGS__);
#define SLAVE_DUMPb2(a, ...) fprintf(g_dump_master.fp, "B[SLA][%7d.%d]" a, NORMALIZE_TIME2, ##__VA_ARGS__);fprintf(g_dump_all.fp, "B[SLA][%7d.%d]" a, NORMALIZE_TIME2, ##__VA_ARGS__);

#define _EVERYTHING_APP_HPP_INCLUDED_
#endif//_EVERYTHING_APP_HPP_INCLUDED_
