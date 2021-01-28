
#ifndef _SIM_CH_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

typedef struct {
    u32 power;
    u32 stamp;
    u32 data32;
    u32 dataRetainCycles;
    void *self;
} SimChSimple32_t;

class SimChSimple32 {
public:
    SimChSimple32_t ct; //content

    SimChSimple32(void){Init();};
    ~SimChSimple32(void){};
    void Init(void){memset(&ct,0,sizeof(SimChSimple32_t));ct.self=(void *)this;};
    void Probe(u32 timeStamp);
    void Tx(u32 data, u32 timeStamp);
};

// Loose define structure & class
typedef struct {
    u32 repeatType;
    u32 repeatCtr;
    timeint_t activeCycles;
    u32 act; //tx or rx or ...
    u32 result;
    void *simDb; // no ops, us SimEngine v1.0 (SimTime3 ...)
    void *timeDesc;
    u32 chType;
    void *chHdl;
    void *hdlA;
    void *hdlB;
    void *hdlC;
    Common_CB_t cb; // handle is this desc, tx or rx cb every activeCycles

    Common_CB_t upperCb;
    u32 upperBufLen;
    u8 *upperBuf;
} SimCh_Desc;
class SimCh_Helpler {
    SimCh_Desc desc;

    SimCh_Helpler(void){Init(&desc);};
    ~SimCh_Helpler(void){};
    void Init(SimCh_Desc *pDesc);
    void Bind(void *simDb, void *timeDesc);
    //void GoInTime(timeint_t startTime);
};

#define REPEAT_INFINITE BIT_0
#define ACT_RX BIT_0
#define ACT_TX BIT_1
#define RESULT_RX_1_UNIT    BIT_0
#define RESULT_TX_1_UNIT    BIT_1
#define RESULT_TX_DONE      BIT_2
u32 SimCh_Helpler_Time3RepeatCB(SimTime3_Desc *desc);

#define _SIM_CH_HPP_INCLUDED_
#endif//_SIM_CH_HPP_INCLUDED_