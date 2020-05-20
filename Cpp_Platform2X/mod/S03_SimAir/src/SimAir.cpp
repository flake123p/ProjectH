
#include "Everything_SimAir.hpp"
#include "SimAir.hpp"
#include <vector>

#if SIM_AIR_LOG
LibFileIoClass g_sim_air_log = LibFileIoClass("log_sim_air.txt", "w+b");
bool g_sim_air_log_enable = false;
bool g_sim_air_log_enable_every_clock_dump =false;
bool g_sim_air_log_only_dump_tx_rx_done =false;
#endif //SIM_AIR_LOG

typedef enum {
    SIM_AIR_WAKEUP_CB = 100,
    SIM_AIR_INTERNAL_TRx,
}SimAir_CB_Type_t;

typedef struct {
    SimAir_Handle_t sim_air_hdl;
    const char *sim_air_id_str;
    SimAir_CB_Type_t cb_type;
    SimAir_CB_t wake_up_cb;
    SimAir_CB_t rxing_cb; //including last rx done
    SimAir_CB_t txing_cb; //including last tx done
    u32 vector_index; //for debug, develop
    Time_Slice_Descriptor2 *p_time_slice_descriptor;
    SimAir_Info_t *requ_resp_info;
}SimAir_Descriptor_t;

static std::vector<SimAir_Descriptor_t *> gSimAirDes_Vector;
static SimAir_Handle_t gSimAir_Handle_Ctr = 0;
static SimAir_Handle_t gSimAir_Handle_InternalTRx = 0;
//static Time_Slice_Descriptor2 *gSimAir_Descriptor2_Last = NULL;

const char *SimAir_Handle_ID_String_Get(u32 index)
{
    BASIC_ASSERT(index < gSimAirDes_Vector.size());
    if (gSimAirDes_Vector[index]->sim_air_id_str == NULL)
        return "(NULL)";
    return gSimAirDes_Vector[index]->sim_air_id_str;
}

static void SimAir_ActivePeriodicInternalTRx(void)
{
    if (gSimAirDes_Vector[gSimAir_Handle_InternalTRx]->p_time_slice_descriptor->state == TIME_SLICE_CB_DONE)
    {
        gSimAirDes_Vector[gSimAir_Handle_InternalTRx]->p_time_slice_descriptor->remain_time = 1;
        gSimAirDes_Vector[gSimAir_Handle_InternalTRx]->p_time_slice_descriptor->state = TIME_SLICE_CB_WAITING;
    }
}

static int SimAir_InternalTRx(SimAir_Info_t *info)
{
//    u32 clk_h, clk_l;
//    SimTimeSlice_TimeStampGet(&clk_h, &clk_l);
//    printf("%s() -- %d\n", __func__, clk_l);

    SimAir_ChMgr_ProcessOneTick();

    if (SimAir_ChMgr_CheckDoesNeedToTick()) {
        SimAir_ActivePeriodicInternalTRx();
    }
    return 0;
}

static int SimAir_Dispatcher(SimAir_Descriptor_t* p_des)
{
    //printf("%s() hdl:%d, cb_type:%d\n", __func__, p_des->sim_air_hdl, p_des->cb_type);

#if SIM_AIR_LOG
    if (g_sim_air_log_enable) {
        u32 clk_h, clk_l;
        SimTimeSlice_TimeStampGet(&clk_h, &clk_l);

        const char *cb_type_str;
        switch (p_des->cb_type) {
            case SIM_AIR_WAKEUP_CB:
                cb_type_str="wake";
                fprintf(g_sim_air_log.fp, "[%8d][%4s][------]%s ... request at %d\n", 
                    clk_l,
                    cb_type_str,
                    SimAir_Handle_ID_String_Get(p_des->sim_air_hdl),
                    p_des->requ_resp_info->request_time_l);
                break;
            case SIM_AIR_INTERNAL_TRx:
                cb_type_str="i_TR";
                break;
            default:
                cb_type_str="????";
                break;
        }
    }
#endif //SIM_AIR_LOG

    switch (p_des->cb_type) {
        case SIM_AIR_WAKEUP_CB:
            BASIC_ASSERT(p_des->requ_resp_info != NULL);
            SimTimeSlice_TimeStampGet(&p_des->requ_resp_info->response.ref_clock_H, &p_des->requ_resp_info->response.ref_clock_L);
            p_des->requ_resp_info->response.resp_type = SIM_AIR_WAKEUP_TIMESUP;
            if (p_des->wake_up_cb != NULL) {
                p_des->wake_up_cb(p_des->requ_resp_info);
            }
            break;

        case SIM_AIR_INTERNAL_TRx:
            BASIC_ASSERT(p_des->requ_resp_info == NULL);
            SimAir_InternalTRx(p_des->requ_resp_info);
            break;

        default:
            DUMPD(p_des->cb_type);
            BASIC_ASSERT(0);
            break;
    }

    return 0;
}

