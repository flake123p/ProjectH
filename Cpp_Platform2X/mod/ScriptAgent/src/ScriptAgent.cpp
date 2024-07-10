
#include "Everything_ThisMod.hpp"

ScriptAgent::ScriptAgent(int disableTracer /* = 0*/) : Tracer("ScriptAgent", disableTracer)
{
    file = NULL;
}
ScriptAgent::~ScriptAgent()
{
    SAFE_DELETE(file);
}

int ScriptAgent_Demo(void)
{
    LibMem_Init();
    printf("This is %s()\n", __func__);
    //ScriptAgent sa = ScriptAgent(DISABLE_TRACER);
    class ScriptAgent *b = new(ScriptAgent);
    delete(b);

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
