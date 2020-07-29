
#include "Everything_App.hpp"

int main(int argc, char *argv[])
{
    //ScriptAgent_Demo();
    //UniVariable_Demo();
    //Test_UniVariable_TouchAssert();

    //TextCall_Demo();
    //TextVar_Demo();

#define INPUT_SCRIPT "templateScript.txt"

    //TextCall_Demo();

    LibFileIoClass testFile(INPUT_SCRIPT, "r+b");

    testFile.FileOpenForRead(1024);

    MM_INIT();
    TextCallDB *db0 = new(TextCallDB);
    db0->currFile = INPUT_SCRIPT;
    TextCall_DefaultInit(db0);
    TextCall_BasicInit(db0);

    int ret;
    while (testFile.GetLine() != RC_FILE_REACH_EOF) {
        db0->currFileLine += 1;
        db0->Start((const char *)testFile.lineStr, &ret);
        if (ret == TXT_CALL_RET_EXIT) {
            break;
        }
    }

    TextCall_DefaultUninit(db0);
    MM_UNINIT();

    return 0;
}