u32 SimAir_TimeStamp_Low_Get(void)
{
    return SimTimeSlice_TimeStamp_Low_Get();
}

u32 SimAir_TimeStamp_High_Get(void)
{
    return SimTimeSlice_TimeStamp_High_Get();
}

SimAir_Handle_t SimAir_Init_AddDescriptor(SimAir_CB_t wake_up_cb, SimAir_CB_t rxing_cb, SimAir_CB_t txing_cb, const char *id_str)
{
    SimAir_Descriptor_t *p_curr_sim_air_des;
    u32 newest;

    p_curr_sim_air_des = (SimAir_Descriptor_t *)malloc(sizeof(SimAir_Descriptor_t));
    p_curr_sim_air_des->p_time_slice_descriptor = (Time_Slice_Descriptor2 *)malloc(sizeof(Time_Slice_Descriptor2));
    p_curr_sim_air_des->sim_air_hdl = gSimAir_Handle_Ctr;
    p_curr_sim_air_des->sim_air_id_str = id_str;
    p_curr_sim_air_des->cb_type = SIM_AIR_WAKEUP_CB;
    p_curr_sim_air_des->wake_up_cb = wake_up_cb;
    p_curr_sim_air_des->rxing_cb = rxing_cb;
    p_curr_sim_air_des->txing_cb = txing_cb;
    p_curr_sim_air_des->p_time_slice_descriptor->remain_time = 0;
    p_curr_sim_air_des->p_time_slice_descriptor->state = TIME_SLICE_CB_DONE;
    p_curr_sim_air_des->p_time_slice_descriptor->times_up_cb = (Common_CB_t)SimAir_Dispatcher;
    p_curr_sim_air_des->p_time_slice_descriptor->hdl_to_cb = p_curr_sim_air_des;
    p_curr_sim_air_des->requ_resp_info = NULL;

    gSimAirDes_Vector.push_back(p_curr_sim_air_des);
    newest = gSimAirDes_Vector.size()-1;
    SimTimeSlice2_Init_AddDescriptor(gSimAirDes_Vector[newest]->p_time_slice_descriptor);

    gSimAir_Handle_Ctr++;
    return p_curr_sim_air_des->sim_air_hdl;
}

SimAir_Handle_t SimAir_Init_AddDescriptor2(SimAir_CB_Set_t *cb_set)
{
    SimAir_Descriptor_t *p_curr_sim_air_des;
    u32 newest;

    p_curr_sim_air_des = (SimAir_Descriptor_t *)malloc(sizeof(SimAir_Descriptor_t));
    p_curr_sim_air_des->p_time_slice_descriptor = (Time_Slice_Descriptor2 *)malloc(sizeof(Time_Slice_Descriptor2));
    p_curr_sim_air_des->sim_air_hdl = gSimAir_Handle_Ctr;
    p_curr_sim_air_des->sim_air_id_str = cb_set->id_str;
    p_curr_sim_air_des->cb_type = SIM_AIR_WAKEUP_CB;
    p_curr_sim_air_des->wake_up_cb = cb_set->wake_cb;
    p_curr_sim_air_des->rxing_cb = cb_set->rx_cb;
    p_curr_sim_air_des->txing_cb = cb_set->tx_cb;
    p_curr_sim_air_des->p_time_slice_descriptor->remain_time = 0;
    p_curr_sim_air_des->p_time_slice_descriptor->state = TIME_SLICE_CB_DONE;
    p_curr_sim_air_des->p_time_slice_descriptor->times_up_cb = (Common_CB_t)SimAir_Dispatcher;
    p_curr_sim_air_des->p_time_slice_descriptor->hdl_to_cb = p_curr_sim_air_des;
    p_curr_sim_air_des->requ_resp_info = NULL;

    gSimAirDes_Vector.push_back(p_curr_sim_air_des);
    newest = gSimAirDes_Vector.size()-1;
    SimTimeSlice2_Init_AddDescriptor(gSimAirDes_Vector[newest]->p_time_slice_descriptor);

    gSimAir_Handle_Ctr++;
    return p_curr_sim_air_des->sim_air_hdl;
}

