#include "Everything_ThisMod.hpp"

void TextVar::TextVarDump(void)
{
    printf("====== %s() ====== name : %s\n", __func__, name.c_str());
    DUMPND(passingVarNum);
    DUMPNX(flags);

    if(pUniVar!=NULL) {
        pUniVar->UniVarDump();
    }
};

TextVar *TextVarDB::AddPair(std::string str, TextVar *pTextVar /*= NULL*/)
{
    if (pTextVar == NULL) {
        pTextVar = new(TextVar);
    }
    pTextVar->name = str;

    {
        std::pair<std::map<std::string, TextVar *>::iterator,bool> ret;
        ret = varDB.insert ( std::pair<std::string, TextVar *>(str, pTextVar) );
        if (ret.second==false) {
            // The name is exist.
            return NULL;
        }
    }

    return pTextVar;
}

TextVar *TextVarDB::Find(std::string name)
{
    std::map<std::string, TextVar *>::iterator it;

    it = varDB.find(name);
    if (it == varDB.end()) {
        // Can't find it
        return NULL;
    }

    return it->second;
}

int TextVarDB::Erase(std::string name, TextVar **outTextVar /*=NULL*/)
{
    int doAutoDelete = 0;

    std::map<std::string, TextVar *>::iterator it;

    it = varDB.find(name);
    if (it == varDB.end()) {
        // Can't find it
        return 1;
    }

    varDB.erase(it);
    if (it->second->flags & TXT_VAR_IS_AUTO_FREE) {
        doAutoDelete = 1;
    }

    if (outTextVar != NULL) {
        *outTextVar = it->second;
        doAutoDelete = 0;
    }

    if (doAutoDelete) {
        SAFE_DELETE(it->second);
        //erased
        return 2;
    }

    return 0;
}

void TextVar_Demo(void)
{
    MM_INIT();
    PRINT_FUNC;
    PRINT_FUNC;

    TextVarDB *db = new(TextVarDB);
    db->AddPair("123");
    db->AddPair("ab");
    TextVar *test = db->Find("123");
    test->name = "ffdd0";
    db->Erase("123", &test);
    PRINT_LINE;
    db->Dump();
    PRINT_LINE;
    test->TextVarDump();
    SAFE_DELETE(test);
    SAFE_DELETE(db);
    MM_UNINIT();
}

