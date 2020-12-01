
#include "Everything_Lib_Mgr.hpp"

#define LIB_VCD_DEBUG_MSG ( 1 )

#if 1
#define LIB_VCD_ASSERT BASIC_ASSERT
#else
#define LIB_VCD_ASSERT(...)
#endif

typedef struct {
    LibVCD_WireInfo_t ori;
    char ch_symbol; //0x21~0x7E ... TBD
} _LibVCD_WireInfo_Internal_t;

typedef struct {
    Lib_Desc_Info_t desc_entry;
    LibVcd_WireValue_t data;
} _LibVCD_WireValueDesc_t;

u32 gLibVCD_TimeScale;
TIME_UNIT_t gLibVCD_TimeUnit;
_LibVCD_WireInfo_Internal_t *gLibVCD_InfoArry = NULL;
u32 gLibVCD_NumberOfInfo;
//LibFileIoClass gLibVCD_OutFile = LibFileIoClass("12.txt", "w+b");
FILE *gLibVCD_fp = NULL;
const char * gLibVCD_FileName;
//u32 gLibVCD_ClkLow = 0;
//u32 gLibVCD_ClkHigh = 0;
//LibU64_t gLibVCD_AccuClks = {0};
u32 gLibVCD_AccuClks = 0;
Lib_Desc_Head_t *gLibVCD_JobHead = NULL;
int gLibVCD_IsValueNotPrintYet = 0;

//
//  LA global variables (time node & value change node)
//
#define VCD_LA_HEAD (&gLibVCD_LA_Head)
#define VCD_LA_TAIL (gLibVCD_LA_Head.tail_or_self)

DLList_Head_t gLibVCD_LA_Head;
typedef struct {
    DLList_Entry_t next;
    DLList_Head_t  valueChangeHead;
    u32 clocksToAdd;
} _LibVCD_LA_TimeNode_t;
typedef struct {
    DLList_Entry_t next;
    int isDontCare;
    u32 index;
    u32 value;
} _LibVCD_LA_ValueChangeNode_t;
u32 gLibVCD_LA_BeforeClks; //input
u32 gLibVCD_LA_AfterClks;  //input
//u32 gLibVCD_LA_TriggerClkLow;
//u32 gLibVCD_LA_TriggerClkHigh;

int gLibVCD_LA_IsUsingTrigger = 0;
int gLibVCD_LA_IsTriggered = 0;
int gLibVCD_LA_IsAfterClksFull = 0;

//LibU64_t gLibVCD_StartClks = {0,0}; //increase only when trigger clock buffer is full
//LibU64_t gLibVCD_LA_CurrClks = {0}; //increase in every "clocks add".
//LibU64_t gLibVCD_LA_TriggerClks = {0}; //save trigger clocks
u32 gLibVCD_LA_VcdStartClks = 0; //increase only when trigger clock buffer is full
u32 gLibVCD_LA_CurrClks = 0; //increase in every "clocks add".
u32 gLibVCD_LA_TriggerClks = 0; //save trigger clocks

u32 gLibVCD_LA_TriggerIndex; //last index


char *_LibVCD_StringOfValue(u32 num_of_bits, int isValueDontCare, u32 value)
{
    static char charBuf[260];

    BASIC_ASSERT(num_of_bits <= 256);
    BASIC_ASSERT(num_of_bits >= 1);

    if (isValueDontCare) {
        if (num_of_bits == 1) {
            charBuf[0] = 'x';
            charBuf[1] = 0;
        } else {
            charBuf[0] = 'b';
            charBuf[1] = 'x';
            charBuf[2] = ' ';
            charBuf[3] = 0;
        }
        return charBuf;
    }

    if (num_of_bits == 1) {
        BASIC_ASSERT(value <= 1);
        charBuf[0] = '0' + value;
        charBuf[1] = 0;
    } else {
        charBuf[0] = 'b';
        FOR_I(num_of_bits) {
            charBuf[1+num_of_bits-1-i] = value & 0x01 ? '1' : '0';
            value = value >> 1;
        }
        charBuf[num_of_bits+1] = ' ';
        charBuf[num_of_bits+2] = 0;
    }

    return charBuf;
}

int _LibVCD_Init_0_InitList(u32 num_of_desc)
{
    gLibVCD_JobHead = LibDesc_CreateList(sizeof(Lib_Desc_Head_t), num_of_desc, sizeof(_LibVCD_WireValueDesc_t));
    return 0;
}

