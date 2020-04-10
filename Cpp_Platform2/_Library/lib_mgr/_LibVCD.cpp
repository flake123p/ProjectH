
#include "Everything_Lib_Mgr.hpp"

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
LibU64_t gLibVCD_AccuClks = {0};
Lib_Desc_Head_t *gLibVCD_JobHead;
int gLibVCD_IsValueNotPrintYet = 0;

//
//  LA global variables (time node & value change node)
//
DLList_Head_t gLibVCD_LA_Head;
typedef struct {
    DLList_Entry_t next;
    DLList_Entry_t valueChangeEntry;
    u32 clockToAdd;
} _LibVCD_LA_TimeNode_t;
typedef struct {
    DLList_Entry_t next;
    int isDontCare;
    u32 index;
    u32 value;
} _LibVCD_LA_ValueChangeNode_t;
u32 gLibVCD_LA_ClocksBefore;
u32 gLibVCD_LA_ClocksAfter;
u32 gLibVCD_LA_TriggerClkLow;
u32 gLibVCD_LA_TriggerClkHigh;
int gLibVCD_LA_IsTriggered = 0;

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
    BASIC_ASSERT(gLibVCD_AccuClks.high == 0); //not implement u64 print
    fprintf(gLibVCD_fp, "#%d\n", gLibVCD_AccuClks.low);

    if (gLibVCD_InfoArry != NULL) {
        MM_FREE(gLibVCD_InfoArry);
    }

    if (gLibVCD_fp != NULL) {
        fclose(gLibVCD_fp);
    }

    LibDesc_DestroyList(gLibVCD_JobHead, 1);

    return 0;
}

int LibVCD_DumpAllCurrValues(void)
{
    if (gLibVCD_IsValueNotPrintYet) {
        gLibVCD_IsValueNotPrintYet = 0;

        _LibVCD_WireValueDesc_t *prev_desc;
        _LibVCD_WireValueDesc_t *curr_desc;

        BASIC_ASSERT(gLibVCD_AccuClks.high == 0); //not implement u64 print
        fprintf(gLibVCD_fp, "#%d\n", gLibVCD_AccuClks.low);
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
    if (clocksToAdd) {
        LibVCD_DumpAllCurrValues();

        LibU64_AddU32(&gLibVCD_AccuClks, clocksToAdd);
        //LibUtil_AddInU64_TwoU32(&gLibVCD_ClkHigh, &gLibVCD_ClkLow, clocksToAdd);
    }
    MUTEX_LIB_VCD_UNLOCK;
    return 0;
}

int LibVCD_ValueChangeToDontCare(u32 index)
{
    _LibVCD_WireValueDesc_t *desc;
    MUTEX_LIB_VCD_LOCK;
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
typedef struct {
    LIB_VCD_FLAG_t flag;
    union {
        struct {
            u32 value;
            u32 index;
            DLList_Entry_t wireEntry;
        } wire;
        struct {
            u32 clocksToAdd;
            DLList_Head_t wireHead;
            DLList_Entry_t clockEntry;
        } clock;
    } unidata;
} LibVCD_LA_Cell_t;

DLList_Head_t gAllRecords = DLLIST_HEAD_INIT(&gAllRecords);

int LibVCD_LA_Init(const char *outFileName, u32 timescale, TIME_UNIT_t unit, LibVCD_WireInfo_t *info, u32 num_of_info)
{
    DLLIST_HEAD_RESET(&gLibVCD_LA_Head);
    _LibVCD_Init_0_InitList(num_of_info);
    _LibVCD_Init_1_SaveToGlobal(outFileName, timescale, unit, info, num_of_info);
    _LibVCD_Init_2_DumpFileHeader();

    return 0;
}

int LibVCD_LA_Uninit(void)
{
    return LibVCD_Uninit();
}

int LibVCD_LA_ClockAdd(u32 clocksToAdd)
{
    MUTEX_LIB_VCD_LOCK;
    if (clocksToAdd) {
        //if ()
        LibVCD_DumpAllCurrValues();

        LibU64_AddU32(&gLibVCD_AccuClks, clocksToAdd);
        //LibUtil_AddInU64_TwoU32(&gLibVCD_ClkHigh, &gLibVCD_ClkLow, clocksToAdd);
    }
    MUTEX_LIB_VCD_UNLOCK;
    return 0;
}

void LibVCD_LA_Demo(void)
{
    printf("%s()\n", __func__);

    LibVCD_WireInfo_t test[3] = {
        {1, "ax", VALUE_IN_FOLLOWING, 1},
        {2, "bx", VALUE_IS_DONT_CARE, 0},
        {8, "cx", VALUE_IN_FOLLOWING, 0},
    };
    LibVCD_LA_Init("example.vcd", 1, TIME_UNIT_US, test, 3);

    LibVCD_ClockAdd(300);
    LibVCD_ValueChange(0, 0);
    LibVCD_ClockAdd(300);
    LibVCD_ValueChange(0, 1);
    LibVCD_ClockAdd(300);
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

