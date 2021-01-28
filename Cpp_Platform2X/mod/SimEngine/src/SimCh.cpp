
#include "Everything_SimEngine.hpp"

void SimChSimple32::Probe(u32 timeStamp)
{
    u32 diff = timeStamp - ct.stamp;
    if (diff < 0x80000000) {
        if (diff >= ct.dataRetainCycles) {
            ct.power = 0;
            ct.data32 = 0;
        }
    } else {
        ct.power = 0;
        ct.data32 = 0;
    }
}

void SimChSimple32::Tx(u32 data, u32 timeStamp)
{
    Probe(timeStamp);
    ct.power = 1;
    ct.data32 = ct.data32 | data;
    ct.stamp = timeStamp;
}

void SimCh_Helpler::Init(SimCh_Desc *pDesc)
{
    pDesc->repeatType = 0;
    pDesc->repeatCtr = 0;
    pDesc->activeCycles = 0;
    pDesc->act = 0;
    pDesc->result = 0;
    pDesc->simDb = NULL;
    pDesc->timeDesc = NULL;
    pDesc->chType = 0;
    pDesc->chHdl = NULL;
    pDesc->hdlA = NULL;
    pDesc->hdlB = NULL;
    pDesc->hdlC = NULL;
    pDesc->cb = NULL;
    pDesc->upperCb = NULL;
}

void SimCh_Helpler::Bind(void *simDb, void *timeDesc)
{
    SimTime3_Desc *pTimeDesc;
    BASIC_ASSERT(desc.timeDesc != NULL);
    pTimeDesc = (SimTime3_Desc *)desc.timeDesc;
    pTimeDesc->cb = (SimeTime3_CB_t)SimCh_Helpler_Time3RepeatCB;
    pTimeDesc->hdlA = (void *)this;

    desc.simDb=simDb;
    desc.timeDesc=timeDesc;
}
/*
void SimCh_Helpler::GoInTime(timeint_t startTime)
{
    SimEngine_Desc *simDb = (SimEngine_Desc *)desc.simDb;
    class SimTime3 *timeDb = (class SimTime3 *)(simDb->timeDb);
}
*/


/*

SimDB
  TimeDB
        Time3Desc       SimCh_Desc
          hdlA-->       <-- timeDesc
   (lower level binding)


*/
u32 SimCh_Helpler_Time3RepeatCB(SimTime3_Desc *desc)
{
    return 0;
}