int _LibVCD_Init_1_SaveToGlobal(const char *outFileName, u32 timescale, TIME_UNIT_t unit, LibVCD_WireInfo_t *info, u32 num_of_info)
{
    BASIC_ASSERT(num_of_info <= 0x7E - 0x21 + 1);

    //store all info
    gLibVCD_TimeScale = timescale;
    gLibVCD_TimeUnit = unit;
    gLibVCD_InfoArry = (_LibVCD_WireInfo_Internal_t *)MM_ALLOC(sizeof(_LibVCD_WireInfo_Internal_t) * num_of_info);
    FOR_I(num_of_info) {
        WT(gLibVCD_InfoArry[i].ori.num_of_bits , info[i].num_of_bits);
        WT(gLibVCD_InfoArry[i].ori.wire_name , info[i].wire_name);
        WT(gLibVCD_InfoArry[i].ori.isInitValueDontCare , info[i].isInitValueDontCare);
        WT(gLibVCD_InfoArry[i].ori.initValue , info[i].initValue);
        WT(gLibVCD_InfoArry[i].ch_symbol , 0x21 + i);
    }
    gLibVCD_FileName = outFileName;
    gLibVCD_NumberOfInfo = num_of_info;
    return 0;
}

int _LibVCD_Init_2_DumpFileHeader(void)
{
    const char *unitStr;

    //write init data to file
    gLibVCD_fp = fopen(gLibVCD_FileName, "w+b");
    BASIC_ASSERT(gLibVCD_fp != NULL);
    switch (gLibVCD_TimeUnit) {
        case TIME_UNIT_PS: unitStr = "ps"; break;
        case TIME_UNIT_NS: unitStr = "ns"; break;
        case TIME_UNIT_US: unitStr = "us"; break;
        case TIME_UNIT_S:  unitStr = "s"; break;
        default: BASIC_ASSERT(0); break;
    }
    fprintf(gLibVCD_fp, "$timescale\n");
    fprintf(gLibVCD_fp, "    %d%s\n", gLibVCD_TimeScale, unitStr);
    fprintf(gLibVCD_fp, "$end\n\n");

    fprintf(gLibVCD_fp, "$scope module M1 $end\n");
    FOR_I(gLibVCD_NumberOfInfo) {
        fprintf(gLibVCD_fp,
            "$var wire %d %c %s $end\n",
            gLibVCD_InfoArry[i].ori.num_of_bits,
            gLibVCD_InfoArry[i].ch_symbol,
            gLibVCD_InfoArry[i].ori.wire_name);
    }
    fprintf(gLibVCD_fp, "$upscope $end\n\n");

    return 0;
}

int _LibVCD_Init_3_DumpInitValue(void)
{
    fprintf(gLibVCD_fp, "#0\n");
    FOR_I(gLibVCD_NumberOfInfo) {
        fprintf(gLibVCD_fp,
            "%s%c\n",
            _LibVCD_StringOfValue(gLibVCD_InfoArry[i].ori.num_of_bits, gLibVCD_InfoArry[i].ori.isInitValueDontCare, gLibVCD_InfoArry[i].ori.initValue),
            gLibVCD_InfoArry[i].ch_symbol);
    }
    fprintf(gLibVCD_fp, "\n");

    return 0;
}

int LibVCD_Init(const char *outFileName, u32 timescale, TIME_UNIT_t unit, LibVCD_WireInfo_t *info, u32 num_of_info)
{
    _LibVCD_Init_0_InitList(num_of_info);
    _LibVCD_Init_1_SaveToGlobal(outFileName, timescale, unit, info, num_of_info);
    _LibVCD_Init_2_DumpFileHeader();
    _LibVCD_Init_3_DumpInitValue();

    return 0;
}

int LibVCD_Uninit(void)
{
    //BASIC_ASSERT(gLibVCD_AccuClks.high == 0); //not implement u64 print
    fprintf(gLibVCD_fp, "#%d\n", gLibVCD_AccuClks);
    gLibVCD_AccuClks = 0;
    gLibVCD_IsValueNotPrintYet = 0;

    if (gLibVCD_InfoArry != NULL) {
        MM_FREE(gLibVCD_InfoArry);
    }

    if (gLibVCD_fp != NULL) {
        fclose(gLibVCD_fp);
    }

    if (gLibVCD_JobHead != NULL) {
        LibDesc_DestroyList(gLibVCD_JobHead, 1);
        gLibVCD_JobHead = NULL;
    }

    return 0;
}

