
#include "Everything_SimAir.hpp"
#include "SimAir.hpp"
#include <vector>
#include <map>

#if SIM_AIR_LOG
extern LibFileIoClass g_sim_air_log;
extern bool g_sim_air_log_enable;
extern bool g_sim_air_log_enable_every_clock_dump;
extern bool g_sim_air_log_only_dump_tx_rx_done;
u32 g_sim_air_log_curr_ch;
extern const char *SimAir_Handle_ID_String_Get(u32 index);
const char *g_sim_air_log_tr_condition;
LibStringClass g_sim_air_log_bit_by_bit_string;
bool g_sim_air_log___is_in_every_clock;
#define TR_CONDITION__TX_IN_1_CLOCK   g_sim_air_log_tr_condition="Tx 1 clock"; g_sim_air_log___is_in_every_clock=true;
#define TR_CONDITION__TX_1_BIT_DONE   g_sim_air_log_tr_condition="Tx 1 bit done"; g_sim_air_log___is_in_every_clock=false;
#define TR_CONDITION__TX_INTERRUPT    g_sim_air_log_tr_condition="Tx interrupt !!!"; g_sim_air_log___is_in_every_clock=false;
#define TR_CONDITION__TX_ALL_DONE     g_sim_air_log_tr_condition="Tx all done !!!!!"; g_sim_air_log___is_in_every_clock=false;
#define TX_IN_CLOCK_LOG_PRINT   \
do { \
    u32 clk_h, clk_l;\
    SimTimeSlice_TimeStampGet(&clk_h, &clk_l);\
    if (false == g_sim_air_log_enable || g_sim_air_log_only_dump_tx_rx_done)\
        break;\
    if (g_sim_air_log___is_in_every_clock) {\
        if (false == g_sim_air_log_enable_every_clock_dump)\
            break;\
    }\
    fprintf(g_sim_air_log.fp,\
            "[%8d][i_TR][ch:%3d]%s - tx_bit:%d, final_power_in_air:%4d - %s (byte:%d/bit:%d/0x%02X->%8s)\n",\
            clk_l,\
            g_sim_air_log_curr_ch,\
            SimAir_Handle_ID_String_Get(p_curr_info->hdl),\
            bit_for_tx,\
            final_power_in_air,\
            g_sim_air_log_tr_condition,\
            tx_done_bits_to_byte_index,\
            tx_done_bits_to_bit_index,\
            p_curr_info->tx_buf[tx_done_bits_to_byte_index],\
            g_sim_air_log_bit_by_bit_string.BitsStringOfOneByte_LittleEndian(p_curr_info->tx_buf[tx_done_bits_to_byte_index],1+tx_done_bits_to_bit_index));\
} while(0);
#define TX_ALL_DONE_LOG_PRINT   \
do {\
    u32 clk_h, clk_l;\
    SimTimeSlice_TimeStampGet(&clk_h, &clk_l);\
    if (false == g_sim_air_log_enable)\
        break;\
    fprintf(g_sim_air_log.fp,\
            "[%8d][i_TR][ch:%3d]%s - tx_bit:X, final_power_in_air:%4d - %s\n",\
            clk_l,\
            g_sim_air_log_curr_ch,\
            SimAir_Handle_ID_String_Get(p_curr_info->hdl),\
            final_power_in_air,\
            g_sim_air_log_tr_condition);\
    _SimAir_Log_Buf_Print(p_curr_info->tx_buf, p_curr_info->response.tx_done_bits);\
} while(0);
/* ========== RX ========== */
/* ========== RX ========== */
/* ========== RX ========== */
const char *g_sim_air_log_rx_bit;
#define RX_BIT_IS_NULL g_sim_air_log_rx_bit="-"
#define RX_BIT_IS_0    g_sim_air_log_rx_bit="0"
#define RX_BIT_IS_1    g_sim_air_log_rx_bit="1"
u32 g_sim_air_log_accu_power_for_rx;
#define STORE_ACCU_POWER_FOR_RX(a) g_sim_air_log_accu_power_for_rx=a
#define TR_CONDITION__RX_IN_1_CLOCK   g_sim_air_log_tr_condition="Rx 1 clock"; g_sim_air_log___is_in_every_clock=true;
#define TR_CONDITION__RX_1_BIT_DONE   g_sim_air_log_tr_condition="Rx 1 bit done"; g_sim_air_log___is_in_every_clock=false;
#define TR_CONDITION__RX_INTERRUPT    g_sim_air_log_tr_condition="Rx interrupt !!!"; g_sim_air_log___is_in_every_clock=false;
#define TR_CONDITION__RX_ALL_DONE     g_sim_air_log_tr_condition="Rx all done !!!!!"; g_sim_air_log___is_in_every_clock=false;
#define RX_IN_CLOCK_LOG_PRINT   \
do { \
    u32 clk_h, clk_l;\
    SimTimeSlice_TimeStampGet(&clk_h, &clk_l);\
    if (false == g_sim_air_log_enable || g_sim_air_log_only_dump_tx_rx_done)\
        break;\
    if (g_sim_air_log___is_in_every_clock) {\
        if (false == g_sim_air_log_enable_every_clock_dump)\
            break;\
    }\
    fprintf(g_sim_air_log.fp,\
            "[%8d][i_TR][ch:%3d]%s - rx_bit:%s, accu_power_for_rx :%4d - %s\n",\
            clk_l,\
            g_sim_air_log_curr_ch,\
            SimAir_Handle_ID_String_Get(p_curr_info->hdl),\
            g_sim_air_log_rx_bit,\
            g_sim_air_log_accu_power_for_rx,\
            g_sim_air_log_tr_condition);\
} while(0);
#define RX_ALL_DONE_LOG_PRINT   \
do {\
    u32 clk_h, clk_l;\
    SimTimeSlice_TimeStampGet(&clk_h, &clk_l);\
    if (false == g_sim_air_log_enable)\
        break;\
    fprintf(g_sim_air_log.fp,\
            "[%8d][i_TR][ch:%3d]%s - rx_bit:%s, accu_power_for_rx :%4d - %s\n",\
            clk_l,\
            g_sim_air_log_curr_ch,\
            SimAir_Handle_ID_String_Get(p_curr_info->hdl),\
            g_sim_air_log_rx_bit,\
            g_sim_air_log_accu_power_for_rx,\
            g_sim_air_log_tr_condition);\
    _SimAir_Log_Buf_Print(p_curr_info->rx_buf, p_curr_info->response.rx_done_bits);\
} while(0);

