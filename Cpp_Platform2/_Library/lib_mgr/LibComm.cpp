
#include "Everything_Lib_Mgr.hpp"

typedef struct {
    u32 currClk;
    u32 interval;
    u32 widenClks; //if widenClks is < 1, auto phase alighment is fine

    u32 nextRxAnchorClk;
    u32 nextRxCenterClk;
    u32 windowSize;
} LibComm_ClkToSync_t;

void LibComm_GetNextRxWindow(LibComm_ClkToSync_t *info)
{
    info->nextRxAnchorClk = info->currClk + info->interval - info->widenClks;
    info->nextRxCenterClk = info->currClk + info->interval;

    info->windowSize= (2 * info->widenClks);
}

void LibComm_RxOkClkUpdate(LibComm_ClkToSync_t *info, u32 currClk)
{
    u32 clkDiff = info->nextRxCenterClk - currClk;

    info->currClk = currClk;

    if (clkDiff & 0x80000000) {
        clkDiff = 0 - clkDiff;
        printf("peer anchor is after us in %d clocks.\n", clkDiff);
    } else {
        printf("peer anchor is before us in %d clocks.\n", clkDiff);
    }
}

int LibComm_ArrayCompare(u16 *ruleArray, u32 ruleLen, u8 *dataForCompare, u32 dataLen, int print_log /* = 1 */)
{
    if (ruleLen != dataLen) {
        if (print_log) {
            printf("length error, ruleLen:%d, dataLen:%d\n", ruleLen, dataLen);
        }
        return 1;
    }

    FOR_I(ruleLen) {
        if (ruleArray[i] & 0x8000)
            continue; //skip compare this byte

        if ((ruleArray[i] & 0xFF) != dataForCompare[i]) {
            if (print_log) {
                printf("data error in index:%d, ruleArray[i]:0x%02X, dataForCompare[i]:0x%02X\n", i, (ruleArray[i] & 0xFF), dataForCompare[i]);
            }
            return 2;
        }
    }

    return 0;
}

void LibComm_Demo(void)
{
    /*
        peer: 100 200 300
               97 194 291
              103 206 309
    */
    LibComm_ClkToSync_t demo = {0, 100, 3};
    LibComm_GetNextRxWindow(&demo);
    printf("RX start in %d, end in %d\n", demo.nextRxAnchorClk, demo.nextRxAnchorClk + demo.windowSize);

    LibComm_RxOkClkUpdate(&demo, 97);
    LibComm_GetNextRxWindow(&demo);
    printf("RX start in %d, end in %d\n", demo.nextRxAnchorClk, demo.nextRxAnchorClk + demo.windowSize);

    LibComm_RxOkClkUpdate(&demo, 194);
    LibComm_GetNextRxWindow(&demo);
    printf("RX start in %d, end in %d\n", demo.nextRxAnchorClk, demo.nextRxAnchorClk + demo.windowSize);

    u32 a;
    int is_before;
    u32 diff;
//    u32 before = 0xFFFFFFFA;
    u32 after = 0x19;

    a = 0xFFFFFFFF;
    diff = a - after;
    if (diff & 0x80000000) {
        diff = 0 - diff;
        is_before = 0;
    } else {
        is_before = 1;
    }
    DUMPX(diff);
    DUMPND(is_before);
}