int LibVCD_DumpAllCurrValues(void)
{
    if (gLibVCD_IsValueNotPrintYet) {
        gLibVCD_IsValueNotPrintYet = 0;

        _LibVCD_WireValueDesc_t *prev_desc;
        _LibVCD_WireValueDesc_t *curr_desc;

        //BASIC_ASSERT(gLibVCD_AccuClks.high == 0); //not implement u64 print
        fprintf(gLibVCD_fp, "#%d\n", gLibVCD_AccuClks);
        DLLIST_WHILE_START(&(gLibVCD_JobHead->head_of_using), curr_desc, _LibVCD_WireValueDesc_t)
        {
            prev_desc = curr_desc;
            DLLIST_WHILE_NEXT(curr_desc, _LibVCD_WireValueDesc_t);
            //dump
            {
                u32 index = prev_desc->data.index;
                fprintf(gLibVCD_fp,
                    "%s%c\n",
                    _LibVCD_StringOfValue(gLibVCD_InfoArry[index].ori.num_of_bits,
                                              prev_desc->data.flag & LIB_VCD_IS_VAL_DONT_CARE,
                                              prev_desc->data.value),
                    gLibVCD_InfoArry[index].ch_symbol);
            }
            DLLIST_REMOVE_NODE(&(gLibVCD_JobHead->head_of_using), prev_desc);
            if (prev_desc->desc_entry.is_pre_allocate)
            {
                DLLIST_INSERT_LAST(&(gLibVCD_JobHead->head_of_pool), prev_desc);
            }
            else
            {
                MM_FREE(prev_desc);
            }
        }
        fprintf(gLibVCD_fp, "\n");

    }
    return 0;
}

int LibVCD_ClockAdd(u32 clocksToAdd)
{
    MUTEX_LIB_VCD_LOCK;
    BASIC_ASSERT(gLibVCD_JobHead != NULL); // not inited
    if (clocksToAdd) {
        LibVCD_DumpAllCurrValues();

        //LibU64_AddU32(&gLibVCD_AccuClks, clocksToAdd);
        gLibVCD_AccuClks += clocksToAdd;
        //LibUtil_AddInU64_TwoU32(&gLibVCD_ClkHigh, &gLibVCD_ClkLow, clocksToAdd);
    }
    MUTEX_LIB_VCD_UNLOCK;
    return 0;
}

int LibVCD_ValueChangeToDontCare(u32 index)
{
    _LibVCD_WireValueDesc_t *desc;
    MUTEX_LIB_VCD_LOCK;
    BASIC_ASSERT(gLibVCD_JobHead != NULL); // not inited
    desc = (_LibVCD_WireValueDesc_t *)LibDesc_GetDesc(gLibVCD_JobHead);
    desc->data.index = index;
    desc->data.flag = LIB_VCD_IS_VAL_DONT_CARE;
    desc->data.value = 0;
    gLibVCD_IsValueNotPrintYet = 1;
    MUTEX_LIB_VCD_UNLOCK;
    return 0;
}

int LibVCD_ValueChange(u32 index, u32 value)
{
    _LibVCD_WireValueDesc_t *desc;
    MUTEX_LIB_VCD_LOCK;
    BASIC_ASSERT(gLibVCD_JobHead != NULL); // not inited
    desc = (_LibVCD_WireValueDesc_t *)LibDesc_GetDesc(gLibVCD_JobHead);
    desc->data.index = index;
    desc->data.flag = LIB_VCD_FLAG_NULL;
    desc->data.value = value;
    gLibVCD_IsValueNotPrintYet = 1;
    MUTEX_LIB_VCD_UNLOCK;
    return 0;
}

void LibVCD_Demo(void)
{
    printf("%s()\n", __func__);

    LibVCD_WireInfo_t test[3] = {
        {1, "ax", VALUE_IN_FOLLOWING, 1},
        {2, "bx", VALUE_IS_DONT_CARE, 0},
        {8, "cx", VALUE_IN_FOLLOWING, 0},
    };
    LibVCD_Init("example.vcd", 1, TIME_UNIT_US, test, 3);
    LibVCD_ClockAdd(300);
    LibVCD_ValueChange(0, 0);
    LibVCD_ClockAdd(200);
    LibVCD_ValueChangeToDontCare(2);
    LibVCD_ClockAdd(100);
    LibVCD_ValueChange(2, 255);
    LibVCD_ValueChange(1, 2);
    LibVCD_ClockAdd(100);
    LibVCD_Uninit();
}

