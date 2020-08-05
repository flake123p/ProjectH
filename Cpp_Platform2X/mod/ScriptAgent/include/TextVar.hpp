

#ifndef _TEXT_VAR_HPP_INCLUDED_

#include "UniVar.hpp"

typedef enum {
    TXT_VAR_IS_AUTO_FREE    = BIT_0,
    TXT_VAR_IS_IN_USE       = BIT_1,
    TXT_VAR_IS_FUNC_PTR     = BIT_2,
    TXT_VAR_IS_TEMP         = BIT_3,



    TXT_VAR_AUTO_FREE_USER_HDL = BIT_31,

    //No need for simplify
    //TXT_VAR_IS_MORE_ARGU    = ,
} TXT_VAR_FLAG_t;

typedef enum {
    TXT_VAR_IS_SINGLE = 0,
    TXT_VAR_IS_MULTI,

    TXT_VAR_IS_NESTED,
    TXT_VAR_RET_CONST,

    TXT_IS_VARIABLE,     // i[j[k[3]]]
    TXT_IS_CONST,        // ret temp variable, 1st string is print symbol %d %x 0x%X ... (u8 s8 u32 ...) num num num ...

    //errors
    TXT_VAR_ERROR = 0x80,
    TXT_VAR_CANT_BE_FOUND,
    TXT_VAR_OUT_OF_RANGE,
    TXT_VAR_FORMAT_ERROR,
    TXT_VAR_IS_NOT_INITED,
    TXT_VAR_ERROR_MAX = 0x8F,
} TXT_VAR_RET_t;

class TextVar{
public:
    std::string name;
    UniVar *pUniVar; //use delete() if auto release is on

    u32 flags; //TXT_VAR_FLAG_t
    u32 start;
    u32 end; // 0xFFFFFFFF for max num
    u32 val32;
    u32 userData;
    void *userHdl;

    /*
        void *pMETA_DATA
        High Level IPC Lock
    */
    TextVar(void){
        name="";
        pUniVar=new(UniVar);
        flags=TXT_VAR_IS_AUTO_FREE;
        start=0;
        end=0;
        val32=0;
        userData=0;
        userHdl=NULL;
    };
    ~TextVar(){
        SAFE_DELETE(pUniVar);
        if (flags&TXT_VAR_AUTO_FREE_USER_HDL) {
            SAFE_FREE2(userHdl);
        }
        BASIC_ASSERT(userHdl==NULL);
        //PRINT_FUNC;
    };
    void SetToTempVar(void) {
        flags|=TXT_VAR_IS_TEMP;
    };
    // Return 1 for get success, 0 for string is not const num (0 1 4 A B 0x08 0X1234 ...)
    int ImportSingleConstNumByString(std::string *str, std::string *scanFormat, u32 type);
    void TextVarDump(void);
};

class TextVarDB: virtual public Tracer {
public:
    std::map<std::string, TextVar *> varDB;

    void _clearParameter(void) {
        std::map<std::string, TextVar *>::iterator it;
        TextVar *pTextVar;
        for (it=varDB.begin(); it!=varDB.end(); it++) {
            pTextVar = (TextVar *)it->second;
            if (pTextVar->flags & TXT_VAR_IS_AUTO_FREE) {
                SAFE_DELETE(pTextVar);
            }
        }
        varDB.clear();
    };

    TextVarDB(int disableTracer = 0) : Tracer("TextVar", disableTracer)
    {
        //_clearParameter();
    };
    ~TextVarDB(){_clearParameter();};

    TextVar *AddVarPair(std::string *str, TextVar *pTextVar = NULL);
    TextVar *AddVarPair(const char *str, TextVar *pTextVar = NULL){std::string cppStr=str;return AddVarPair(&cppStr, pTextVar);};
    TextVar *FindVar(std::string *str);
    TextVar *FindVar(const char *str){std::string cppStr=str;return FindVar(&cppStr);};

    // http://www.cplusplus.com/reference/map/map/erase/
    int EraseVar(std::string *name, TextVar **outTextVar =NULL);
    int EraseVar(const char *name, TextVar **outTextVar =NULL){std::string cppStr=name;return EraseVar(&cppStr, outTextVar);};

    TXT_VAR_RET_t GetVarVal32(std::string *inTxt, TextVar **outVar = NULL, u32 *outVal32 = NULL);
    TXT_VAR_RET_t ParseVarOrConst(LibStringClass *splitedStr, u32 subStrStartIdx, u32 *outSubStrEndIdx, TextVar **outVar, u32 *outVal32);

    void Dump(void) {
        std::map<std::string, TextVar *>::iterator it;
        for (it=varDB.begin(); it!=varDB.end(); it++) {
            it->second->TextVarDump();
        }
    };
    //int AddPair(const char *str, TextCall_CB_t cb, void *userHdl_0 = NULL, void *userHdl_1 = NULL);
    //int RemovePair(const char *str, int doRelease = 1);
};

// Return 1 for get success, 0 for invalid input string
int TextVar_GetTypeFromString(std::string *str, u32 *outType);

void TextVar_Demo(void);


#define _TEXT_VAR_HPP_INCLUDED_
#endif//_TEXT_VAR_HPP_INCLUDED_