void _SimAir_Log_Buf_Print(u8 *buf, u32 bits_len)
{
    u32 i = 0;
    fprintf(g_sim_air_log.fp, "bits_len = %d (in byte:%d ... %d)\n", bits_len, bits_len/8, bits_len%8);

    //print bytes
    i = 0;
    if (bits_len >= 8) {
        for (i=0; i<bits_len; i+=8) {
            if ((i/8)%4==3)
                fprintf(g_sim_air_log.fp, "0x%02X, ", buf[i/8]);
            else
                fprintf(g_sim_air_log.fp, "0x%02X ", buf[i/8]);
        }
    }
    if (i!=bits_len) {
        fprintf(g_sim_air_log.fp, " -- %s", g_sim_air_log_bit_by_bit_string.BitsStringOfOneByte_LittleEndian(buf[bits_len/8], /*1+*/(bits_len%8)));
    }
    fprintf(g_sim_air_log.fp, "\n");
    
    i = 0;
    if (bits_len >= 8) {
        for (i=0; i<bits_len; i+=8) {
            if ((i/8)%4==3)
                fprintf(g_sim_air_log.fp, "%s,  ", g_sim_air_log_bit_by_bit_string.BitsStringOfOneByte_LittleEndian(buf[i/8], 8));
            else
                fprintf(g_sim_air_log.fp, "%s ", g_sim_air_log_bit_by_bit_string.BitsStringOfOneByte_LittleEndian(buf[i/8], 8));
        }
    }
    if (i!=bits_len) {
        fprintf(g_sim_air_log.fp, " -- %s", g_sim_air_log_bit_by_bit_string.BitsStringOfOneByte_LittleEndian(buf[bits_len/8], 1+(bits_len%8)));
    }
    fprintf(g_sim_air_log.fp, "\n");

}
#else
#define TR_CONDITION__TX_IN_1_CLOCK
#define TR_CONDITION__TX_1_BIT_DONE
#define TR_CONDITION__TX_INTERRUPT
#define TR_CONDITION__TX_ALL_DONE
#define TX_IN_CLOCK_LOG_PRINT
#define TX_ALL_DONE_LOG_PRINT
#define RX_BIT_IS_NULL
#define RX_BIT_IS_0
#define RX_BIT_IS_1
#define STORE_ACCU_POWER_FOR_RX(a)
#define TR_CONDITION__RX_IN_1_CLOCK
#define TR_CONDITION__RX_1_BIT_DONE
#define TR_CONDITION__RX_INTERRUPT
#define TR_CONDITION__RX_ALL_DONE
#define RX_IN_CLOCK_LOG_PRINT
#define RX_ALL_DONE_LOG_PRINT
#define _SimAir_Log_Buf_Print(a,b)
#endif //SIM_AIR_LOG

typedef struct {
    u32 vector_ctr;
    std::vector<SimAir_Info_t *> *p_rx_vector;
    std::vector<SimAir_Info_t *> *p_tx_vector;
}SimAir_Channel_Peers_t;