/*
clock -> wire -> wire -> wire
|
clock -> wire
|
clock
|
clock -> wire -> wire
*/
DLList_Head_t gAllRecords = DLLIST_HEAD_INIT(&gAllRecords);

int _LibVCD_LA_FreeAllValueNodes(_LibVCD_LA_TimeNode_t *timeNode)
{
    _LibVCD_LA_ValueChangeNode_t *currValueNode;
    _LibVCD_LA_ValueChangeNode_t *oldValueNode;

    DLLIST_WHILE_START(&(timeNode->valueChangeHead),currValueNode,_LibVCD_LA_ValueChangeNode_t)
    {
        oldValueNode = currValueNode;
        DLLIST_WHILE_NEXT(currValueNode,_LibVCD_LA_ValueChangeNode_t);
        MM_FREE(oldValueNode);
    }

    DLLIST_HEAD_RESET(&timeNode->valueChangeHead);

    return 0;
}

int _LibVCD_LA_MoveValueNodes(_LibVCD_LA_TimeNode_t *timeNodeFrom, _LibVCD_LA_TimeNode_t *timeNodeTo)
{
    int isExistInTimeNodeTo;
    _LibVCD_LA_ValueChangeNode_t *currValueNode;
    _LibVCD_LA_ValueChangeNode_t *oldValueNode;
    _LibVCD_LA_ValueChangeNode_t *currValueNode2;

    DLLIST_WHILE_START(&(timeNodeFrom->valueChangeHead),currValueNode,_LibVCD_LA_ValueChangeNode_t)
    {
        oldValueNode = currValueNode;
        DLLIST_WHILE_NEXT(currValueNode,_LibVCD_LA_ValueChangeNode_t);

        isExistInTimeNodeTo = 0;
        DLLIST_FOREACH(&(timeNodeTo->valueChangeHead),currValueNode2,_LibVCD_LA_ValueChangeNode_t)
        {
            if (oldValueNode->index == currValueNode2->index) {
                isExistInTimeNodeTo = 1;
                break;
            }
        }
        if (isExistInTimeNodeTo) {
            MM_FREE(oldValueNode);
        } else {
            DLLIST_INSERT_LAST(&(timeNodeTo->valueChangeHead), oldValueNode);
        }
    }

    return 0;
}

int LibVCD_LA_DumpAll(void)
{
    u32 startClks = gLibVCD_LA_VcdStartClks;
    _LibVCD_LA_TimeNode_t *currTimeNode;
    _LibVCD_LA_ValueChangeNode_t *currValueNode;

    printf("////// %s() //////\n", __func__);
    printf("BeforeClks   =%u\n", gLibVCD_LA_BeforeClks);
    printf("AfterClks    =%u\n", gLibVCD_LA_AfterClks);
    printf("VcdStartClks =%u\n", gLibVCD_LA_VcdStartClks);
    printf("CurrClks     =%u\n", gLibVCD_LA_CurrClks);
    printf("TriggerClks  =%u. (usingTrigger:%d/isTriggered:%d/AfterClksFull:%d)\n", 
        gLibVCD_LA_TriggerClks, 
        gLibVCD_LA_IsUsingTrigger, 
        gLibVCD_LA_IsTriggered, 
        gLibVCD_LA_IsAfterClksFull);
    
    DLLIST_FOREACH(VCD_LA_HEAD,currTimeNode,_LibVCD_LA_TimeNode_t)
    {
        //LibU64_AddU32(&startClks, currTimeNode->clocksToAdd);
        startClks += currTimeNode->clocksToAdd;
        printf("[Time node]  clocksToAdd=%-4d clk=%-4d\n", currTimeNode->clocksToAdd, startClks);
        if ( DLLIST_IS_NOT_EMPTY( &currTimeNode->valueChangeHead )) {
            PRINT01("[Value node]\n");
        }
        DLLIST_FOREACH(&(currTimeNode->valueChangeHead),currValueNode,_LibVCD_LA_ValueChangeNode_t)
        {
            if (currValueNode->isDontCare) {
                PRINT02("index=%d  isDontCare=%d\n", currValueNode->index, currValueNode->isDontCare);
            } else {
                PRINT02("index=%d  isDontCare=%d  value=%d\n", currValueNode->index, currValueNode->isDontCare, currValueNode->value);
            }
        }
    }

    return 0;
}

