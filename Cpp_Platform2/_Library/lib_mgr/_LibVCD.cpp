
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
//LibFileIoClass gLibVCD_OutFile = LibFileIoClass("12.txt", "w+b");
FILE *gLibVCD_fp = NULL;
u32 gLibVCD_ClkLow = 0;
u32 gLibVCD_ClkHigh = 0;
Lib_Desc_Head_t *gLibVCD_JobHead;
int gLibVCD_IsValueNotPrintYet = 0;

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

int LibVCD_Init(const char *outFileName, u32 timescale, TIME_UNIT_t unit, LibVCD_WireInfo_t *info, u32 num_of_info)
{
    const char *unitStr;
    BASIC_ASSERT(num_of_info <= 0x7E - 0x21 + 1);

    //store all info
    gLibVCD_TimeScale = timescale;
    gLibVCD_TimeUnit = unit;
    gLibVCD_InfoArry = (_LibVCD_WireInfo_Internal_t *)MEM_ALLOC(sizeof(_LibVCD_WireInfo_Internal_t) * num_of_info);
    FOR_I(num_of_info) {
        WT(gLibVCD_InfoArry[i].ori.num_of_bits , info[i].num_of_bits);
        WT(gLibVCD_InfoArry[i].ori.wire_name , info[i].wire_name);
        WT(gLibVCD_InfoArry[i].ch_symbol , 0x21 + i);
    }
    //write init data to file
    gLibVCD_fp = fopen(outFileName, "w+b");
    BASIC_ASSERT(gLibVCD_fp != NULL);
    switch (unit) {
        case TIME_UNIT_PS: unitStr = "ps"; break;
        case TIME_UNIT_NS: unitStr = "ns"; break;
        case TIME_UNIT_US: unitStr = "us"; break;
        case TIME_UNIT_S:  unitStr = "s"; break;
        default: BASIC_ASSERT(0); break;
    }
    fprintf(gLibVCD_fp, "$timescale\n");
    fprintf(gLibVCD_fp, "    %d%s\n", timescale, unitStr);
    fprintf(gLibVCD_fp, "$end\n\n");

    fprintf(gLibVCD_fp, "$scope module M1 $end\n");
    FOR_I(num_of_info) {
        fprintf(gLibVCD_fp,
            "$var wire %d %c %s $end\n",
            gLibVCD_InfoArry[i].ori.num_of_bits,
            gLibVCD_InfoArry[i].ch_symbol,
            gLibVCD_InfoArry[i].ori.wire_name);
    }
    fprintf(gLibVCD_fp, "$upscope $end\n\n");

    fprintf(gLibVCD_fp, "#0\n");
    FOR_I(num_of_info) {
        fprintf(gLibVCD_fp,
            "%s%c\n",
            _LibVCD_StringOfValue(info[i].num_of_bits, info[i].isInitValueDontCare, info[i].initValue),
            gLibVCD_InfoArry[i].ch_symbol);
    }
    fprintf(gLibVCD_fp, "\n");

    //wait for event, and store it
    //if time move on, write all stored events to file
    gLibVCD_JobHead = LibDesc_CreateList(sizeof(Lib_Desc_Head_t), num_of_info, sizeof(_LibVCD_WireValueDesc_t));

    //wait for uninit

    return 0;
}

int LibVCD_Uninit(void)
{
    BASIC_ASSERT(gLibVCD_ClkHigh == 0); //not implement u64 print
    fprintf(gLibVCD_fp, "#%d\n", gLibVCD_ClkLow);

    if (gLibVCD_InfoArry != NULL) {
        MEM_FREE(gLibVCD_InfoArry);
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

        BASIC_ASSERT(gLibVCD_ClkHigh == 0); //not implement u64 print
        fprintf(gLibVCD_fp, "#%d\n", gLibVCD_ClkLow);
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
                                              prev_desc->data.isValueDontCare,
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
                MEM_FREE(prev_desc);
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

        LibUtil_AddInU64_TwoU32(&gLibVCD_ClkHigh, &gLibVCD_ClkLow, clocksToAdd);
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
    desc->data.isValueDontCare = 1;
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
    desc->data.isValueDontCare = 0;
    desc->data.value = value;
    gLibVCD_IsValueNotPrintYet = 1;
    MUTEX_LIB_VCD_UNLOCK;
    return 0;
}

void LibVCD_Demo(void)
{
    printf("%s()\n", __func__);

    LibVCD_WireInfo_t test[3] = {
        {1, "ax", VALUE_IS_DONT_CARE, 0},
        {2, "bx", VALUE_IS_DONT_CARE, 0},
        {8, "cx", VALUE_IN_FOLLOWING, 0},
    };
    LibVCD_Init("example.vcd", 1, TIME_UNIT_US, test, 3);
    LibVCD_ClockAdd(500);
    LibVCD_ValueChangeToDontCare(2);
    LibVCD_ClockAdd(100);
    LibVCD_ValueChange(2, 255);
    LibVCD_ValueChange(1, 2);
    LibVCD_ClockAdd(100);
    LibVCD_Uninit();
}