static void SimAir_Init_AddDescriptor_InternalTRx(void)
{
    SimAir_Descriptor_t *p_curr_sim_air_des;

    p_curr_sim_air_des = (SimAir_Descriptor_t *)malloc(sizeof(SimAir_Descriptor_t));
    p_curr_sim_air_des->p_time_slice_descriptor = SimTimeSlice2_GetLastStandaloneDes();
    p_curr_sim_air_des->sim_air_hdl = gSimAir_Handle_Ctr;
    p_curr_sim_air_des->cb_type = SIM_AIR_INTERNAL_TRx;
    p_curr_sim_air_des->wake_up_cb = SimAir_InternalTRx;
    p_curr_sim_air_des->rxing_cb = NULL;
    p_curr_sim_air_des->txing_cb = NULL;
    p_curr_sim_air_des->p_time_slice_descriptor->remain_time = 0;
    p_curr_sim_air_des->p_time_slice_descriptor->state = TIME_SLICE_CB_DONE;
    p_curr_sim_air_des->p_time_slice_descriptor->times_up_cb = (Common_CB_t)SimAir_Dispatcher;
    p_curr_sim_air_des->p_time_slice_descriptor->hdl_to_cb = p_curr_sim_air_des;
    p_curr_sim_air_des->requ_resp_info = NULL;

    gSimAirDes_Vector.push_back(p_curr_sim_air_des);
    //Flake: don't have to register, because it's using "LastStandalone Descriptor".
    //newest = gSimAirDes_Vector.size()-1;
    //SimTimeSlice2_Init_AddDescriptor(gSimAirDes_Vector[newest]->p_time_slice_descriptor);

    gSimAir_Handle_InternalTRx = p_curr_sim_air_des->sim_air_hdl;
    gSimAir_Handle_Ctr++;
}

int SimAir_Start(int enableLibVCD /*= 0*/)
{
    if (enableLibVCD) {
        SimTimeSlice_RegisterTimeAddCallback(LibVCD_ClockAdd);
    }
    SimAir_Init_AddDescriptor_InternalTRx();
    SimTimeSlice2_Start();
    return 0;
}

int SimAir_Uninit(void)
{
    u32 size_of_gSimAirDes_Vector = gSimAirDes_Vector.size();

    for (u32 i=0; i<size_of_gSimAirDes_Vector; i++)
    {
        if (gSimAir_Handle_InternalTRx != i) {
            free(gSimAirDes_Vector[i]->p_time_slice_descriptor);
        }
        free(gSimAirDes_Vector[i]);
    }
    gSimAirDes_Vector.clear();

    gSimAir_Handle_Ctr = 0;
    gSimAir_Handle_InternalTRx = 0;

    SimAir_ChMgr_Uninit();

    SimTimeSlice2_Uninit();
    return 0;
}

int SimAir_Log_Enable(bool enable_every_clock_dump, bool only_dump_tx_rx_done)
{
#if SIM_AIR_LOG
    g_sim_air_log.FileOpen();
    g_sim_air_log_enable = true;
    g_sim_air_log_enable_every_clock_dump = enable_every_clock_dump;
    g_sim_air_log_only_dump_tx_rx_done = only_dump_tx_rx_done;
#endif //SIM_AIR_LOG
    return 0;
}

int SimAir_Log_Disable(void)
{
#if SIM_AIR_LOG
    g_sim_air_log.FileClose();
    g_sim_air_log_enable = false;
    g_sim_air_log_enable_every_clock_dump = false;
#endif //SIM_AIR_LOG
    return 0;
}

