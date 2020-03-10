

#ifndef __LIB_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================
#define IS_MT LibIsMT()

typedef enum {
    LIB_NONE_ENABLE = 0,
    LIB_MT_ENABLE   = BIT_0,
} LIB_FLAG_t;

int Lib_Init(LIB_FLAG_t flags = LIB_NONE_ENABLE);
int Lib_Uninit(void);
int Lib_IsMT(void);

class LibInit{
public:
    int is_verbose;

    LibInit(LIB_FLAG_t flags = LIB_NONE_ENABLE, int isVerbose = 0);
    ~LibInit(void);
};

#define __LIB_HPP_INCLUDED_
#endif//__LIB_HPP_INCLUDED_

