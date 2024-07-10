
#include "Everything_ThisMod.hpp"

#if 1
#define UNI_VAR_BASIC_ASSERT BASIC_ASSERT
#else
#define UNI_VAR_BASIC_ASSERT()
#endif

UniVariable::UniVariable(u32 inType /*= VAR_IS_UNINITED*/, const void *p_inVar /*= NULL*/, u32 inAryLen /*= 0 only used in array type*/)
{
    _constructor();
    InitEx(inType, p_inVar, inAryLen);
}

UniVariable::~UniVariable(void)
{
    Uninit();
}

void UniVariable::Uninit(void)
{
    if (type == VAR_IS_UNINITED) {
        return;
    }
#if 0
    UniVar_Features_t *oldFeature;
    UniVar_Features_t *currFeature = feature;

    while (1) {
        if (currFeature == NULL)
            break;

        oldFeature = currFeature;
        currFeature = currFeature->next;
        switch (oldFeature->id) {
            case UNI_VAR_DYNAMIC_ARRAY: SAFE_FREE(oldFeature); break;
            default:
                BASIC_ASSERT(0);
                break;
        }
    }
#else
    if (feature != NULL)
    {
        switch (feature->id) {
            case UNI_VAR_DYNAMIC_ARRAY: {
                UniVar_DynamicArray_t *dynamicArray = (UniVar_DynamicArray_t *)feature;
                UniVar_DynamicRestLen_t *currRestLen;
                UniVar_DynamicRestLen_t *oldRestLen;
                for (currRestLen = dynamicArray->next; currRestLen!=NULL; ) {
                    oldRestLen = currRestLen;
                    currRestLen = currRestLen->next;
                    SAFE_FREE(oldRestLen);
                }
                SAFE_FREE2(feature);
            } break;
            default:
                BASIC_ASSERT(0);
                break;
        }
    }
#endif

    if (type & VAR_IS_CPP_STRING) {
        SAFE_DELETE((std::string *)p_var);
    } else {
        SAFE_FREE(p_var);
    }
    type = VAR_IS_UNINITED;
    p_var = NULL;
    varLen = 0;
}

void UniVariable::InitEx(u32 inType /*= VAR_IS_UNINITED*/, const void *p_inVar /*= NULL*/, u32 inAryLen /*= 0 only used in array type*/)
{
    u32 unitInBytes = UniVar_GetUnitInBytes(inType);

    UNI_VAR_BASIC_ASSERT(type == VAR_IS_UNINITED);
    type = inType;

    if(type & VAR_IS_ARRAY) {
        UNI_VAR_BASIC_ASSERT((type & (VAR_IS_C_STRING|VAR_IS_CPP_STRING)) == 0);
        varLen = inAryLen;
        if (inAryLen) {
            UNI_VAR_BASIC_ASSERT(unitInBytes != 0);
            UNI_VAR_BASIC_ASSERT(p_inVar != NULL);
            p_var = MM_ALLOC(inAryLen * unitInBytes);
            MM_CPY(p_var, p_inVar, inAryLen * unitInBytes);
        } else {
            p_var = NULL;
        }
    } else if(type & VAR_IS_C_STRING) {
        UNI_VAR_BASIC_ASSERT((type & (VAR_IS_ARRAY|VAR_IS_CPP_STRING)) == 0);
        UNI_VAR_BASIC_ASSERT(p_inVar != NULL);
        varLen = strlen((const char *)p_inVar);
        p_var = MM_ALLOC(varLen+1);
        MM_CPY(p_var, p_inVar, varLen+1);
    } else if(type & VAR_IS_CPP_STRING) {
        UNI_VAR_BASIC_ASSERT((type & (VAR_IS_C_STRING|VAR_IS_ARRAY)) == 0);
        UNI_VAR_BASIC_ASSERT(p_inVar != NULL);
        std::string *dstStr = new(std::string);
        std::string *srcStr = (std::string *)p_inVar;
        *dstStr = *srcStr;
        p_var = (void *)dstStr;
        varLen = strlen((const char *)dstStr->c_str());
    } else if(type & VAR_IS_VOID_POINTER) {
        UNI_VAR_BASIC_ASSERT((type & (VAR_IS_ARRAY|VAR_IS_C_STRING|VAR_IS_CPP_STRING)) == 0);
        //single variable
        varLen = unitInBytes;
        p_var = MM_ALLOC(unitInBytes);
        if (p_inVar != NULL) {
            // it's &p_inVar, not p_inVar
            MM_CPY(p_var, &p_inVar, unitInBytes);
        }
    } else if(type != VAR_IS_UNINITED) {
        UNI_VAR_BASIC_ASSERT(p_inVar != NULL);
        UNI_VAR_BASIC_ASSERT((type & (VAR_IS_ARRAY|VAR_IS_C_STRING|VAR_IS_CPP_STRING)) == 0);
        //single variable
        varLen = unitInBytes;
        p_var = MM_ALLOC(unitInBytes);
        if (p_inVar != NULL) {
            MM_CPY(p_var, p_inVar, unitInBytes);
        }
    } else {
        BASIC_ASSERT(type == VAR_IS_UNINITED);
    }
}