int SimAir_Request(SimAir_Info_t *info)
{
    BASIC_ASSERT(info != NULL);
    SimTimeSlice_TimeStampGet(&info->request_time_h, &info->request_time_l);
    //info->tx_rx_initial_dump_done = false;

    //BASIC_ASSERT(gSimAir_Handle_InternalTRx != info->hdl);//upper can't use internal trx function directly
    switch (info->requ_type) {
        case SIM_AIR_WAKEUP_REQUEST:
            gSimAirDes_Vector[info->hdl]->p_time_slice_descriptor->remain_time = info->next_wake_up_time;
            gSimAirDes_Vector[info->hdl]->p_time_slice_descriptor->state = TIME_SLICE_CB_WAITING;
            //Hook requ/resp info by handle
            gSimAirDes_Vector[info->hdl]->requ_resp_info = info;
            break;

        case SIM_AIR_RX_REQUEST:
        case SIM_AIR_TX_REQUEST:
            BASIC_ASSERT(info->clocks_total > 0);
            SimAir_ActivePeriodicInternalTRx();;
            SimAir_ChMgr_AddRequest(info);
            break;

        default:
            BASIC_ASSERT(0);
            break;
    }

#if SIM_AIR_LOG
    if (g_sim_air_log_enable) {
        u32 clk_h, clk_l;
        SimTimeSlice_TimeStampGet(&clk_h, &clk_l);

        const char *tx_rx_str;
        switch (info->requ_type) {
            case SIM_AIR_RX_REQUEST: tx_rx_str = "RX"; break;
            case SIM_AIR_TX_REQUEST: tx_rx_str = "TX"; break;
            default:
                break;
        }
        switch (info->requ_type) {
            case SIM_AIR_RX_REQUEST:
            case SIM_AIR_TX_REQUEST:
                fprintf(g_sim_air_log.fp, "[%8d][i_TR][------]%s ... %s start\n", 
                    clk_l,
                    SimAir_Handle_ID_String_Get(info->hdl),
                    tx_rx_str);
                break;
            default:
                break;
        }
    }
#endif //SIM_AIR_LOG

    return 0;
}

int SimAir_ExtendRxTime(SimAir_Info_t *info, u32 extend_time)
{
    switch (info->requ_type) {
        case SIM_AIR_RX_REQUEST:
            info->clocks_total += extend_time;
            break;

        default:
            BASIC_ASSERT(0);
            break;
    }
    return 0;
}

int SimAir_ExecuteResponseCallback(SimAir_Info_t *info)
{
    BASIC_ASSERT(info!=NULL);
    SimTimeSlice_TimeStampGet(&info->response.ref_clock_H, &info->response.ref_clock_L);

    switch (info->response.resp_type) {
        case SIM_AIR_RXING:
        case SIM_AIR_RX_DONE:
            if (gSimAirDes_Vector[info->hdl]->rxing_cb != NULL) {
                (*(gSimAirDes_Vector[info->hdl]->rxing_cb))(info);
            }
            break;

        case SIM_AIR_TXING:
        case SIM_AIR_TX_DONE:
            if (gSimAirDes_Vector[info->hdl]->txing_cb != NULL) {
                (*(gSimAirDes_Vector[info->hdl]->txing_cb))(info);
            }
            break;

        default:
            BASIC_ASSERT(0);
            break;
    }

    return 0;
}

#define ___DEMO___________________________
#define ___DEMO__________________________
#define ___DEMO_________________________

static SimAir_Handle_t g_slave_hdl, g_master_hdl;
static SimAir_Info_t g_slave_info, g_master_info;
static int g_slave_stage = 0;
static u8  g_slave_rx_buf[300] = {0x77, 0x88}; //garbage value
//static u8  g_slave_tx_buf[300];
static int g_master_stage = 0;
//static u8  g_master_rx_buf[300];
static u8  g_master_tx_buf[300] = {0x05, 0x9A, /*Length*/0x07, 0x34,  0x56, 0x78, 0x9A, 0xBC,  0xDE, 0xF1};

/* TEST 1 - Pure Rx
Time:   10      100     900
S       wake    w+r800  rDone
M
*/
static int Test_1_S_Wake(SimAir_Info_t *info)
{
    printf("%s() -- %d, stage:%d\n", __func__, info->response.ref_clock_L, g_slave_stage);

    switch (g_slave_stage) {
        case 0: //wake up in 100
            g_slave_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
            g_slave_info.next_wake_up_time = 100 - info->response.ref_clock_L;
            SimAir_Request(&g_slave_info);
            g_slave_stage++;
            break;

        case 1: //r800
            g_slave_info.requ_type = SIM_AIR_RX_REQUEST;
            g_slave_info.clocks_total = 800;
            g_slave_info.freq = 5;
            SimAir_Request(&g_slave_info);
            g_slave_stage++;
            break;
    }
    return 0;
}

