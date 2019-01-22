
#include "Everything_SimAir.hpp"
#include "SimAir.hpp"
#include <vector>
#include <map>

//std::vector<std::string> standAloneArgs;

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

            p_curr_info->response.clocks_elapsed++;
            if ((p_curr_info->response.clocks_elapsed % p_curr_info->clocks_per_bit) == 0) {
                p_curr_info->response.tx_done_bits++;
            }

            if (p_curr_info->response.clocks_elapsed == p_curr_info->clocks_total) {
                //delete this request in next stage, and call tx done response
            } else {
                if ((p_curr_info->response.clocks_elapsed % p_curr_info->clocks_to_interrupt_trx) == 0) {
                    p_curr_info->response.resp_type = SIM_AIR_TXING;
                    SimAir_ExecuteResponseCallback(p_curr_info);
                }
            }
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
            }
        }

        //go through rx than & call back
        for (u32 i=0; i<ch_peers->p_rx_vector->size(); i++)
        {
            SimAir_Info_t *p_curr_info = (*ch_peers->p_rx_vector)[i];
            //do rx
            p_curr_info->response.accu_power_for_rx += final_power_in_air;
            p_curr_info->response.clocks_elapsed++;
            //do sampling to one bit
            if ((p_curr_info->response.clocks_elapsed % p_curr_info->clocks_per_bit) == 0) {
                u32 rx_done_bits = p_curr_info->response.rx_done_bits;
                u32 rx_done_bits_to_bit_index = rx_done_bits % 8;
                u32 rx_done_bits_to_byte_index = rx_done_bits / 8;

                if (rx_done_bits_to_bit_index == 0) {
                    //clear whole byte for next 8 input bits
                    p_curr_info->rx_buf[rx_done_bits_to_byte_index] = 0x00;
                }

                u8 rx_bit;
                if (p_curr_info->response.accu_power_for_rx > 50 * p_curr_info->clocks_per_bit) {
                    rx_bit = 1 << rx_done_bits_to_bit_index;
                } else {
                    rx_bit = 0;
                }
                p_curr_info->rx_buf[rx_done_bits_to_byte_index] |= rx_bit;
                p_curr_info->response.accu_power_for_rx = 0;
                p_curr_info->response.rx_done_bits++;
            }

            if (p_curr_info->response.clocks_elapsed == p_curr_info->clocks_total) {
                //delete this request in next stage, and call tx done response
            } else {
                if ((p_curr_info->response.clocks_elapsed % p_curr_info->clocks_to_interrupt_trx) == 0) {
                    p_curr_info->response.resp_type = SIM_AIR_RXING;
                    SimAir_ExecuteResponseCallback(p_curr_info);
                }
            }
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
    return info->freq + 3;
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
