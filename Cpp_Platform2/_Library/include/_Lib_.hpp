

#ifndef __LIB_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================

typedef enum {
    ENABLE_NONE    = 0,
    ENABLE_LIB_MT = BIT_0,
} LIB_FLAG_t;

int Lib_Init(LIB_FLAG_t initFlags);
int Lib_Uninit(void);

#define __LIB_HPP_INCLUDED_
#endif//__LIB_HPP_INCLUDED_

