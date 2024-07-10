

#ifndef _EVERYTHING_LIB_MGR_HPP_INCLUDED_

#if 0
//#include "stdafx.h" //For porting to "Visual Studio"

// ====== Standard C/Cpp Library ======
#include <stdio.h>  // printf, scanf, NULL
#include <string.h>
#include <stdint.h> // uint32_t, ...
#include <stdlib.h> // malloc, free, rand
#include <stdarg.h>

// ====== Basics ======
#include "My_Basics.hpp"

// ====== Platform Library ======
/*
	Library writing guide:
		1.) Create Demo()
		2.) Create Dump()
*/
#else
#include "Everything_Lib_Mgr_Lite.hpp"
#endif

#include "_Lib_.hpp"
#include "_LibMT.hpp"
#include "_LibMem.hpp"
#include "_LibMemory.hpp"
#include "_LibMenu.hpp"
#include "_LibString.hpp"
#include "_Lib51Hex.hpp"
#include "_LibError.hpp"
#include "_LibSyntex.hpp"
#include "_LibDesc.hpp"
#include "_LibModule.hpp"
#include "_LibTimer.hpp"
#include "_LibVCD.hpp"
#include "LibUart.hpp"
#include "LibTime.hpp"
#include "LibFileIO.hpp"
#include "LibFileIO2.hpp"
#include "LibUtility.hpp"
#include "LibU64.hpp"
#include "LibOs.hpp"
#include "LibTui.hpp"
#include "LibSysCmd.hpp"
#include "LibScreenLogic.hpp"
#include "LibSharedMemory.hpp"
#include "LibEvent.hpp"
#include "LibBuffer.hpp"
#include "LibThread.hpp"
#include "LibBT_HCI.hpp"
#include "LibArgParser.hpp"
#include "LibSimpleFS.hpp"
#include "LibEncrypt.hpp"
#include "LibPacket.hpp"
#include "LibMsg.hpp"
#include "LibComm.hpp"
#include "LibTDD.hpp"
#include "LibArray.hpp"
#include "LibList.hpp"



//
// Objects Tracer Class
//
#define NULLIFY_TRACER_FOR_SPEED_UP ( 0 )
//constructor arument
#define DISABLE_TRACER (1)
class Tracer {
public:
#if NULLIFY_TRACER_FOR_SPEED_UP
    Tracer(const char *from, int disableTracer){};
    ~Tracer(){};
#else
    const char *_from;
    void *ptr;
    Tracer(const char *from=NULL, int disableTracer=0)
    {
        _from=from;
        if (disableTracer) {
            ptr=NULL;
        } else {
            BASIC_ASSERT(_from!=NULL);
            ptr=MM_ALLOC2(4,_from);
        }
    };
    ~Tracer(){if(ptr!=NULL)MM_FREE(ptr);};
#endif
};



#define _EVERYTHING_LIB_MGR_HPP_INCLUDED_
#endif//_EVERYTHING_LIB_MGR_HPP_INCLUDED_




