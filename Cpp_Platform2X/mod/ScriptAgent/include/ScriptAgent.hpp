

#ifndef _SCRIPT_AGENT_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

#include "SA_Channel.hpp"
#include "SA_Variable.hpp"
#include "SA_UniFormat.hpp"
#include "SA_SwLib.hpp"
#include "SA_String.hpp"
#include "SA_Content.hpp"

#include "UniVariable.hpp"
#include "UniVar.hpp"
#include "TextVar.hpp"
#include "TextCall.hpp"
#include "TextCall_Basic.hpp"
#include "TextCall_Uart.hpp"


class ScriptAgent: virtual public Tracer {
public:
    class LibFileIoClass *file;

    ScriptAgent(int disableTracer = 0);
    ~ScriptAgent();
};



int ScriptAgent_Demo(void);


#define _SCRIPT_AGENT_HPP_INCLUDED_
#endif//_SCRIPT_AGENT_HPP_INCLUDED_