static int Test_1_S_Rx(SimAir_Info_t *info)
{
    printf("%s() -- %d, resp_type:%d, rx_done_bits:%d\n", __func__, info->response.ref_clock_L, info->response.resp_type, info->response.rx_done_bits);

    if (info->response.resp_type == SIM_AIR_RX_DONE) {
        ARRAYDUMPX3(g_slave_rx_buf, info->response.rx_done_bits/8);
        Test_1_S_Wake(info);
    }
    return 0;
}

static int Test_1(void)
{
    printf("\n[[[ This is %s() ]]]\n", __func__);

    g_slave_stage = 0;
    g_slave_hdl = SimAir_Init_AddDescriptor(Test_1_S_Wake, Test_1_S_Rx, NULL, NULL);
    //init first wake up & set hdl into info
    g_slave_info.hdl = g_slave_hdl;
    g_slave_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_slave_info.next_wake_up_time = 10;
    //init basic air info, must complete
    g_slave_info.clocks_per_bit = 10;
    g_slave_info.clocks_to_interrupt_trx = 80;
    g_slave_info.clocks_total = 1;
    g_slave_info.freq = 0;
    g_slave_info.rx_buf = g_slave_rx_buf;
    g_slave_info.rx_buf_max_size_in_bits = 300 * 8;
    SimAir_Request(&g_slave_info);

    //SimAir_Dump();
    //SimTimeSlice2_Dump();
    SimAir_Start();
    //SimTimeSlice2_Dump();
    //SimAir_Dump();
    SimAir_Uninit();

    return 0;
}

/* TEST 2 - Pure Tx
Time:   20      100     340
S
M       wake    w+t240
*/
static int Test_2_M_Wake(SimAir_Info_t *info)
{
    printf("\t\t%s() -- %d, stage:%d\n", __func__, info->response.ref_clock_L, g_master_stage);
    switch (g_master_stage) {
        case 0: //wake up in 100
            g_master_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
            g_master_info.next_wake_up_time = 100 - info->response.ref_clock_L;
            SimAir_Request(&g_master_info);
            g_master_stage++;
            break;

        case 1: //t240
            g_master_info.requ_type = SIM_AIR_TX_REQUEST;
            g_master_info.clocks_total = 240;
            g_master_info.freq = 5;
            SimAir_Request(&g_master_info);
            g_master_stage++;
            break;
    }
    return 0;
}

static int Test_2_M_Tx(SimAir_Info_t *info)
{
    printf("\t\t%s() -- %d, resp_type:%d, tx_done_bits:%d\n", __func__, info->response.ref_clock_L, info->response.resp_type, info->response.tx_done_bits);

    if (info->response.resp_type == SIM_AIR_TX_DONE) {
        Test_2_M_Wake(info);
    }

    return 0;
}

static int Test_2(void)
{
    printf("\n[[[ This is %s() ]]]\n", __func__);

    g_master_stage = 0;
    g_master_hdl = SimAir_Init_AddDescriptor(Test_2_M_Wake, NULL, Test_2_M_Tx, NULL);
    //init first wake up & set hdl into info
    g_master_info.hdl = g_master_hdl;
    g_master_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info.next_wake_up_time = 20;
    //init basic air info, must complete
    g_master_info.clocks_per_bit = 10;
    g_master_info.clocks_to_interrupt_trx = 80;
    g_master_info.clocks_total = 1;
    g_master_info.freq = 0;
    g_master_info.tx_buf = g_master_tx_buf;
    g_master_info.tx_buf_len_in_bits = 300 * 8;
    SimAir_Request(&g_master_info);

    //SimAir_Dump();
    //SimTimeSlice2_Dump();
    SimAir_Start();
    //SimTimeSlice2_Dump();
    //SimAir_Dump();
    SimAir_Uninit();

    return 0;
}

