#include "Everything_ThisMod.hpp"

void TextVar::TextVarDump(void)
{
    printf("====== %s() ====== name : %s\n", __func__, name.c_str());
    DUMPND(start);
    DUMPND(end);
    DUMPND(val32);
    DUMPNX(flags);

    if(pUniVar!=NULL) {
        pUniVar->UniVarDump();
    }
};

TextVar *TextVarDB::AddVarPair(std::string *str, TextVar *pTextVar /*= NULL*/)
{
    if (pTextVar == NULL) {
        pTextVar = new(TextVar);
    }
    pTextVar->name = *str;

    {
        std::pair<std::map<std::string, TextVar *>::iterator,bool> ret;
        ret = varDB.insert ( std::pair<std::string, TextVar *>(*str, pTextVar) );
        if (ret.second==false) {
            // The name is exist.
            return NULL;
        }
    }

    return pTextVar;
}

TextVar *TextVarDB::FindVar(std::string *name)
{
    std::map<std::string, TextVar *>::iterator it;

    it = varDB.find(*name);
    if (it == varDB.end()) {
        // Can't find it
        return NULL;
    }

    return it->second;
}

int TextVarDB::EraseVar(std::string *name, TextVar **outTextVar /*=NULL*/)
{
    int doAutoDelete = 0;

    std::map<std::string, TextVar *>::iterator it;

    it = varDB.find(*name);
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

TXT_VAR_ATT_t TextVarDB::TextVar_GetVal32(std::string *inTxt, TextVar **outVar /*= NULL*/, u32 *outVal32 /*= NULL*/)
{
    //find '-'
    //get index str
    //get name str
    //get var
    //get val by index
    {
        size_t retPos = inTxt->find('-');

        if (retPos != std::string::npos) {
            return TXT_VAR_FORMAT_ERROR;
        }
    }

    std::string varStr;
    std::string indexStr;
    u32 index = 0;
    u32 temp32;

    if (LibString_GetDeciOrHex32(inTxt, &temp32)) {
        if (outVal32 != NULL) {
            *outVal32 = temp32;
        }
    }

    if (LibString_IsArrayPattern2(inTxt, &varStr, &indexStr)) {
        //nested find
        TXT_VAR_ATT_t ret = TextVar_GetVal32(&indexStr, NULL, &temp32);
        if (ret & TXT_VAR_ERROR) {
            return ret;
        }
        index = temp32;
    } else {
        index = 0;
    }
    TextVar *var = FindVar(&varStr);
    if (var == NULL) {
        printf("[Unknown Var: %s in %s (%d)]", varStr.c_str(), inTxt->c_str(), __LINE__);
        return TXT_VAR_CANT_BE_FOUND;
    }

    u32 varMaxNum = var->pUniVar->MaxNum();
    if (varMaxNum == 0) {
        printf("[Uninit Var: %s in %s (%d)]", varStr.c_str(), inTxt->c_str(), __LINE__);
        return TXT_VAR_IS_NOT_INITED;
    }
    if (index >= varMaxNum) {
        printf("[Var Range Error:%s (Max:%d)] (%d)", inTxt->c_str(), varMaxNum-1, __LINE__);
        return TXT_VAR_OUT_OF_RANGE;
    }
    var->pUniVar->GetSingleElement32(index, &temp32);
    if (outVar != NULL) {
        var->val32 = temp32;
        var->start = index;
        *outVar = var;
    }
    if (outVal32 != NULL) {
        *outVal32 = temp32;
    }

    return TXT_VAR_IS_SINGLE;
}

void TextVar_Demo(void)
{
    MM_INIT();
    PRINT_FUNC;
    PRINT_FUNC;

    TextVarDB *db = new(TextVarDB);
    db->AddVarPair("123");
    db->AddVarPair("ab");
    TextVar *test = db->FindVar("123");
    test->name = "ffdd0";
    db->EraseVar("123", &test);
    PRINT_LINE;
    db->Dump();
    PRINT_LINE;
    test->TextVarDump();
    SAFE_DELETE(test);
    SAFE_DELETE(db);
    MM_UNINIT();
}

