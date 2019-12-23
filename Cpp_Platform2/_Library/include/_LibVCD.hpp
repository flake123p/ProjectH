
#ifndef __LIB_VCD_HPP_INCLUDED_

#include "My_Types.h"
#include "_LibLinkedList.hpp"

// ============================== Debug ==============================

// ============================== Define ==============================
#define VALUE_IS_DONT_CARE ( 1 )
#define VALUE_IN_FOLLOWING ( 0 )

typedef enum {
    TIME_UNIT_PS,
    TIME_UNIT_NS,
    TIME_UNIT_US,
    TIME_UNIT_S,
}TIME_UNIT_t;

typedef struct {
    u32 num_of_bits;
    const char *wire_name;
    int isInitValueDontCare;
    u32 initValue;
} LibVCD_WireInfo_t;

typedef struct {
    u32 index;
    int isValueDontCare;
    u32 value;
} LibVcd_WireValue_t;

int LibVCD_Init(const char *outFileName, u32 timescale, TIME_UNIT_t unit, LibVCD_WireInfo_t *info, u32 num_of_info);
int LibVCD_Uninit(void);
int LibVCD_DumpAllCurrValues(void);
int LibVCD_ClockAdd(u32 clocksToAdd);
int LibVCD_ValueChangeToDontCare(u32 index);
int LibVCD_ValueChange(u32 index, u32 value);

void LibVCD_Demo(void);

#define __LIB_VCD_HPP_INCLUDED_
#endif//__LIB_VCD_HPP_INCLUDED_

