

#ifndef _UNI_VARIABLE_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

#define VAR_IS_SIGNED       BIT_31
#define VAR_IS_ARRAY        BIT_30
#define VAR_IS_C_STRING     BIT_29
#define VAR_IS_CPP_STRING   BIT_28

#define VAR_IS_8BITS    BIT_0
#define VAR_IS_16BITS   BIT_1
#define VAR_IS_32BITS   BIT_2
#define VAR_IS_64BITS   BIT_3
#define VAR_IS_128BITS  BIT_4
#define VAR_IS_256BITS  BIT_5

#define VAR_IS_UNINITED  (0)

#define VAR_C_STRING    (VAR_IS_C_STRING)
#define VAR_CPP_STRING  (VAR_IS_CPP_STRING)
#define VAR_U8          (VAR_IS_8BITS)
#define VAR_U16         (VAR_IS_16BITS)
#define VAR_U32         (VAR_IS_32BITS)
#define VAR_S8          (VAR_IS_SIGNED|VAR_IS_8BITS)
#define VAR_S16         (VAR_IS_SIGNED|VAR_IS_16BITS)
#define VAR_S32         (VAR_IS_SIGNED|VAR_IS_32BITS)
#define VAR_U8_ARRAY    (VAR_IS_ARRAY|VAR_IS_8BITS)
#define VAR_U16_ARRAY   (VAR_IS_ARRAY|VAR_IS_16BITS)
#define VAR_U32_ARRAY   (VAR_IS_ARRAY|VAR_IS_32BITS)
#define VAR_S8_ARRAY    (VAR_IS_ARRAY|VAR_IS_SIGNED|VAR_IS_8BITS)
#define VAR_S16_ARRAY   (VAR_IS_ARRAY|VAR_IS_SIGNED|VAR_IS_16BITS)
#define VAR_S32_ARRAY   (VAR_IS_ARRAY|VAR_IS_SIGNED|VAR_IS_32BITS)

/********************
string to array
array to string
dynamic array len
********************/
class UniVariable {
public:
    //std::string *p_name; //map index
    u32 type;
    //u32 retCode;
    u32 varLen; //The length of c string not includes /0 byte
    void* p_var;

    UniVariable(u32 inType = VAR_IS_UNINITED, const void *p_inVar = NULL, u32 inAryLen = 0/*only used in array type*/);
    UniVariable(u8 *in){ InitEx(in); };
    ~UniVariable(void);
    void Uninit(void);
    void Init(u32 inType = VAR_IS_UNINITED, const void *p_inVar = NULL, u32 inAryLen = 0/*only used in array type*/);
    void InitEx(u8 *in){ Init(VAR_U8, (void *)in); };
    void InitEx(u16 *in);
    
    u32 GetUnitInBytes(u32 inType);

    //TODO:
    //modify init() because var is might be std::string
    //var_dump()
    //map function or class?? (dictionary)
};

class DynamicArrayRecord {
public:
    u32 allocIncrement;
    u32 usedLenInBytes;
    u32 allocLenInBytes;
    void* p_ary; //will transfer to this->var
    class UniVariable *var;

    DynamicArrayRecord(u32 inAllocIncrement = 1000, class UniVariable *inVar = NULL){
        allocIncrement=inAllocIncrement;
        usedLenInBytes=0;
        allocLenInBytes = inAllocIncrement;
        p_ary = MM_ALLOC(allocIncrement);
        var = inVar;
    };
    ~DynamicArrayRecord(void){
        SAFE_FREE(p_ary);
    };
};

void UniVariable_Demo(void);

#define _UNI_VARIABLE_HPP_INCLUDED_
#endif//_UNI_VARIABLE_HPP_INCLUDED_



