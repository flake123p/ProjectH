
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
    u32 unitInBytes = GetUnitInBytes(inType);

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
    } else if(type != VAR_IS_UNINITED) {
        UNI_VAR_BASIC_ASSERT(p_inVar != NULL);
        UNI_VAR_BASIC_ASSERT((type & (VAR_IS_ARRAY|VAR_IS_C_STRING|VAR_IS_CPP_STRING)) == 0);
        //single variable
        varLen = unitInBytes;
        p_var = MM_ALLOC(unitInBytes);
        if (p_inVar != NULL) {
            MM_CPY(p_var, p_inVar, unitInBytes);
        }
    }
}

u32 UniVariable::GetUnitInBytes(u32 inType)
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

    return 0;
}

void UniVariable::dump(void)
{
    printf("type = 0x%08X : ", type);

    switch (type) {
        case VAR_C_STRING: printf("VAR_C_STRING\n"); break;
        case VAR_CPP_STRING: printf("VAR_CPP_STRING\n"); break;
        case VAR_U8: printf("VAR_U8\n"); break;
        case VAR_U16: printf("VAR_U16\n"); break;
        case VAR_U32: printf("VAR_U32\n"); break;
        case VAR_S8: printf("VAR_S8\n"); break;
        case VAR_S16: printf("VAR_S16\n"); break;
        case VAR_S32: printf("VAR_S32\n"); break;
        case VAR_U8_ARRAY: printf("VAR_U8_ARRAY\n"); break;
        case VAR_U16_ARRAY: printf("VAR_U16_ARRAY\n"); break;
        case VAR_U32_ARRAY: printf("VAR_U32_ARRAY\n"); break;
        case VAR_S8_ARRAY: printf("VAR_S8_ARRAY\n"); break;
        case VAR_S16_ARRAY: printf("VAR_S16_ARRAY\n"); break;
        case VAR_S32_ARRAY: printf("VAR_S32_ARRAY\n"); break;
        default:
            BASIC_ASSERT(0);
            break;
    }

    if(type & VAR_IS_ARRAY) {
        printf("var =\n");
        u32 i;
        if (type & VAR_IS_8BITS) {
            u8 *ary = (u8 *)p_var;
            for (i=0; i<varLen; i++) {
                printf("0x%02X ", ary[i]);
                if ((i % 8) == 7) {
                    printf("\n");
                }
            }
            if (((i-1) % 8) != 7) {
                printf("\n");
            }
        } else if (type & VAR_IS_16BITS) {
            u16 *ary = (u16 *)p_var;
            for (i=0; i<varLen; i++) {
                printf("0x%04X ", ary[i]);
                if ((i % 8) == 7) {
                    printf("\n");
                }
            }
            if (((i-1) % 8) != 7) {
                printf("\n");
            }
        } else if (type & VAR_IS_32BITS) {
            u32 *ary = (u32 *)p_var;
            for (i=0; i<varLen; i++) {
                printf("0x%08X ", ary[i]);
                if ((i % 4) == 3) {
                    printf("\n");
                }
            }
            if (((i-1) % 4) != 3) {
                printf("\n");
            }
        } else {
            BASIC_ASSERT(0);
        }
    } else if(type & VAR_IS_C_STRING) {
        printf("var = %s\n", (const char *)p_var);
    } else if(type & VAR_IS_CPP_STRING) {
        printf("var = %s\n", ((std::string *)p_var)->c_str());
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
}

void UniVariable::InitDynamicArray(u32 inType, u32 inAllocIncrement /*= 1000*/)
{
    UniVar_Features_t **p_currFeature = &feature;
    UniVar_DynamicArray_t *dynamicArray;

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

    dynamicArray = (UniVar_DynamicArray_t *)MM_ALLOC(sizeof(UniVar_DynamicArray_t));
    dynamicArray->hdr.next = NULL;
    dynamicArray->hdr.id = UNI_VAR_DYNAMIC_ARRAY;
    dynamicArray->allocIncrement = inAllocIncrement;
    dynamicArray->usedLenInBytes = 0;
    dynamicArray->allocLenInBytes = inAllocIncrement;
    p_var = MM_ALLOC(inAllocIncrement);
    *p_currFeature = (UniVar_Features_t *)dynamicArray;
}

void UniVariable::PushDynamicArray(void *in, u32 inAryLen)
{
    //cache?
}

void UniVariable_Demo(void)
{
    UniVariable x;
    u32 i = 0x99881122;
    x.InitEx(VAR_IS_32BITS, (void *)&i);
    //DUMPNS(x.p_name->c_str());
    DUMPNX(*((u32 *)x.p_var));
}