int UniVariable::Convert(UniVariable *from)
{
/*
    switch (type)
    {
        case VAR_C_STRING  :   return "VAR_C_STRING    ";
        case VAR_CPP_STRING:   return "VAR_CPP_STRING  ";
        case VAR_VOID_POINTER: return "VAR_VOID_POINTER";
        case VAR_U8        :   return "VAR_U8          ";
        case VAR_U16       :   return "VAR_U16         ";
        case VAR_U32       :   return "VAR_U32         ";
        case VAR_S8        :   return "VAR_S8          ";
        case VAR_S16       :   return "VAR_S16         ";
        case VAR_S32       :   return "VAR_S32         ";
        case VAR_U8_ARRAY  :   return "VAR_U8_ARRAY    ";
        case VAR_U16_ARRAY :   return "VAR_U16_ARRAY   ";
        case VAR_U32_ARRAY :   return "VAR_U32_ARRAY   ";
        case VAR_S8_ARRAY  :   return "VAR_S8_ARRAY    ";
        case VAR_S16_ARRAY :   return "VAR_S16_ARRAY   ";
        case VAR_S32_ARRAY :   return "VAR_S32_ARRAY   ";
        default:
            return "Unknown Type";
    }
*/
return 0;
}

int UniVariable::Copy(UniVariable *from)
{
    Uninit();
    if(from->type == VAR_IS_VOID_POINTER) {
        void **ptr = (void **)from->p_var;
        InitEx(from->type, *ptr, from->varLen);
    } else {
        InitEx(from->type, from->p_var, from->varLen);
    }
    return 0;
}

