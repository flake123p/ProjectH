

#ifndef _UNI_VAR_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"
#include "UniVariable.hpp"

class UniVar {
public:
    u32 type; //VAR_IS_UNINITED ...
    u32 varLen; //The length of c string includes /0 byte
    void* pVar;
    u32 allocLenInBytes;
    u32 allocIncrement;
};

void UniVar_Demo(void);

#define _UNI_VAR_HPP_INCLUDED_
#endif//_UNI_VAR_HPP_INCLUDED_



