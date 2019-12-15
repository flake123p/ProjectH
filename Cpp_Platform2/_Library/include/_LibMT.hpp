

#ifndef __LIB_MT_HPP_INCLUDED_

#include "My_Types.h"
#include "_LibLinkedList.hpp" //LinkedListClass

#include "LibThread.hpp"

// ============================== Debug ==============================

// ============================== Define ==============================
typedef struct {
    //int initiated;
    MUTEX_HANDLE_t handle;
} LibMT_UtilMutex_t; //init with = {0, NULL};

//destroy in LibMT_Uninit();
int LibMT_UtilMutex_Init(LibMT_UtilMutex_t *mutex);
int LibMT_UtilMutex_Uninit(LibMT_UtilMutex_t *mutex);
int LibMT_UtilMutex_Lock(LibMT_UtilMutex_t *mutex);
int LibMT_UtilMutex_Unlock(LibMT_UtilMutex_t *mutex);
void LibMT_UtilMutex_Demo(void);

int LibMT_Init(void);
int LibMT_Uninit(void);

#define __LIB_MT_HPP_INCLUDED_
#endif//__LIB_MT_HPP_INCLUDED_

