

#ifndef _SIM_ENGINE_HPP_INCLUDED_


#include "SimTime.hpp"
#include "SimModule.hpp"
#include "SimTime3.hpp"
#include "SimCh.hpp"

typedef struct {
    class SimTime3 *timeDb;
} SimEngine_Desc;

int SimEngine_Demo(void);



#define _SIM_ENGINE_HPP_INCLUDED_
#endif//_SIM_ENGINE_HPP_INCLUDED_

