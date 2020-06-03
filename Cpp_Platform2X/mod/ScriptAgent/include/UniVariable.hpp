

#ifndef _UNI_VARIABLE_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

#define VAR_IS_SIGNED   BIT_31
#define VAR_IS_ARRAY    BIT_30
#define VAR_IS_STRING   BIT_29

#define VAR_IS_8BITS    BIT_0
#define VAR_IS_16BITS   BIT_1
#define VAR_IS_32BITS   BIT_2
#define VAR_IS_64BITS   BIT_3
#define VAR_IS_128BITS  BIT_4
#define VAR_IS_256BITS  BIT_5

#define VAR_IS_UNINITED  (0)

class UniVariable {
public:
    std::string *p_name; //map index
    u32 type;
    u32 retCode;
    u32 varLen;
    void* p_var;

    UniVariable(std::string *p_inName = NULL, u32 inType = VAR_IS_UNINITED, u32 inVarLen = 0, void *p_inVar = NULL);
    ~UniVariable(void);
    void Init(std::string *p_inName = NULL, u32 inType = VAR_IS_UNINITED, u32 inVarLen = 0, void *p_inVar = NULL);
    //
    void Init(const char *p_inName = NULL, u32 inType = VAR_IS_UNINITED, u32 inVarLen = 0, void *p_inVar = NULL);
    //TODO:
    //modify init() because var is might be std::string
    //var_dump()
    //map function or class?? (dictionary)
};

void UniVariable_Demo(void);

#define _UNI_VARIABLE_HPP_INCLUDED_
#endif//_UNI_VARIABLE_HPP_INCLUDED_



