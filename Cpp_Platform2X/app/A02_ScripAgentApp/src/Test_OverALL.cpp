
#include "Everything_App.hpp"

void Test_UniVariable_TouchAssert(void)
{
    {
        UniVariable x;
        u32 i = 0x99881122;
        x.Init(VAR_IS_32BITS, (void *)&i);
        //DUMPNS(x.p_name->c_str());
        DUMPNX(*((u32 *)x.p_var));
        DUMPND(x.varLen);
    }
    {
        UniVariable x;
        //u32 i = 0x99881122;
        x.Init(VAR_IS_C_STRING, "abc");
        DUMPNS((const char *)x.p_var);
        DUMPND(x.varLen);
    }
    {
        std::string str = "bbbddd";
        UniVariable x;
        x.Init(VAR_IS_CPP_STRING, &str);
        DUMPNS(((std::string *)x.p_var)->c_str());
        DUMPND(x.varLen);
        x.Uninit();
    }
}

