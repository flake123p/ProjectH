
#ifndef __LIB_VCD_HPP_INCLUDED_

#include "My_Types.h"
#include "_LibLinkedList.hpp"

// ============================== Debug ==============================

// ============================== Define ==============================
typedef enum {
    TIME_UNIT_PS,
    TIME_UNIT_NS,
    TIME_UNIT_US,
    TIME_UNIT_S,
}Time_Unit_t;

typedef struct {
    u32 num_of_bits;
    const char *wire_name;
} LIB_VCD_WIRE_INFO_t;

void LibVCD_Demo(void);

#define __LIB_VCD_HPP_INCLUDED_
#endif//__LIB_VCD_HPP_INCLUDED_