int LibVCD_LA_FlushAll(void)
{
    /*
        1.clocksToAdd of 1st node is always 0.
        2.clocksToAdd of 2nd node will be decreased, when it's 0, 1st node will be removed.
    */
    _LibVCD_LA_TimeNode_t *currTimeNode;
    _LibVCD_LA_TimeNode_t *oldTimeNode;
    _LibVCD_LA_ValueChangeNode_t *currValueNode;
    _LibVCD_LA_ValueChangeNode_t *oldValueNode;

    //print init clocks
    gLibVCD_AccuClks += gLibVCD_LA_VcdStartClks;

    DLLIST_WHILE_START(VCD_LA_HEAD,currTimeNode,_LibVCD_LA_TimeNode_t)
    {
        LibVCD_ClockAdd(currTimeNode->clocksToAdd);
        DLLIST_WHILE_START(&(currTimeNode->valueChangeHead),currValueNode,_LibVCD_LA_ValueChangeNode_t)
        {
            if (currValueNode->isDontCare) {
                LibVCD_ValueChangeToDontCare(currValueNode->index);
            } else {
                LibVCD_ValueChange(currValueNode->index, currValueNode->value);
            }
            oldValueNode = currValueNode;
            DLLIST_WHILE_NEXT(currValueNode,_LibVCD_LA_ValueChangeNode_t);
            MM_FREE(oldValueNode);
        }
        oldTimeNode = currTimeNode;
        DLLIST_WHILE_NEXT(currTimeNode,_LibVCD_LA_TimeNode_t);
        MM_FREE(oldTimeNode);
    }

    return 0;
}


int LibVCD_LA_Init(const char *outFileName, u32 timescale, TIME_UNIT_t unit, LibVCD_WireInfo_t *info, u32 num_of_info)
{
    //add trigger indicate wire
    LibVCD_WireInfo_t *myInfo = (LibVCD_WireInfo_t *)MM_ALLOC(sizeof(LibVCD_WireInfo_t) * (num_of_info+1));

    FOR_I(num_of_info) {
        WT(myInfo[i].num_of_bits , info[i].num_of_bits);
        WT(myInfo[i].wire_name , info[i].wire_name);
        WT(myInfo[i].isInitValueDontCare , info[i].isInitValueDontCare);
        WT(myInfo[i].initValue , info[i].initValue);
    }
    //last one is trigger index
    myInfo[num_of_info].num_of_bits = 1;
    myInfo[num_of_info].wire_name = "Trigger";
    myInfo[num_of_info].isInitValueDontCare = 0;
    myInfo[num_of_info].initValue =0;
    gLibVCD_LA_TriggerIndex = num_of_info;

    DLLIST_HEAD_RESET(VCD_LA_HEAD);
    _LibVCD_Init_0_InitList(num_of_info);
    _LibVCD_Init_1_SaveToGlobal(outFileName, timescale, unit, myInfo, num_of_info+1);
    _LibVCD_Init_2_DumpFileHeader();

    _LibVCD_LA_TimeNode_t *newTimeNode = (_LibVCD_LA_TimeNode_t *)MM_ALLOC(sizeof(_LibVCD_LA_TimeNode_t));
    DLLIST_HEAD_RESET(&(newTimeNode->valueChangeHead));
    newTimeNode->clocksToAdd = 0;
    DLLIST_INSERT_LAST(VCD_LA_HEAD, newTimeNode);

    FOR_I(num_of_info+1) {
        if (myInfo[i].isInitValueDontCare) {
            LibVCD_LA_ValueChangeToDontCare(i);
        } else {
            LibVCD_LA_ValueChange(i, myInfo[i].initValue);
        }
    }

    MM_FREE(myInfo);

    return 0;
}

int LibVCD_LA_InitTrigger(u32 beforeClks, u32 afterClks)
{
    BASIC_ASSERT(gLibVCD_LA_IsUsingTrigger == 0);
    gLibVCD_LA_IsUsingTrigger = 1;

    gLibVCD_LA_BeforeClks = beforeClks;
    gLibVCD_LA_AfterClks = afterClks;
    return 0;
}

