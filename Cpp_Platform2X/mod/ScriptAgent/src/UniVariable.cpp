
#include "Everything_ThisMod.hpp"

#if 1
#define UNI_VAR_BASIC_ASSERT BASIC_ASSERT
#else
#define UNI_VAR_BASIC_ASSERT()
#endif

UniVariable::UniVariable(u32 inType /*= VAR_IS_UNINITED*/, const void *p_inVar /*= NULL*/, u32 inAryLen /*= 0 only used in array type*/)
{
    type = VAR_IS_UNINITED;
    p_var = NULL;
    varLen = 0;
    Init(inType, p_inVar, inAryLen);
}

UniVariable::~UniVariable(void)
{
    Uninit();
}

void UniVariable::Uninit(void)
{
    if (type & VAR_IS_CPP_STRING) {
        SAFE_DELETE((std::string *)p_var);
    } else {
        SAFE_FREE(p_var);
    }
    type = VAR_IS_UNINITED;
    p_var = NULL;
    varLen = 0;
}

void UniVariable::Init(u32 inType /*= VAR_IS_UNINITED*/, const void *p_inVar /*= NULL*/, u32 inAryLen /*= 0 only used in array type*/)
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

void UniVariable_Demo(void)
{
    UniVariable x;
    u32 i = 0x99881122;
    x.Init(VAR_IS_32BITS, (void *)&i);
    //DUMPNS(x.p_name->c_str());
    DUMPNX(*((u32 *)x.p_var));
}