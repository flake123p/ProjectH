
#include "Everything_ThisMod.hpp"

#define MM_NEW(class)

int ScriptAgent_Demo(void)
{
    LibMem_Init();
    printf("This is %s()\n", __func__);
    //ScriptAgent sa;
    testx *t = new(testx);

    LibMem_Uninit();
    return 0;
}

int ScriptAgent_ParsingFile(char *fileName)
{
    printf("This is %s()\n", __func__);

    return 0;
}

int ScriptAgent_Close(char *fileName)
{
    printf("This is %s()\n", __func__);

    return 0;
}
