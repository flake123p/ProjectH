
#include "Everything_SimEngine.hpp"


void SimTime3::Init(void)
{
    DLLIST_HEAD_RESET(&head);
    currTime = 0;
    minNextTime = 0;
    pEntry = NULL;
    pLeave = NULL;
}

void SimTime3::Uninit(void)
{
    Init();
}

int SimTime3::ClearDesc(SimTime3_Desc *pDes)
{
    pDes->remain_time = 0;
    pDes->flag = 0;
    pDes->req = 0;
    pDes->cb = NULL;
    pDes->timeDb = NULL;
    pDes->hdlA = NULL;
    pDes->hdlB = NULL;
    return 0;
}

int SimTime3::InsertHead(SimTime3_Desc *pDes, int doInit /* = 1 */)
{
    if (doInit) {
        pDes->timeDb = (void *)this;
    }
    DLLIST_INSERT_FIRST(&head, pDes);
    return 0;
}

int SimTime3::InsertTail(SimTime3_Desc *pDes, int doInit /* = 1 */)
{
    if (doInit) {
        pDes->timeDb = (void *)this;
    }
    DLLIST_INSERT_LAST(&head, pDes);
    return 0;
}

int SimTime3::Remove(SimTime3_Desc *pDes)
{
    FLG_ADD(pDes->req, TIME3REQ_BYPASS_CB); //for re-insert in callback
    if (pDes == next) {
        next = (SimTime3_Desc *)DLLIST_NEXT(next);
    }
    DLLIST_REMOVE_NODE(&head, pDes);
    return 0;
}

int SimTime3::Run(void)
{
    //u32 ret;
    uint i = 0;
    SimTime3_Desc *curr;

    while (1)
    {
        // find min
        i = 0;
        minNextTime = MAX_TIME_INT;
        DLLIST_FOREACH(&head, curr, SimTime3_Desc)
        {
            //clear old requests
            curr->req = 0;

            if (curr->remain_time) {
                i++;
                if (curr->remain_time< minNextTime) {
                    minNextTime = curr->remain_time;
                    if (minNextTime <= 1) {
                        break;
                    }
                }
            }
        }
        if (i==0) {
            break; //no active nodes
        }

        //Go
        currTime += minNextTime;

        if (pEntry) {
            if (pEntry->cb) {
                pEntry->flag = TIME3FLAG_IS_IN_ENTRY_CB;
                (*(pEntry->cb))((Handle_t)pEntry);
            }
        }

        curr = (SimTime3_Desc *)DLLIST_HEAD(&head);
        while (1)
        {
            if (curr) {
                next = (SimTime3_Desc *)DLLIST_NEXT(curr);
            } else {
                break; // no nodes
            }
            //extract next before doing callback, because remove node might happened in callback
            do {
                if (0 == curr->remain_time) {
                    break;
                }
                if (FLG_CHK(curr->req, TIME3REQ_BYPASS_CB)) { // this node might be a new node and just been removed
                    break;
                }
                curr->remain_time -= minNextTime;
                if (curr->remain_time) {
                    break;
                }
                //timesup
                if (curr->cb) {
                    curr->flag = TIME3FLAG_IS_IN_TIMESUP_CB;
                    /*ret = */(*(curr->cb))((Handle_t)curr);
                }
            } while (0);
            curr = next;
        }

        if (pLeave) {
            if (pLeave->cb) {
                pEntry->flag = TIME3FLAG_IS_IN_LEAVE_CB;
                (*(pLeave->cb))((Handle_t)pLeave);
            }
        }

    }
    return 0;
}

void SimTime3::Dump(SimTime3_Desc *pDes)
{
    printf("remain_time = %lu\n", (unsigned long)pDes->remain_time);
    printf("flag = 0x%08X\n", pDes->flag);
    printf("req = 0x%08X\n", pDes->req);
    printf("cb = %p\n", pDes->cb);
    printf("timeDb = %p\n", pDes->timeDb);
    printf("hdlA = %p\n", pDes->hdlA);
    printf("hdlB = %p\n", pDes->hdlB);
}

void SimTime3::DumpAll(void)
{
    uint i = 0;
    SimTime3_Desc *curr;

    printf("====== DUMP ALL ======\n");
    printf("currTime = %lu\n", (unsigned long)currTime);
    printf("minNextTime = %lu\n", (unsigned long)minNextTime);
    DLLIST_FOREACH(&head, curr, SimTime3_Desc) {
        printf("====== NODE:%d ======\n", i);
        i++;
        Dump(curr);
    }
}
