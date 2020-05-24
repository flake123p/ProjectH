

#ifndef _SCRIPT_AGENT_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

#include "SA_Channel.hpp"
#include "SA_Variable.hpp"
#include "SA_UniFormat.hpp"
#include "SA_SwLib.hpp"
#include "SA_String.hpp"
#include "SA_Content.hpp"

class basicClass {
private:
    int not_uninit_yet;
public:
    basicClass(void){not_uninit_yet=1;};
    ~basicClass(void){if(not_uninit_yet)printf("object leak\n");};
    void uninit(void){not_uninit_yet=0;};
};
class testx {
private:
    int _isDynamic;
    const char *_log;
    void *_ptr;
public:
    testx(const char *log = "class testx", int isDynamic = 0)
    {
        _isDynamic = isDynamic;
        _log = log;
        printf("testx created\n");
        _ptr=MM_ALLOC2(1,_log);
    };
    ~testx(void)
    {
        printf("testx destroyed\n");
        MM_FREE(_ptr);
    };
};
class testx2 {
public:
    testx2(void){printf("testx2 created\n");};
    ~testx2(void){printf("testx2 destroyed\n");};
};
class ScriptAgent : public basicClass {
public:
    //testx tt;
    //basicClass *t;
    ScriptAgent(const char *scriptFileName = NULL)
    {
        printf("ScriptAgent created\n");
        //this->uninit();
        //t=new(basicClass);
    };
    ~ScriptAgent(void){printf("ScriptAgent destroyed\n");};
};

int ScriptAgent_Demo(void);


#define _SCRIPT_AGENT_HPP_INCLUDED_
#endif//_SCRIPT_AGENT_HPP_INCLUDED_