void UniVariable::dump(void)
{
#define MAX_ELEMENT_IN_ONE_LINE_8  (16)
#define MAX_ELEMENT_IN_ONE_LINE_16 (8)
#define MAX_ELEMENT_IN_ONE_LINE_32 (8)
    printf("===>>> type = 0x%08X : %s\n", type, GetUniTypeString(type));


    if(type & VAR_IS_ARRAY) {
        printf("var =\n");
        u32 i;
        if (type & VAR_IS_8BITS) {
            u8 *ary = (u8 *)p_var;
            for (i=0; i<varLen; i++) {
                printf("0x%02X ", ary[i]);
                if ((i % MAX_ELEMENT_IN_ONE_LINE_8) == (MAX_ELEMENT_IN_ONE_LINE_8-1)) {
                    printf("\n");
                }
            }
            if (((i-1) % MAX_ELEMENT_IN_ONE_LINE_8) != (MAX_ELEMENT_IN_ONE_LINE_8-1)) {
                printf("\n");
            }
        } else if (type & VAR_IS_16BITS) {
            u16 *ary = (u16 *)p_var;
            for (i=0; i<varLen; i++) {
                printf("0x%04X ", ary[i]);
                if ((i % (MAX_ELEMENT_IN_ONE_LINE_16)) == (MAX_ELEMENT_IN_ONE_LINE_16-1)) {
                    printf("\n");
                }
            }
            if (((i-1) % (MAX_ELEMENT_IN_ONE_LINE_16)) != (MAX_ELEMENT_IN_ONE_LINE_16-1)) {
                printf("\n");
            }
        } else if (type & VAR_IS_32BITS) {
            u32 *ary = (u32 *)p_var;
            for (i=0; i<varLen; i++) {
                printf("0x%08X ", ary[i]);
                if ((i % (MAX_ELEMENT_IN_ONE_LINE_32)) == (MAX_ELEMENT_IN_ONE_LINE_32-1)) {
                    printf("\n");
                }
            }
            if (((i-1) % (MAX_ELEMENT_IN_ONE_LINE_32)) != (MAX_ELEMENT_IN_ONE_LINE_32-1)) {
                printf("\n");
            }
        } else {
            BASIC_ASSERT(0);
        }
    } else if(type & VAR_IS_C_STRING) {
        printf("var = %s\n", (const char *)p_var);
    } else if(type & VAR_IS_CPP_STRING) {
        printf("var = %s\n", ((std::string *)p_var)->c_str());
    } else if(type & VAR_IS_VOID_POINTER) {
        u8 **ptr = (u8 **)p_var;
        printf("var = %p\n", (*ptr));
    } else if(type != VAR_IS_UNINITED) {
        u32 temp;
        if (type & VAR_IS_8BITS) {
            u8 *ptr = (u8 *)p_var;
            temp = *ptr;
        } else if (type & VAR_IS_16BITS) {
            u16 *ptr = (u16 *)p_var;
            temp = *ptr;
        } else if (type & VAR_IS_32BITS) {
            u32 *ptr = (u32 *)p_var;
            temp = *ptr;
        } else {
            BASIC_ASSERT(0);
        }
        if (type & VAR_IS_SIGNED) {
            printf("var = %d (0x%02X)\n", (s32)temp, (unsigned int)temp);
        } else {
            printf("var = %u (0x%02X)\n", temp, (unsigned int)temp);
        }
    }

    printf("varLen = %d\n", varLen);

    dumpFeatures();
}

void UniVariable::dumpFeatures(void)
{
#if 0
    UniVar_Features_t *currFeature = feature;
    for (currFeature = feature; currFeature != NULL; currFeature = currFeature->next) {
        switch (currFeature->id) {
            case UNI_VAR_DYNAMIC_ARRAY: {
                UniVar_DynamicArray_t *dynamicArray = (UniVar_DynamicArray_t*)currFeature;
                DUMPND(dynamicArray->allocIncrement);
                DUMPND(dynamicArray->usedLenInBytes);
                DUMPND(dynamicArray->allocLenInBytes);
            } break;
            default:
                BASIC_ASSERT(0);
                break;
        }
    }
#endif
    if (feature != NULL)
    {
        switch (feature->id) {
            case UNI_VAR_DYNAMIC_ARRAY: {
                UniVar_DynamicArray_t *dynamicArray = (UniVar_DynamicArray_t*)feature;
                DUMPND(dynamicArray->allocIncrement);
                DUMPND(dynamicArray->usedLenInBytes);
                DUMPND(dynamicArray->allocLenInBytes);
                {
                    UniVar_DynamicRestLen_t *currRestLen;
                    int i = 0;
                    for (currRestLen = dynamicArray->next; currRestLen!=NULL; currRestLen = currRestLen->next) {
                        printf("[%d] unitInBytes=%u, startIndex=%u, doSwap=%d\n", i, UniVar_GetUnitInBytes(currRestLen->type), currRestLen->startIndex, currRestLen->swap);
                        i++;
                    }
                }
            } break;
            default:
                BASIC_ASSERT(0);
                break;
        }
    }
}

