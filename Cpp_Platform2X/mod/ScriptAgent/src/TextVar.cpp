#include "Everything_ThisMod.hpp"

// Return 1 for get success, 0 for string is not const num (0 1 4 A B 0x08 0X1234 ...)
int TextVar::ImportSingleConstNumByString(std::string *str, std::string *scanFormat, u32 type)
{
    u32 temp;

    if (LibString_IsStringNumerical(str->c_str())) {
        //continue
    } else {
        return 0;
    }

    sscanf(str->c_str(), scanFormat->c_str(), &temp);

    switch (type) {
        case VAR_U8:  pUniVar->Import((u8)temp);  break;
        case VAR_U16: pUniVar->Import((u16)temp); break;
        case VAR_U32: pUniVar->Import((u32)temp); break;
        case VAR_S8:  pUniVar->Import((s8)temp);  break;
        case VAR_S16: pUniVar->Import((s16)temp); break;
        case VAR_S32: pUniVar->Import((s32)temp); break;
        default:
            break;
    }

    return 1;
}

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

TXT_VAR_RET_t TextVarDB::GetVarVal32(std::string *inTxt, TextVar **outVar /*= NULL*/, u32 *outVal32 /*= NULL*/)
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
        return TXT_IS_CONST;
    }

    if (LibString_IsArrayPattern2(inTxt, &varStr, &indexStr)) {
        //nested find
        TXT_VAR_RET_t ret = GetVarVal32(&indexStr, NULL, &temp32);
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

/*
    Un-finished
*/
TXT_VAR_RET_t TextVarDB::ParseVarOrConst(LibStringClass *splitedStr, u32 subStrStartIdx, u32 *outSubStrEndIdx, TextVar **outVar, u32 *outVal32)
{
    BASIC_ASSERT(subStrStartIdx < splitedStr->subStrVector.size());
    TXT_VAR_RET_t ret = TXT_VAR_ERROR;
    TextVar *var = NULL;

    if (LibString_IsPrintPattern(&(splitedStr->subStrVector[subStrStartIdx]))) {
        std::string *scanFormat = &(splitedStr->subStrVector[subStrStartIdx]);
        scanFormat = scanFormat;//temp
        subStrStartIdx += 1; //next string
        RETURN_WHEN(subStrStartIdx >= splitedStr->subStrVector.size(), TXT_VAR_FORMAT_ERROR);
        ret = TXT_IS_CONST;
        var = new(TextVar);
        var->SetToTempVar();
        //determin type
        u32 varScanType;
        if (TextVar_GetTypeFromString(&(splitedStr->subStrVector[subStrStartIdx]), &varScanType)) {
            subStrStartIdx += 1; //next string
            RETURN_WHEN(subStrStartIdx >= splitedStr->subStrVector.size(), TXT_VAR_FORMAT_ERROR);
        } else {
            varScanType = VAR_U32;
        }
    } else {
        // go find variable
        ret = TXT_IS_VARIABLE;
    }

    
    return ret;
}

// Return 1 for get success, 0 for invalid input string
int TextVar_GetTypeFromString(std::string *str, u32 *outType)
{
    int ret = 1;
    u32 type = VAR_IS_UNINITED;
    if (*str == "u8") {
        type = VAR_U8;
    } else if (*str == "u16") {
        type = VAR_U16;
    } else if (*str == "u32") {
        type = VAR_U32;
    } else if (*str == "s8") {
        type = VAR_S8;
    } else if (*str == "s16") {
        type = VAR_S16;
    } else if (*str == "s32") {
        type = VAR_S32;
    } else if (*str == "str") {
        type = VAR_C_STRING;
    } else {
        ret = 0;
    }

    if (outType != NULL) {
        if (type != VAR_IS_UNINITED) {
            *outType = type;
        }
    }
    return ret;
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

