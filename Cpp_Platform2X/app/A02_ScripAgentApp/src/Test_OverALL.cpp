
#include "Everything_App.hpp"

void Test_UniVariable_TouchAssert_InitTest(void)
{
    {
        UniVariable x("axdddd");
        BASIC_ASSERT(x.type == VAR_IS_C_STRING);
        DUMPNS((const char *)x.p_var);
        DUMPND(x.varLen);
    }
    {
        UniVariable x;
        char str[3] = {'d', '8', 0};
        x.Init(str);
        BASIC_ASSERT(x.type == VAR_IS_C_STRING);
        DUMPNS((const char *)x.p_var);
        DUMPND(x.varLen);
    }
    {
        std::string str = "cppstring";
        UniVariable x(str);
        BASIC_ASSERT(x.type == VAR_IS_CPP_STRING);
        DUMPNS(((std::string *)x.p_var)->c_str());
        DUMPND(x.varLen);
    }
    {
        std::string str = "cppstringpointer";
        UniVariable x(&str);
        BASIC_ASSERT(x.type == VAR_IS_CPP_STRING);
        DUMPNS(((std::string *)x.p_var)->c_str());
        DUMPND(x.varLen);
        x.dump();
    }
    {u8 i=0;UniVariable x(i);BASIC_ASSERT(x.type == VAR_U8);}
    {u8 i=0;UniVariable x(&i);BASIC_ASSERT(x.type == VAR_U8);}
    {u16 i=0;UniVariable x(i);BASIC_ASSERT(x.type == VAR_U16);}
    {s16 i=0;UniVariable x(&i);BASIC_ASSERT(x.type == (u32)VAR_S16);}

    {
        u32 ary[6] = {1,2,4,7};
        UniVariable x(ary, 3);
        BASIC_ASSERT(x.type == VAR_U32_ARRAY);
        x.dump();
    }

}

void Test_UniVariable_TouchAssert(void)
{
    //Test_UniVariable_TouchAssert_InitTest();
    UniVariable_Demo();

    return;
    /*
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
    */

}