/* TEST 3 - Rx + Tx, Test1+Test2
Time:   10      20      100     900
S       wake            w+r800  rDone
M               wake    w+t240
*/
static int Test_3(void)
{
    printf("\n[[[ This is %s() ]]]\n", __func__);

    //SimAir_Log_Enable();

    g_slave_stage = 0;
    g_slave_hdl = SimAir_Init_AddDescriptor(Test_1_S_Wake, Test_1_S_Rx, NULL, NULL);
    //init first wake up & set hdl into info
    g_slave_info.hdl = g_slave_hdl;
    g_slave_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_slave_info.next_wake_up_time = 10;
    //init basic air info, must complete
    g_slave_info.clocks_per_bit = 10;
    g_slave_info.clocks_to_interrupt_trx = 80;
    g_slave_info.clocks_total = 1;
    g_slave_info.freq = 0;
    g_slave_info.rx_buf = g_slave_rx_buf;
    g_slave_info.rx_buf_max_size_in_bits = 300 * 8;
    SimAir_Request(&g_slave_info);

    g_master_stage = 0;
    g_master_hdl = SimAir_Init_AddDescriptor(Test_2_M_Wake, NULL, Test_2_M_Tx, NULL);
    //init first wake up & set hdl into info
    g_master_info.hdl = g_master_hdl;
    g_master_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info.next_wake_up_time = 20;
    //init basic air info, must complete
    g_master_info.clocks_per_bit = 10;
    g_master_info.clocks_to_interrupt_trx = 80;
    g_master_info.clocks_total = 1;
    g_master_info.freq = 0;
    g_master_info.tx_buf = g_master_tx_buf;
    g_master_info.tx_buf_len_in_bits = 300 * 8;
    SimAir_Request(&g_master_info);

    //SimAir_Dump();
    //SimTimeSlice2_Dump();
    SimAir_Start();
    //SimTimeSlice2_Dump();
    //SimAir_Dump();
    SimAir_Uninit();

    SimAir_Log_Disable();

    return 0;
}

/* TEST 4 - Extend Rx
Time:   10      20      100     340         900
S       wake            w+r320  rExtend480  rDone
M               wake    w+t800              tDone
*/
static int Test_4_S_Wake(SimAir_Info_t *info)
{
    printf("%s() -- %d, stage:%d\n", __func__, info->response.ref_clock_L, g_slave_stage);

    switch (g_slave_stage) {
        case 0: //wake up in 100
            g_slave_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
            g_slave_info.next_wake_up_time = 100 - info->response.ref_clock_L;
            SimAir_Request(&g_slave_info);
            g_slave_stage++;
            break;

        case 1: //r320
            g_slave_info.requ_type = SIM_AIR_RX_REQUEST;
            g_slave_info.clocks_total = 320;
            g_slave_info.freq = 5;
            SimAir_Request(&g_slave_info);
            g_slave_stage++;
            break;
    }
    return 0;
}

static int Test_4_S_Rx(SimAir_Info_t *info)
{
    printf("%s() -- %d, resp_type:%d, rx_done_bits:%d\n", __func__, info->response.ref_clock_L, info->response.resp_type, info->response.rx_done_bits);

    if (info->response.resp_type == SIM_AIR_RX_DONE) {
        ARRAYDUMPX3(g_slave_rx_buf, info->response.rx_done_bits/8);
        Test_4_S_Wake(info);
    }

    if (g_slave_stage == 2 && info->response.resp_type == SIM_AIR_RXING) {
        if (info->response.rx_done_bits == 24) {
            u32 len_in_packet = g_slave_rx_buf[2];
            u32 extend_time = (len_in_packet * 80) - /*left time*/80;
            SimAir_ExtendRxTime(info, extend_time);
        }
    }

    return 0;
}

static int Test_4_M_Wake(SimAir_Info_t *info)
{
    printf("\t\t%s() -- %d, stage:%d\n", __func__, info->response.ref_clock_L, g_master_stage);
    switch (g_master_stage) {
        case 0: //wake up in 100
            g_master_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
            g_master_info.next_wake_up_time = 100 - info->response.ref_clock_L;
            SimAir_Request(&g_master_info);
            g_master_stage++;
            break;

        case 1: //t800
            g_master_info.requ_type = SIM_AIR_TX_REQUEST;
            g_master_info.clocks_total = 800;
            g_master_info.freq = 5;
            SimAir_Request(&g_master_info);
            g_master_stage++;
            break;
    }
    return 0;
}