void UniVariable::DynamicArrayInitEx(u32 inType, u32 inAllocIncrement /*= 1000*/)
{
    UniVar_DynamicArray_t *dynamicArray;

    UNI_VAR_BASIC_ASSERT(feature == NULL);
    UNI_VAR_BASIC_ASSERT(type == VAR_IS_UNINITED);
    switch (inType) {
        case VAR_U8_ARRAY:
        case VAR_U16_ARRAY:
        case VAR_U32_ARRAY:
        case VAR_S8_ARRAY:
        case VAR_S16_ARRAY:
        case VAR_S32_ARRAY:
            break;
        default:
            BASIC_ASSERT(0);
            break;
    }
    type = inType;
/*
    do {
        if(*p_currFeature == NULL) {
            break;
        }
        if ((*p_currFeature)->id == UNI_VAR_DYNAMIC_ARRAY) {
            //already inited
            BASIC_ASSERT(0);
        }
        p_currFeature = &(feature->next);
    } while (0);
*/
    dynamicArray = (UniVar_DynamicArray_t *)MM_ALLOC(sizeof(UniVar_DynamicArray_t));
    //dynamicArray->hdr.next = NULL;
    dynamicArray->hdr.id = UNI_VAR_DYNAMIC_ARRAY;
    dynamicArray->allocIncrement = inAllocIncrement;
    dynamicArray->usedLenInBytes = 0;
    dynamicArray->allocLenInBytes = inAllocIncrement;
    dynamicArray->next = NULL;
    p_var = MM_ALLOC(inAllocIncrement);
    feature = (UniVar_Features_t *)dynamicArray;
}

void UniVariable::DynamicArrayWrite(void *in, u32 inAryLen, u32 startIndex)
{
    u8 *ptr;
    u32 unitInBytes = UniVar_GetUnitInBytes(type);
    u32 inAryLenInBytes = inAryLen * unitInBytes;
    u32 newUsedLen;
    UniVar_DynamicArray_t *dynamicArray;

    UNI_VAR_BASIC_ASSERT(feature != NULL);
    UNI_VAR_BASIC_ASSERT(feature->id == UNI_VAR_DYNAMIC_ARRAY);
    UNI_VAR_BASIC_ASSERT((unitInBytes == 1) || (unitInBytes == 2) || (unitInBytes == 4));
    UNI_VAR_BASIC_ASSERT(inAryLenInBytes >= inAryLen); //overflow
    UNI_VAR_BASIC_ASSERT(startIndex <= varLen);

    dynamicArray = (UniVar_DynamicArray_t *)feature;

    // 1.memory ran out check, if ran out, than re-allocate
    if (startIndex + inAryLen < varLen) {
        newUsedLen = varLen * unitInBytes;
    } else {
        newUsedLen = (startIndex + inAryLen) * unitInBytes;
    }
    UNI_VAR_BASIC_ASSERT(newUsedLen >= varLen * unitInBytes); //overflow
    if (newUsedLen > dynamicArray->allocLenInBytes) {
        void *newBuf;
        UNI_VAR_BASIC_ASSERT(dynamicArray->allocLenInBytes + dynamicArray->allocIncrement > dynamicArray->allocLenInBytes); //overflow
        newBuf = MM_ALLOC(dynamicArray->allocLenInBytes + dynamicArray->allocIncrement);
        MM_CPY(newBuf, p_var, varLen*unitInBytes);
        SAFE_FREE(p_var);
        p_var = newBuf;
        dynamicArray->allocLenInBytes += dynamicArray->allocIncrement;
    }

    // 2.copy data
    ptr = (u8 *)p_var;
    MM_CPY((void *)(ptr+(startIndex*unitInBytes)), in, inAryLenInBytes);
    varLen = newUsedLen / unitInBytes;
    dynamicArray->usedLenInBytes = newUsedLen;
}