std::map<u32, SimAir_Channel_Peers_t *> gGlobalChannelMap;

int SimAir_ChMgr_ProcessOneTick(void)
{
    u32 is_any_map_empty = 0;
    u32 final_power_in_air = 0;
    SimAir_Channel_Peers_t *ch_peers;

    std::map<u32, SimAir_Channel_Peers_t *>::iterator it;
    for (it=gGlobalChannelMap.begin(); it!=gGlobalChannelMap.end(); ++it) {
        //printf("%2u, %s\n", it->first, it->second.c_str());
        ch_peers = it->second;
        BASIC_ASSERT(ch_peers->vector_ctr != 0);
        BASIC_ASSERT(ch_peers->vector_ctr == ch_peers->p_rx_vector->size() + ch_peers->p_tx_vector->size());
#if SIM_AIR_LOG
        g_sim_air_log_curr_ch = it->first;
#endif //SIM_AIR_LOG

        //go through tx first & get single_bit_in_air & call back
        for (u32 i=0; i<ch_peers->p_tx_vector->size(); i++)
        {
            SimAir_Info_t *p_curr_info = (*ch_peers->p_tx_vector)[i];
            u32 tx_done_bits = p_curr_info->response.tx_done_bits;
            u32 tx_done_bits_to_bit_index = tx_done_bits % 8;
            u32 tx_done_bits_to_byte_index = tx_done_bits / 8;

            u32 bit_for_tx;
            bit_for_tx = p_curr_info->tx_buf[tx_done_bits_to_byte_index];
            bit_for_tx = bit_for_tx >> tx_done_bits_to_bit_index;
            bit_for_tx = bit_for_tx & 0x00000001;
            if (bit_for_tx) {
                final_power_in_air+=100;
            }
            TR_CONDITION__TX_IN_1_CLOCK;

            p_curr_info->response.clocks_elapsed++;
            if ((p_curr_info->response.clocks_elapsed % p_curr_info->clocks_per_bit) == 0) {
                TR_CONDITION__TX_1_BIT_DONE;
                p_curr_info->response.tx_done_bits++;
            }

            if (p_curr_info->response.clocks_elapsed == p_curr_info->clocks_total) {
                //delete this request in next stage, and call tx done response
            } else {
                if ((p_curr_info->response.clocks_elapsed % p_curr_info->clocks_to_interrupt_trx) == 0) {
                    TR_CONDITION__TX_INTERRUPT;
                    p_curr_info->response.resp_type = SIM_AIR_TXING;
                    SimAir_ExecuteResponseCallback(p_curr_info);
                }
            }
            TX_IN_CLOCK_LOG_PRINT;
        }
        //search completed request & delete & tx done response call back
        for (u32 i=ch_peers->p_tx_vector->size(); i>0; i--)
        {
            SimAir_Info_t *p_curr_info = (*ch_peers->p_tx_vector)[i-1];
            if (p_curr_info->response.clocks_elapsed == p_curr_info->clocks_total) {
                //delete this request in this stage, and call tx done response
                p_curr_info->response.resp_type = SIM_AIR_TX_DONE;
                SimAir_ExecuteResponseCallback(p_curr_info);
                // erase
                ch_peers->p_tx_vector->erase(ch_peers->p_tx_vector->begin()+i-1);
                ch_peers->vector_ctr--;

                TR_CONDITION__TX_ALL_DONE;
                TX_ALL_DONE_LOG_PRINT;
            }
        }

        //go through rx than & call back
        for (u32 i=0; i<ch_peers->p_rx_vector->size(); i++)
        {
            u8 rx_bit;
            RX_BIT_IS_NULL;
            SimAir_Info_t *p_curr_info = (*ch_peers->p_rx_vector)[i];
            //do rx
            p_curr_info->response.accu_power_for_rx += final_power_in_air;
            p_curr_info->response.clocks_elapsed++;
            STORE_ACCU_POWER_FOR_RX(p_curr_info->response.accu_power_for_rx);
            TR_CONDITION__RX_IN_1_CLOCK;
            //do sampling to one bit
            if ((p_curr_info->response.clocks_elapsed % p_curr_info->clocks_per_bit) == 0) {
                u32 rx_done_bits = p_curr_info->response.rx_done_bits;
                u32 rx_done_bits_to_bit_index = rx_done_bits % 8;
                u32 rx_done_bits_to_byte_index = rx_done_bits / 8;

                if (rx_done_bits_to_bit_index == 0) {
                    //clear whole byte for next 8 input bits
                    p_curr_info->rx_buf[rx_done_bits_to_byte_index] = 0x00;
                }

                if (p_curr_info->response.accu_power_for_rx > 50 * p_curr_info->clocks_per_bit) {
                    rx_bit = 1 << rx_done_bits_to_bit_index;
                    RX_BIT_IS_1;
                } else {
                    rx_bit = 0;
                    RX_BIT_IS_0;
                }
                p_curr_info->rx_buf[rx_done_bits_to_byte_index] |= rx_bit;
                p_curr_info->response.accu_power_for_rx = 0;
                p_curr_info->response.rx_done_bits++;
                TR_CONDITION__RX_1_BIT_DONE;
            }

            if (p_curr_info->response.clocks_elapsed == p_curr_info->clocks_total) {
                //delete this request in next stage, and call tx done response
            } else {
                if ((p_curr_info->response.clocks_elapsed % p_curr_info->clocks_to_interrupt_trx) == 0) {
                    TR_CONDITION__RX_INTERRUPT;
                    p_curr_info->response.resp_type = SIM_AIR_RXING;
                    SimAir_ExecuteResponseCallback(p_curr_info);
                }
            }
            RX_IN_CLOCK_LOG_PRINT;
        }
        //search completed request & delete & tx done response call back
        for (u32 i=ch_peers->p_rx_vector->size(); i>0; i--)
        {
            SimAir_Info_t *p_curr_info = (*ch_peers->p_rx_vector)[i-1];
            if (p_curr_info->response.clocks_elapsed == p_curr_info->clocks_total) {
                //delete this request in this stage, and call tx done response
                p_curr_info->response.resp_type = SIM_AIR_RX_DONE;
                SimAir_ExecuteResponseCallback(p_curr_info);
                // erase
                ch_peers->p_rx_vector->erase(ch_peers->p_rx_vector->begin()+i-1);
                ch_peers->vector_ctr--;

                TR_CONDITION__RX_ALL_DONE;
                RX_ALL_DONE_LOG_PRINT;
            }
        }

        //empty map check
        if (ch_peers->vector_ctr == 0) {
            is_any_map_empty = 1;
        }
    }

    //check & delete map if there are no more jobs in a channel
    if (is_any_map_empty) {
        while (1) {
            u32 map_ctr = 0;
            u32 map_size = gGlobalChannelMap.size();
            for (it=gGlobalChannelMap.begin(); it!=gGlobalChannelMap.end(); ++it) {
                ch_peers = it->second;
                if (ch_peers->vector_ctr == 0) {
                    gGlobalChannelMap.erase(it);
                    break;
                }
                map_ctr++;
            }
            if (map_ctr == map_size)
                break;
        }
    }

    return 0;
}

