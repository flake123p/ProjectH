
#include "Everything_ThisMod.hpp"

UniVariable::UniVariable(std::string *p_inName /*= NULL*/, u32 inType /*= VAR_IS_UNINITED*/, u32 inVarLen /*= 0*/, void *p_inVar /*= NULL*/)
{
    retCode = 0;
    Init(p_inName, inType, inVarLen, p_inVar);
}

UniVariable::~UniVariable(void)
{
    SAFE_DELETE(p_name);
    SAFE_FREE(p_var);
}

void UniVariable::Init(std::string *p_inName /*= NULL*/, u32 inType /*= VAR_IS_UNINITED*/, u32 inVarLen /*= 0*/, void *p_inVar /*= NULL*/)
{
    p_name = p_inName;
    type = inType;
    varLen = inVarLen;
    if (inVarLen) {
        p_var = MM_ALLOC(inVarLen);
        MM_CPY(p_var, p_inVar, inVarLen);
    } else {
        p_var = NULL;
    }
}

void UniVariable::Init(const char *p_inName /*= NULL*/, u32 inType /*= VAR_IS_UNINITED*/, u32 inVarLen /*= 0*/, void *p_inVar /*= NULL*/)
{
    if (p_inName == NULL) {
        p_name = NULL;
    } else {
        p_name = new(std::string);
        *p_name = p_inName;
    }
    type = inType;
    retCode = 0;
    varLen = inVarLen;
    if (inVarLen) {
        p_var = MM_ALLOC(inVarLen);
        MM_CPY(p_var, p_inVar, inVarLen);
    } else {
        p_var = NULL;
    }
}

void UniVariable_Demo(void)
{
    UniVariable x;
    u32 i = 0x99881122;
    x.Init("abcc", VAR_IS_32BITS, 4, (void *)&i);
    DUMPNS(x.p_name->c_str());
    DUMPNX(*((u32 *)x.p_var));
}