#if 0
void UniVariable::DynamicArrayPushBack(void *in, u32 inAryLen)
{
    u8 *ptr;
    u32 unitInBytes = UniVar_GetUnitInBytes(type);
    u32 inAryLenInBytes = inAryLen * unitInBytes;
    u32 newUsedLen;
    UniVar_DynamicArray_t *dynamicArray;

    UNI_VAR_BASIC_ASSERT(feature != NULL);
    UNI_VAR_BASIC_ASSERT(feature->id == UNI_VAR_DYNAMIC_ARRAY);
    UNI_VAR_BASIC_ASSERT((unitInBytes == 1) || (unitInBytes == 2) || (unitInBytes == 4));
    UNI_VAR_BASIC_ASSERT(inAryLenInBytes >= inAryLen); //overflow

    dynamicArray = (UniVar_DynamicArray_t *)feature;

    // 1.memory ran out check, if ran out, than re-allocate
    newUsedLen = dynamicArray->usedLenInBytes + inAryLenInBytes;
    UNI_VAR_BASIC_ASSERT(newUsedLen >= dynamicArray->usedLenInBytes); //overflow
    if (newUsedLen > dynamicArray->allocLenInBytes) {
        void *newBuf;
        UNI_VAR_BASIC_ASSERT(dynamicArray->allocLenInBytes + dynamicArray->allocIncrement > dynamicArray->allocLenInBytes); //overflow
        newBuf = MM_ALLOC(dynamicArray->allocLenInBytes + dynamicArray->allocIncrement);
        MM_CPY(newBuf, p_var, varLen*unitInBytes);
        SAFE_FREE(p_var);
        p_var = newBuf;
        dynamicArray->allocLenInBytes += dynamicArray->allocIncrement;
    }

    // 2.copy data in the back
    ptr = (u8 *)p_var;
    MM_CPY((void *)(ptr+(varLen*unitInBytes)), in, inAryLenInBytes);
    varLen += inAryLen;
    dynamicArray->usedLenInBytes = newUsedLen;
}
#endif

void UniVariable::DynamicArrayFlush(void)
{
    UniVar_DynamicArray_t *dynamicArray;

    UNI_VAR_BASIC_ASSERT(feature != NULL);
    UNI_VAR_BASIC_ASSERT(feature->id == UNI_VAR_DYNAMIC_ARRAY);

    dynamicArray = (UniVar_DynamicArray_t *)feature;

    {
        UniVar_DynamicRestLen_t *currRestLen;
        UniVar_DynamicRestLen_t *oldRestLen;
        for (currRestLen = dynamicArray->next; currRestLen!=NULL; ) {
            oldRestLen = currRestLen;
            currRestLen = currRestLen->next;
            SAFE_FREE(oldRestLen);
        }
    }

    dynamicArray->next = NULL;
    dynamicArray->usedLenInBytes = 0;
    varLen = 0;
}

void UniVariable::DynamicArrayRestLenSet(u32 inType, bool inSwap /*= false*/)
{
    u8 buf[16] = {0};
    UniVar_DynamicArray_t *dynamicArray;
    UniVar_DynamicRestLen_t **p_currRestLen;
    UniVar_DynamicRestLen_t *newRestLen;

    UNI_VAR_BASIC_ASSERT(feature != NULL);
    UNI_VAR_BASIC_ASSERT(feature->id == UNI_VAR_DYNAMIC_ARRAY);

    dynamicArray = (UniVar_DynamicArray_t *)feature;
    newRestLen = (UniVar_DynamicRestLen_t *)MM_ALLOC(sizeof(UniVar_DynamicRestLen_t));

    newRestLen->next = NULL;
    newRestLen->type = inType;
    newRestLen->startIndex = varLen;
    newRestLen->swap = inSwap;

    p_currRestLen = &(dynamicArray->next);
    while (*p_currRestLen != NULL) {
        p_currRestLen = &((*p_currRestLen)->next);
    }
    *p_currRestLen = newRestLen;

    DynamicArrayPushBack((void *)buf, UniVar_ConvertLen(type, inType, 1));
}