int LibVCD_LA_Trigger(void)
{
    BASIC_ASSERT(gLibVCD_LA_IsUsingTrigger == 1);
    gLibVCD_LA_IsTriggered = 1;

    gLibVCD_LA_TriggerClks = gLibVCD_LA_CurrClks;
    LibVCD_LA_ValueChange(gLibVCD_LA_TriggerIndex, 1);
    return 0;
}

int LibVCD_LA_Uninit(void)
{
    return LibVCD_Uninit();
}

static int LibVCD_LA_ClockAdd_BeforeCurrClockAdjust(u32 clocksToAdd)
{
    //LibU64_AddU32(&gLibVCD_LA_CurrClks, clocksToAdd);
    gLibVCD_LA_CurrClks += clocksToAdd;

    return 0;
}

static int LibVCD_LA_ClockAdd_CurrClockAdjust(void)
{
    u32 clkDiff = gLibVCD_LA_CurrClks - gLibVCD_LA_VcdStartClks;

    //need more test
    if (clkDiff > gLibVCD_LA_BeforeClks) {
        u32 remainRedundentClks = clkDiff - gLibVCD_LA_BeforeClks;

        _LibVCD_LA_TimeNode_t *timeNode1st;
        _LibVCD_LA_TimeNode_t *timeNode2nd;

        gLibVCD_LA_VcdStartClks += remainRedundentClks;

        // 1.reduce 2nd node time
        // 2.if 2nd node time is reduced to 0, remove first node.
        while (remainRedundentClks) {
            timeNode1st = (_LibVCD_LA_TimeNode_t *)DLLIST_FIRST(VCD_LA_HEAD);
            timeNode2nd = (_LibVCD_LA_TimeNode_t *)(timeNode1st->next.next);
            if (timeNode2nd == NULL) {
                break;
            }
            if (timeNode2nd->clocksToAdd > remainRedundentClks) {
                timeNode2nd->clocksToAdd -= remainRedundentClks;
                break;
            } else { //if (timeNode2nd->clocksToAdd <= remainRedundentClks)
                remainRedundentClks -= timeNode2nd->clocksToAdd;
                timeNode2nd->clocksToAdd = 0;
                //move first node value information to 2nd node if the value doesn't exist in 2nd node
                _LibVCD_LA_MoveValueNodes(timeNode1st, timeNode2nd);
                //than remove 1st node and conitnue while loop
                DLLIST_REMOVE_FIRST(VCD_LA_HEAD);
                MM_FREE(timeNode1st);
            }
        }
    }

    return 0;
}


//TODO: return 1 if afterClks exceeds the limit.
int LibVCD_LA_ClockAdd(u32 clocksToAdd)
{
    // 1.malloc & insert time node
    // 2.scan & clear old node that is before trigger limits (curr)
    int ret = 0;

    if (clocksToAdd)
    {
        MUTEX_LIB_VCD_LOCK;
        do {

            if (gLibVCD_LA_IsAfterClksFull) {
                ret = 1;
                break;
            }

            if (gLibVCD_LA_IsTriggered) {
                u32 remainClks = gLibVCD_LA_AfterClks - (gLibVCD_LA_CurrClks - gLibVCD_LA_TriggerClks);

                if (remainClks <= clocksToAdd) {
                    gLibVCD_LA_IsAfterClksFull = 1;
                    clocksToAdd = remainClks;
                    ret = 1;
                }
            }

            {
                _LibVCD_LA_TimeNode_t *newTimeNode = (_LibVCD_LA_TimeNode_t *)MM_ALLOC(sizeof(_LibVCD_LA_TimeNode_t));
                DLLIST_HEAD_RESET(&(newTimeNode->valueChangeHead));
                newTimeNode->clocksToAdd = clocksToAdd;
                DLLIST_INSERT_LAST(VCD_LA_HEAD, newTimeNode);

                LibVCD_LA_ClockAdd_BeforeCurrClockAdjust(clocksToAdd);
                if (gLibVCD_LA_IsTriggered == 0) {
                    LibVCD_LA_ClockAdd_CurrClockAdjust();
                }
            }
        } while (0);
        MUTEX_LIB_VCD_UNLOCK;
    }

    return ret;
}

