

#ifndef _LIB_COMM_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================
typedef enum {
    DO_NOT_COMPARE = 0,
    DO_COMPARE     = 1,
}COMPARE_FLAG_t;

// ============================== Library: Cross-Platform (Manager) ==============================
// ruleArray: H byte: flag, L byte: compare value
int LibComm_ArrayCompare(u16 *ruleArray, u32 ruleLen, u8 *dataForCompare, u32 dataLen, int print_log = 1);

// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================

void LibComm_Demo(void);

#define _LIB_COMM_HPP_INCLUDED_
#endif//_LIB_COMM_HPP_INCLUDED_