void UniVariable::DynamicArrayRestLenApply(void)
{
    u32 cellLen;
    u32 restLen;
    UniVar_DynamicArray_t *dynamicArray;
    UniVar_DynamicRestLen_t *currRestLen;

    UNI_VAR_BASIC_ASSERT(feature != NULL);
    UNI_VAR_BASIC_ASSERT(feature->id == UNI_VAR_DYNAMIC_ARRAY);

    dynamicArray = (UniVar_DynamicArray_t *)feature;

    for (currRestLen = dynamicArray->next; currRestLen != NULL; currRestLen = currRestLen->next) {
        cellLen = UniVar_ConvertLen(type, currRestLen->type, 1);
        restLen = varLen - (currRestLen->startIndex) - cellLen;
        if (currRestLen->type & VAR_IS_8BITS) {
            u8 len = (u8)restLen;
            u8 len2;
            LibUtil_IntSwapCopy((u8 *)&len2, (u8 *)&len, 1, currRestLen->swap);
            DynamicArrayWrite(&len2, cellLen, currRestLen->startIndex);
        } else if (currRestLen->type & VAR_IS_16BITS) {
            u16 len = (u16)restLen;
            u16 len2;
            LibUtil_IntSwapCopy((u8 *)&len2, (u8 *)&len, 2, currRestLen->swap);
            DynamicArrayWrite(&len2, cellLen, currRestLen->startIndex);
        } else if (currRestLen->type & VAR_IS_32BITS) {
            u32 len = (u32)restLen;
            u32 len2;
            LibUtil_IntSwapCopy((u8 *)&len2, (u8 *)&len, 4, currRestLen->swap);
            DynamicArrayWrite(&len2, cellLen, currRestLen->startIndex);
        } else {
            UNI_VAR_BASIC_ASSERT(0);
        }
    }
}

u32 UniVar_GetUnitInBytes(u32 inType)
{
    if (inType & VAR_IS_8BITS) {
        return 1;
    } else if (inType & VAR_IS_16BITS) {
        return 2;
    } else if (inType & VAR_IS_32BITS) {
        return 4;
    } else if (inType & VAR_IS_64BITS) {
        return 8;
    } else if (inType & VAR_IS_128BITS) {
        return 16;
    }

    if (inType & VAR_IS_VOID_POINTER) {
        return (u32)sizeof(void *);
    }

    return 0;
}

u32 UniVar_ConvertLen(u32 dstType, u32 srcType, u32 srcLen)
{
    u32 dstUnitInBytes = UniVar_GetUnitInBytes(dstType);
    u32 srcUnitInBytes = UniVar_GetUnitInBytes(srcType);
    u32 newLen = srcLen * srcUnitInBytes / dstUnitInBytes;

    if (newLen == 0)
        newLen = 1;

    return newLen;
}

