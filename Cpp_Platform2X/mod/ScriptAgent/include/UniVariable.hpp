

#ifndef _UNI_VARIABLE_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

#define VAR_IS_SIGNED       BIT_31
#define VAR_IS_ARRAY        BIT_30
#define VAR_IS_C_STRING     BIT_29
#define VAR_IS_CPP_STRING   BIT_28
#define VAR_IS_VOID_POINTER BIT_27

#define VAR_IS_8BITS    BIT_0
#define VAR_IS_16BITS   BIT_1
#define VAR_IS_32BITS   BIT_2
#define VAR_IS_64BITS   BIT_3
#define VAR_IS_128BITS  BIT_4
#define VAR_IS_256BITS  BIT_5

#define VAR_IS_UNINITED  (0)

#define VAR_C_STRING        (VAR_IS_C_STRING)
#define VAR_CPP_STRING      (VAR_IS_CPP_STRING)
#define VAR_VOID_POINTER    (VAR_IS_VOID_POINTER)
#define VAR_U8              (VAR_IS_8BITS)
#define VAR_U16             (VAR_IS_16BITS)
#define VAR_U32             (VAR_IS_32BITS)
#define VAR_S8              (VAR_IS_SIGNED|VAR_IS_8BITS)
#define VAR_S16             (VAR_IS_SIGNED|VAR_IS_16BITS)
#define VAR_S32             (VAR_IS_SIGNED|VAR_IS_32BITS)
#define VAR_U8_ARRAY        (VAR_IS_ARRAY|VAR_IS_8BITS)
#define VAR_U16_ARRAY       (VAR_IS_ARRAY|VAR_IS_16BITS)
#define VAR_U32_ARRAY       (VAR_IS_ARRAY|VAR_IS_32BITS)
#define VAR_S8_ARRAY        (VAR_IS_ARRAY|VAR_IS_SIGNED|VAR_IS_8BITS)
#define VAR_S16_ARRAY       (VAR_IS_ARRAY|VAR_IS_SIGNED|VAR_IS_16BITS)
#define VAR_S32_ARRAY       (VAR_IS_ARRAY|VAR_IS_SIGNED|VAR_IS_32BITS)

typedef enum {
    UNI_VAR_DYNAMIC_ARRAY,
}UNI_VAR_FEATURE_ID_t;

//extended feature
typedef struct UniVar_Features_t {
    //struct UniVar_Features_t *next;
    UNI_VAR_FEATURE_ID_t id;
} UniVar_Features_t;

typedef struct UniVar_DynamicRestLen_t {
    struct UniVar_DynamicRestLen_t *next;
    u32 type;
    u32 startIndex;
    bool swap;
} UniVar_DynamicRestLen_t;

typedef struct {
    UniVar_Features_t hdr; //id = UNI_VAR_DYNAMIC_ARRAY
    //feature variable
    u32 allocIncrement;
    u32 usedLenInBytes;
    u32 allocLenInBytes;
    struct UniVar_DynamicRestLen_t *next;
} UniVar_DynamicArray_t;

u32 UniVar_GetUnitInBytes(u32 inType);
u32 UniVar_ConvertLen(u32 dstType, u32 srcType, u32 srcLen);

/********************
string to array
array to string
dynamic array len
********************/
class UniVariable {
public:
    u32 type;
    u32 varLen; //The length of c string not includes /0 byte
    void* p_var;
    UniVar_Features_t *feature; //one feature in the same time