int LibVCD_LA_ValueChangeToDontCare(u32 index)
{
    int ret = 0;

    MUTEX_LIB_VCD_LOCK;
    do
    {
        if (gLibVCD_LA_IsAfterClksFull) {
            ret = 1;
            break;
        }

        {
            // 1.find current time node
            // 2.malloc vc node
            // 3.insert last into vc list in time node

            _LibVCD_LA_TimeNode_t *lastTimeNode = (_LibVCD_LA_TimeNode_t *)VCD_LA_TAIL;
            _LibVCD_LA_ValueChangeNode_t *newValueNode = (_LibVCD_LA_ValueChangeNode_t *)MM_ALLOC(sizeof(_LibVCD_LA_ValueChangeNode_t));

            LIB_VCD_ASSERT( DLLIST_IS_NOT_EMPTY(VCD_LA_HEAD) );
            //DUMPNP(lastTimeNode);
            
            newValueNode->index = index;
            newValueNode->isDontCare = 1;

            DLLIST_INSERT_LAST(&(lastTimeNode->valueChangeHead), newValueNode);
        }
    } while (0);
    MUTEX_LIB_VCD_UNLOCK;

    return ret;
}

int LibVCD_LA_ValueChange(u32 index, u32 value)
{
    int ret = 0;

    MUTEX_LIB_VCD_LOCK;
    do
    {
        if (gLibVCD_LA_IsAfterClksFull) {
            ret = 1;
            break;
        }

        {
            // 1.find current time node
            // 2.malloc vc node
            // 3.insert last into vc list in time node

            _LibVCD_LA_TimeNode_t *lastTimeNode = (_LibVCD_LA_TimeNode_t *)VCD_LA_TAIL;
            _LibVCD_LA_ValueChangeNode_t *newValueNode = (_LibVCD_LA_ValueChangeNode_t *)MM_ALLOC(sizeof(_LibVCD_LA_ValueChangeNode_t));

            LIB_VCD_ASSERT( DLLIST_IS_NOT_EMPTY(VCD_LA_HEAD) );
            //DUMPNP(lastTimeNode);
            
            newValueNode->index = index;
            newValueNode->isDontCare = 0;
            newValueNode->value = value;

            DLLIST_INSERT_LAST(&(lastTimeNode->valueChangeHead), newValueNode);
        }
    } while (0);
    MUTEX_LIB_VCD_UNLOCK;

    return ret;
}

void LibVCD_LA_Test0(void)
{
    printf("%s()\n", __func__);

    LibVCD_WireInfo_t test[3] = {
        {1, "ax", VALUE_IN_FOLLOWING, 1},
        {2, "bx", VALUE_IS_DONT_CARE, 0},
        {8, "cx", VALUE_IN_FOLLOWING, 0},
    };

    LibVCD_LA_Init("example.vcd", 1, TIME_UNIT_US, test, 3);
    LibVCD_LA_InitTrigger(0,0xffffffff);
    //LibVCD_LA_InitTrigger(250,300);
    LibVCD_LA_ClockAdd(0);
    LibVCD_LA_ClockAdd(100);
    LibVCD_LA_ValueChange(2, 7);
    LibVCD_LA_ClockAdd(100);
    LibVCD_LA_ValueChange(2, 9);
    LibVCD_LA_ClockAdd(100);
    LibVCD_LA_Trigger();
    LibVCD_LA_ClockAdd(100);
    LibVCD_LA_ClockAdd(100);
    LibVCD_LA_ValueChange(0, 0);
    LibVCD_LA_ValueChangeToDontCare(2);
    LibVCD_LA_ClockAdd(500);
    LibVCD_LA_ClockAdd(1);
    LibVCD_LA_DumpAll();
    LibVCD_LA_FlushAll();
    /*
      TODO:
        [x]flush()
        [-]64bit obj
        [-]128bit obj
        [x]clockNode remove
        [x]clockNode first anchor node when clockNode removed
        [ ]trigger
    */
    /*
    LibVCD_ClockAdd(300);
    LibVCD_ValueChange(0, 0);
    LibVCD_ClockAdd(200);
    LibVCD_ValueChangeToDontCare(2);
    LibVCD_ClockAdd(100);
    LibVCD_ValueChange(2, 255);
    LibVCD_ValueChange(1, 2);
    LibVCD_ClockAdd(100);
    */
    LibVCD_LA_Uninit();
}

//64bits clock test
void LibVCD_LA_Test1(void)
{
}

void LibVCD_LA_Demo(void)
{
    LibVCD_LA_Test0();
}

