
#include "Everything_App.hpp"

class SimTime3 timeDb;
SimTime3_Desc a,b,c;

u32 SimTime3_DemoCB(SimTime3_Desc *pDesc)
{
    class SimTime3 *timeDb = (class SimTime3 *)pDesc->timeDb;
    printf("ss currTime = %lu\n", (unsigned long)timeDb->currTime);
    static int x = 0;
    x++;
    if (x==2)
    {
        timeDb->Remove(&b);
        timeDb->DumpAll();
    }
    
    return 0;
}



int SimTime3_Demo(void)
{
    timeDb.ClearDesc(&a);
    a.remain_time = 1;
    a.cb = (SimeTime3_CB_t)SimTime3_DemoCB;
    
    timeDb.ClearDesc(&b);
    b.remain_time = 4;
    b.cb = (SimeTime3_CB_t)SimTime3_DemoCB;
    
    timeDb.ClearDesc(&c);
    c.remain_time = 7;
    c.cb = (SimeTime3_CB_t)SimTime3_DemoCB;

    timeDb.InsertHead(&a);
    timeDb.InsertHead(&b);
    timeDb.InsertTail(&c);
    //PRINT_LINE
    //timeDb.DumpAll();
    timeDb.Run();
    //PRINT_LINE
    return 0;
}