    UniVariable(u32 inType = VAR_IS_UNINITED, const void *p_inVar = NULL, u32 inAryLen = 0/*only used in array type*/);
    UniVariable(const char *str){ _constructor(); Init(str); };
    UniVariable(char *str){ _constructor(); Init(str); };
    UniVariable(std::string *str){ _constructor(); Init(str); };
    UniVariable(std::string str){ _constructor(); Init(str); };
    UniVariable(void *in){ _constructor(); Init(in); };
    UniVariable(u8 in){ _constructor(); Init(in); };
    UniVariable(u16 in){ _constructor(); Init(in); };
    UniVariable(u32 in){ _constructor(); Init(in); };
    UniVariable(s8 in){ _constructor(); Init(in); };
    UniVariable(s16 in){ _constructor(); Init(in); };
    UniVariable(s32 in){ _constructor(); Init(in); };
    UniVariable(u8 *in){ _constructor(); Init(in); };
    UniVariable(u16 *in){ _constructor(); Init(in); };
    UniVariable(u32 *in){ _constructor(); Init(in); };
    UniVariable(s8 *in){ _constructor(); Init(in); };
    UniVariable(s16 *in){ _constructor(); Init(in); };
    UniVariable(s32 *in){ _constructor(); Init(in); };
    UniVariable(u8 *in, u32 inAryLen){ _constructor(); Init(in, inAryLen); };
    UniVariable(u16 *in, u32 inAryLen){ _constructor(); Init(in, inAryLen); };
    UniVariable(u32 *in, u32 inAryLen){ _constructor(); Init(in, inAryLen); };
    UniVariable(s8 *in, u32 inAryLen){ _constructor(); Init(in, inAryLen); };
    UniVariable(s16 *in, u32 inAryLen){ _constructor(); Init(in, inAryLen); };
    UniVariable(s32 *in, u32 inAryLen){ _constructor(); Init(in, inAryLen); };
    ~UniVariable(void);
    void _constructor(void){type=VAR_IS_UNINITED;p_var=NULL;varLen=0;feature=NULL;};
    void Uninit(void);
    void InitEx(u32 inType = VAR_IS_UNINITED, const void *p_inVar = NULL, u32 inAryLen = 0/*only used in array type*/);
    void Init(const char *str){ InitEx(VAR_IS_C_STRING, (const void *)str); };
    void Init(char *str){ InitEx(VAR_IS_C_STRING, (const void *)str); };
    void Init(std::string *str){ InitEx(VAR_IS_CPP_STRING, (const void *)str); };
    void Init(std::string str){ InitEx(VAR_IS_CPP_STRING, (const void *)&str); };
    void Init(void *in){ InitEx(VAR_VOID_POINTER, in); };
    void Init(u8 in){ InitEx(VAR_U8, (const void *)&in); };
    void Init(u16 in){ InitEx(VAR_U16, (const void *)&in); };
    void Init(u32 in){ InitEx(VAR_U32, (const void *)&in); };
    void Init(s8 in){ InitEx(VAR_S8, (const void *)&in); };
    void Init(s16 in){ InitEx(VAR_S16, (const void *)&in); };
    void Init(s32 in){ InitEx(VAR_S32, (const void *)&in); };
    void Init(u8 *in){ InitEx(VAR_U8, (const void *)in); };
    void Init(u16 *in){ InitEx(VAR_U16, (const void *)in); };
    void Init(u32 *in){ InitEx(VAR_U32, (const void *)in); };
    void Init(s8 *in){ InitEx(VAR_S8, (const void *)in); };
    void Init(s16 *in){ InitEx(VAR_S16, (const void *)in); };
    void Init(s32 *in){ InitEx(VAR_S32, (const void *)in); };
    void Init(u8 *in, u32 inAryLen){ InitEx(VAR_U8_ARRAY, (const void *)in, inAryLen); };
    void Init(u16 *in, u32 inAryLen){ InitEx(VAR_U16_ARRAY, (const void *)in, inAryLen); };
    void Init(u32 *in, u32 inAryLen){ InitEx(VAR_U32_ARRAY, (const void *)in, inAryLen); };
    void Init(s8 *in, u32 inAryLen){ InitEx(VAR_S8_ARRAY, (const void *)in, inAryLen); };
    void Init(s16 *in, u32 inAryLen){ InitEx(VAR_S16_ARRAY, (const void *)in, inAryLen); };
    void Init(s32 *in, u32 inAryLen){ InitEx(VAR_S32_ARRAY, (const void *)in, inAryLen); };

    int Convert(UniVariable *from);
    int Copy(UniVariable *from);

    void dump(void);
    void dumpFeatures(void);