static int Test_4_M_Tx(SimAir_Info_t *info)
{
    printf("\t\t%s() -- %d, resp_type:%d, tx_done_bits:%d\n", __func__, info->response.ref_clock_L, info->response.resp_type, info->response.tx_done_bits);

    if (info->response.resp_type == SIM_AIR_TX_DONE) {
        Test_4_M_Wake(info);
    }

    return 0;
}

static int Test_4_Demo_ExtendRx(void)
{
    printf("\n[[[ This is %s() ]]]\n", __func__);

    SimAir_Log_Enable();

    g_slave_stage = 0;
    g_slave_hdl = SimAir_Init_AddDescriptor(Test_4_S_Wake, Test_4_S_Rx, NULL, "Peer_R");
    //init first wake up & set hdl into info
    g_slave_info.hdl = g_slave_hdl;
    g_slave_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_slave_info.next_wake_up_time = 10;
    //init basic air info, must complete
    g_slave_info.clocks_per_bit = 10;
    g_slave_info.clocks_to_interrupt_trx = 80;
    g_slave_info.clocks_total = 1;
    g_slave_info.freq = 0;
    g_slave_info.rx_buf = g_slave_rx_buf;
    g_slave_info.rx_buf_max_size_in_bits = 300 * 8;
    SimAir_Request(&g_slave_info);

    g_master_stage = 0;
    g_master_hdl = SimAir_Init_AddDescriptor(Test_4_M_Wake, NULL, Test_4_M_Tx, "Peer_T");
    //init first wake up & set hdl into info
    g_master_info.hdl = g_master_hdl;
    g_master_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_master_info.next_wake_up_time = 20;
    //init basic air info, must complete
    g_master_info.clocks_per_bit = 10;
    g_master_info.clocks_to_interrupt_trx = 80;
    g_master_info.clocks_total = 1;
    g_master_info.freq = 0;
    g_master_info.tx_buf = g_master_tx_buf;
    g_master_info.tx_buf_len_in_bits = 300 * 8;
    SimAir_Request(&g_master_info);

    //SimAir_Dump();
    //SimTimeSlice2_Dump();
    SimAir_Start();

//Demo restart mechanism
#if 0
    PRINT_LINE;
    SimAir_Start();
    PRINT_LINE;
    g_slave_info.requ_type = SIM_AIR_WAKEUP_REQUEST;
    g_slave_info.next_wake_up_time = 10;
    SimAir_Request(&g_slave_info);
    SimAir_Start();
#endif
    //SimTimeSlice2_Dump();
    //SimAir_Dump();
    SimAir_Uninit();

    SimAir_Log_Disable();

    return 0;
}

int SimAir_Demo(void)
{
    Test_1();
    Test_2();
    Test_3();
    Test_4_Demo_ExtendRx();
    return 0;
}

void SimAir_Dump(void)
{
    printf("\n=== %s() ===\n", __func__);

    u32 size_of_gSimAirDes_Vector = gSimAirDes_Vector.size();
    DUMPU(size_of_gSimAirDes_Vector);

    for (u32 i=0; i<size_of_gSimAirDes_Vector; i++)
    {
        printf("sim_air_hdl:%d, cb_type:%d, remain_time:%6d, state:%d, ( 0x%08X / 0x%08X )\n",
            gSimAirDes_Vector[i]->sim_air_hdl,
            gSimAirDes_Vector[i]->cb_type,
            gSimAirDes_Vector[i]->p_time_slice_descriptor->remain_time,
            gSimAirDes_Vector[i]->p_time_slice_descriptor->state,
            POINTER_TO_U32(gSimAirDes_Vector[i]->p_time_slice_descriptor->times_up_cb),
            POINTER_TO_U32(gSimAirDes_Vector[i]->p_time_slice_descriptor->hdl_to_cb));
    }

    for (u32 i=0; i<size_of_gSimAirDes_Vector; i++)
    {
        printf("sim air des:0x%08X, time slice des:0x%08X\n",
            POINTER_TO_U32(gSimAirDes_Vector[i]),
            POINTER_TO_U32(gSimAirDes_Vector[i]->p_time_slice_descriptor));
    }

    //DUMPA(Test_Slave_Wake_Up);
    //DUMPA(Test_Master_Txing);
}