int SimAir_ChMgr_CheckDoesNeedToTick(void)
{
    return gGlobalChannelMap.size() ? 1 : 0;
}

static u32 SimAir_ChMgr_CalculateChannel(SimAir_Info_t *info)
{
    return info->freq;// + 3;
}

int SimAir_ChMgr_AddRequest(SimAir_Info_t *info)
{
    u32 ch = SimAir_ChMgr_CalculateChannel(info);
    SimAir_Channel_Peers_t *ch_peers;

    //Find ch & it's vectors
    std::map<u32,SimAir_Channel_Peers_t *>::iterator it = gGlobalChannelMap.find(ch);
    if (it == gGlobalChannelMap.end()) {
        ch_peers = (SimAir_Channel_Peers_t *)malloc(sizeof(SimAir_Channel_Peers_t));
        ch_peers->vector_ctr = 0;
        ch_peers->p_rx_vector = new (std::vector<SimAir_Info_t *>);
        ch_peers->p_tx_vector = new (std::vector<SimAir_Info_t *>);
        gGlobalChannelMap.insert(std::pair<u32, SimAir_Channel_Peers_t *>(ch, ch_peers));
    } else {
        ch_peers = it->second;
    }

    //Init tx/rx data
    info->response.resp_type = SIM_AIR_INVALID_RESPONSE;
    info->response.clocks_elapsed = 0;
    info->response.tx_done_bits = 0;
    info->response.rx_done_bits = 0;
    info->response.accu_power_for_rx = 0;

    //Add to vectors
    ch_peers->vector_ctr++;
    switch (info->requ_type) {
        case SIM_AIR_RX_REQUEST:
            BASIC_ASSERT(info->rx_buf_max_size_in_bits >= info->clocks_total);
            ch_peers->p_rx_vector->push_back(info);
            break;

        case SIM_AIR_TX_REQUEST:
            BASIC_ASSERT(info->tx_buf_len_in_bits >= info->clocks_total);
            ch_peers->p_tx_vector->push_back(info);
            break;

        default:
            BASIC_ASSERT(0);
            break;
    }

    return 0;
}

void SimAir_ChMgr_Uninit(void)
{
    gGlobalChannelMap.clear();
}