    void DynamicArrayInit(u8 in, u32 inAllocIncrement = 1000){ DynamicArrayInitEx(VAR_U8_ARRAY, inAllocIncrement); };
    void DynamicArrayInit(u16 in, u32 inAllocIncrement = 1000){ DynamicArrayInitEx(VAR_U16_ARRAY, inAllocIncrement); };
    void DynamicArrayInit(u32 in, u32 inAllocIncrement = 1000){ DynamicArrayInitEx(VAR_U32_ARRAY, inAllocIncrement); };
    void DynamicArrayInit(s8 in, u32 inAllocIncrement = 1000){ DynamicArrayInitEx(VAR_S8_ARRAY, inAllocIncrement); };
    void DynamicArrayInit(s16 in, u32 inAllocIncrement = 1000){ DynamicArrayInitEx(VAR_S16_ARRAY, inAllocIncrement); };
    void DynamicArrayInit(s32 in, u32 inAllocIncrement = 1000){ DynamicArrayInitEx(VAR_S32_ARRAY, inAllocIncrement); };
    void DynamicArrayInit(u8 *in, u32 inAllocIncrement = 1000){ DynamicArrayInitEx(VAR_U8_ARRAY, inAllocIncrement); };
    void DynamicArrayInit(u16 *in, u32 inAllocIncrement = 1000){ DynamicArrayInitEx(VAR_U16_ARRAY, inAllocIncrement); };
    void DynamicArrayInit(u32 *in, u32 inAllocIncrement = 1000){ DynamicArrayInitEx(VAR_U32_ARRAY, inAllocIncrement); };
    void DynamicArrayInit(s8 *in, u32 inAllocIncrement = 1000){ DynamicArrayInitEx(VAR_S8_ARRAY, inAllocIncrement); };
    void DynamicArrayInit(s16 *in, u32 inAllocIncrement = 1000){ DynamicArrayInitEx(VAR_S16_ARRAY, inAllocIncrement); };
    void DynamicArrayInit(s32 *in, u32 inAllocIncrement = 1000){ DynamicArrayInitEx(VAR_S32_ARRAY, inAllocIncrement); };
    void DynamicArrayInitEx(u32 inType, u32 inAllocIncrement = 1000);
    void DynamicArrayWrite(void *in, u32 inAryLen, u32 startIndex);
    void DynamicArrayPushBack(void *in, u32 inAryLen) { DynamicArrayWrite(in, inAryLen, varLen); } ;
    void DynamicArrayPushBack(u8 in, bool swap = false) { DynamicArrayPushBack((void *)&in, UniVar_ConvertLen(type, VAR_U8, 1)); };
    void DynamicArrayPushBack(u16 in, bool swap = false) { u16 x; LibUtil_IntSwapCopy((u8 *)&x, (u8 *)&in, 2, swap); DynamicArrayPushBack((void *)&x, UniVar_ConvertLen(type, VAR_U16, 1)); };
    void DynamicArrayPushBack(u32 in, bool swap = false) { u32 x; LibUtil_IntSwapCopy((u8 *)&x, (u8 *)&in, 4, swap); DynamicArrayPushBack((void *)&x, UniVar_ConvertLen(type, VAR_U32, 1)); };
    void DynamicArrayPushBack(s8 in, bool swap = false) { DynamicArrayPushBack((void *)&in, UniVar_ConvertLen(type, VAR_S8, 1)); };
    void DynamicArrayPushBack(s16 in, bool swap = false) { s16 x; LibUtil_IntSwapCopy((u8 *)&x, (u8 *)&in, 2, swap); DynamicArrayPushBack((void *)&x, UniVar_ConvertLen(type, VAR_S16, 1)); };
    void DynamicArrayPushBack(s32 in, bool swap = false) { s32 x; LibUtil_IntSwapCopy((u8 *)&x, (u8 *)&in, 4, swap); DynamicArrayPushBack((void *)&x, UniVar_ConvertLen(type, VAR_S32, 1)); };
    void DynamicArrayFlush(void);
    void DynamicArrayRestLenSet(u32 inType, bool inSwap = false);
    void DynamicArrayRestLenApply(void);
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

u32 GetUniStrType(char *in);
u32 GetUniStrType(const char *in);
u32 GetUniStrType(std::string in);
u32 GetUniStrType(std::string *in);
u32 GetUniAryType(u8 *in);
u32 GetUniAryType(u16 *in);
u32 GetUniAryType(u32 *in);
u32 GetUniAryType(s8 *in);
u32 GetUniAryType(s16 *in);
u32 GetUniAryType(s32 *in);
u32 GetUniType(void *in);
u32 GetUniType(u8 in);
u32 GetUniType(u16 in);
u32 GetUniType(u32 in);
u32 GetUniType(s8 in);
u32 GetUniType(s16 in);
u32 GetUniType(s32 in);
u32 GetUniType(u8 *in);
u32 GetUniType(u16 *in);
u32 GetUniType(u32 *in);
u32 GetUniType(s8 *in);
u32 GetUniType(s16 *in);
u32 GetUniType(s32 *in);
u32 GetUniType(u8 *in, u32 inAryLen);
u32 GetUniType(u16 *in, u32 inAryLen);
u32 GetUniType(u32 *in, u32 inAryLen);
u32 GetUniType(s8 *in, u32 inAryLen);
u32 GetUniType(s16 *in, u32 inAryLen);
u32 GetUniType(s32 *in, u32 inAryLen);
const char *GetUniTypeString(u32 type);


void UniVariable_Demo(void);

#define _UNI_VARIABLE_HPP_INCLUDED_
#endif//_UNI_VARIABLE_HPP_INCLUDED_