u32 GetUniStrType(char *in){ return VAR_C_STRING; };
u32 GetUniStrType(const char *in){ return VAR_C_STRING; };
u32 GetUniStrType(std::string in){ return VAR_CPP_STRING; };
u32 GetUniStrType(std::string *in){ return VAR_CPP_STRING; };
u32 GetUniAryType(u8 *in){ return VAR_U8_ARRAY; };
u32 GetUniAryType(u16 *in){ return VAR_U16_ARRAY; };
u32 GetUniAryType(u32 *in){ return VAR_U32_ARRAY; };
u32 GetUniAryType(s8 *in){ return VAR_S8_ARRAY; };
u32 GetUniAryType(s16 *in){ return VAR_S16_ARRAY; };
u32 GetUniAryType(s32 *in){ return VAR_S32_ARRAY; };
u32 GetUniType(void *in){ return VAR_VOID_POINTER; };
u32 GetUniType(u8 in){ return VAR_U8; };
u32 GetUniType(u16 in){ return VAR_U16; };
u32 GetUniType(u32 in){ return VAR_U32; };
u32 GetUniType(s8 in){ return VAR_S8; };
u32 GetUniType(s16 in){ return VAR_S16; };
u32 GetUniType(s32 in){ return VAR_S32; };
u32 GetUniType(u8 *in){ return VAR_U8; };
u32 GetUniType(u16 *in){ return VAR_U16; };
u32 GetUniType(u32 *in){ return VAR_U32; };
u32 GetUniType(s8 *in){ return VAR_S8; };
u32 GetUniType(s16 *in){ return VAR_S16; };
u32 GetUniType(s32 *in){ return VAR_S32; };
u32 GetUniType(u8 *in, u32 inAryLen){ return VAR_U8_ARRAY; };
u32 GetUniType(u16 *in, u32 inAryLen){ return VAR_U16_ARRAY; };
u32 GetUniType(u32 *in, u32 inAryLen){ return VAR_U32_ARRAY; };
u32 GetUniType(s8 *in, u32 inAryLen){ return VAR_S8_ARRAY; };
u32 GetUniType(s16 *in, u32 inAryLen){ return VAR_S16_ARRAY; };
u32 GetUniType(s32 *in, u32 inAryLen){ return VAR_S32_ARRAY; };
const char *GetUniTypeString(u32 type)
{
    switch (type)
    {
        case VAR_C_STRING  :   return "VAR_C_STRING    ";
        case VAR_CPP_STRING:   return "VAR_CPP_STRING  ";
        case VAR_VOID_POINTER: return "VAR_VOID_POINTER";
        case VAR_U8        :   return "VAR_U8          ";
        case VAR_U16       :   return "VAR_U16         ";
        case VAR_U32       :   return "VAR_U32         ";
        case VAR_S8        :   return "VAR_S8          ";
        case VAR_S16       :   return "VAR_S16         ";
        case VAR_S32       :   return "VAR_S32         ";
        case VAR_U8_ARRAY  :   return "VAR_U8_ARRAY    ";
        case VAR_U16_ARRAY :   return "VAR_U16_ARRAY   ";
        case VAR_U32_ARRAY :   return "VAR_U32_ARRAY   ";
        case VAR_S8_ARRAY  :   return "VAR_S8_ARRAY    ";
        case VAR_S16_ARRAY :   return "VAR_S16_ARRAY   ";
        case VAR_S32_ARRAY :   return "VAR_S32_ARRAY   ";
        default:
            return "Unknown Type";
    }
}

void UniVariable_Demo(void)
{
    {
        UniVariable x;
        u32 i = 0x99881122;
        x.InitEx(VAR_IS_32BITS, (void *)&i);
        //DUMPNS(x.p_name->c_str());
        DUMPNX(*((u32 *)x.p_var));
    }
    {
        u8 ary[6] = {1,2,4,7, 11, 19};
        u16 y = 0x0908;
        UniVariable x;
        x.DynamicArrayInit(ary, 6);
        x.dump();

        
        x.DynamicArrayPushBack(ary, 4);
        x.DynamicArrayPushBack(y, true);
        x.DynamicArrayWrite(ary, 6, 5);
        //x.DynamicArrayFlush();
        x.DynamicArrayRestLenSet(VAR_U16);
        x.DynamicArrayRestLenSet(VAR_U32);
        x.DynamicArrayRestLenApply();
        x.DynamicArrayPushBack(ary, 2);
        x.DynamicArrayRestLenApply();
        //x.DynamicArrayFlush();
        x.dump();
    }
    #if 0
    {
        s32 ary[6] = {1,2,4,7, 11, 19};
        UniVariable x;
        u8 i = 0xFF;
        x.DynamicArrayInit(ary);
        x.DynamicArrayPushBack(ary, 4);
        x.DynamicArrayPushBack(ary, 2);
        x.DynamicArrayPushBack(ary, 6);
        x.DynamicArrayPushBack(i, true);
        x.dump();
    }
    #endif
}