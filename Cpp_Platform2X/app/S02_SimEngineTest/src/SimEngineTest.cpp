
#include "Everything_App.hpp"

void SimChSimple32Test(void)
{
    class SimChSimple32 ch;

    ch.ct.dataRetainCycles = 10;
    BASIC_ASSERT(ch.ct.power == 0);

    ch.Tx(0x65, 0);
    DUMPNX(ch.ct.data32);
    BASIC_ASSERT(ch.ct.data32 == 0x65);
    BASIC_ASSERT(ch.ct.power == 1);
    
    ch.Probe(9);
    DUMPNX(ch.ct.data32);
    BASIC_ASSERT(ch.ct.data32 == 0x65);
    BASIC_ASSERT(ch.ct.power == 1);

    ch.Probe(10);
    DUMPNX(ch.ct.data32);
    BASIC_ASSERT(ch.ct.data32 == 0);
    BASIC_ASSERT(ch.ct.power == 0);

    ch.Tx(0x0F, 11);
    ch.Tx(0xF0, 12);
    BASIC_ASSERT(ch.ct.data32 == 0xFF);

    ch.Probe(21);
    BASIC_ASSERT(ch.ct.data32 == 0xFF);
    ch.Probe(22);
    BASIC_ASSERT(ch.ct.data32 == 0);

    printf("ALL PASS!!\n");

    A_Main();
}

/*
CSMA/CD
Preamble <1 byte 0xA5>
DA <1 byte>
DS <1 byte>
LEN <1 byte>
DATA <LEN byte>
end_symbol <2 byte 0 nothing>

Pre TX probe:
waiting 1 byte end_symbol(no power in channel)
*/
SimEngine_Desc A_SimDb;
class SimTime3 A_TimeDb;
class SimChSimple32 A_Ch;
SimTime3_Desc Aar,Abr,Acr,Aat,Abt,Act;
typedef struct {
    int isTxing;
    int isRxing;
} Ainst_t;
class Ainst {
public:
    Ainst_t ct;
    Ainst(void){memset(&ct,0,sizeof(Ainst_t));};
    ~Ainst(void){};
};
Ainst Ainsta, Ainstb, Ainstc;
u32 SimTime3_DemoCBxx(SimTime3_Desc *pDesc)
{
    class SimTime3 *timeDb = (class SimTime3 *)pDesc->timeDb;
    printf("ss currTime = %lu\n", (unsigned long)timeDb->currTime);
    static int x = 0;
    x++;
    if (x==2)
    {
        //timeDb->Remove(&b);
        //timeDb->DumpAll();
    }
    
    return 0;
}

void A_Main(void)
{
    A_SimDb.timeDb = (class SimTime3*)&A_TimeDb;

    A_TimeDb.ClearDesc(&Aar);
    Aar.remain_time = 1;
    Aar.cb = (SimeTime3_CB_t)SimTime3_DemoCBxx;
    Aar.hdlA = &Ainsta;
    A_TimeDb.ClearDesc(&Aat);
    Aat.remain_time = 1;
    Aat.cb = (SimeTime3_CB_t)SimTime3_DemoCBxx;
    
    A_TimeDb.ClearDesc(&Abr);
    Abr.remain_time = 1;
    Abr.cb = (SimeTime3_CB_t)SimTime3_DemoCBxx;
    A_TimeDb.ClearDesc(&Abt);
    Abt.remain_time = 1;
    Abt.cb = (SimeTime3_CB_t)SimTime3_DemoCBxx;
    
    A_TimeDb.ClearDesc(&Acr);
    Acr.remain_time = 1;
    Acr.cb = (SimeTime3_CB_t)SimTime3_DemoCBxx;
    A_TimeDb.ClearDesc(&Act);
    Act.remain_time = 1;
    Act.cb = (SimeTime3_CB_t)SimTime3_DemoCBxx;

    A_TimeDb.InsertHead(&Aat);
    A_TimeDb.InsertHead(&Abt);
    A_TimeDb.InsertHead(&Act);
    A_TimeDb.InsertTail(&Aar);
    A_TimeDb.InsertTail(&Abr);
    A_TimeDb.InsertTail(&Acr);
    //PRINT_LINE
    //timeDb.DumpAll();
    //A_TimeDb.Run();
    //PRINT_LINE
}
