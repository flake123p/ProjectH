

#ifndef _TEXT_VAR_HPP_INCLUDED_

#include "UniVar.hpp"

typedef enum {
    TXT_VAR_IS_AUTO_FREE    = BIT_0,
    TXT_VAR_IS_IN_USE       = BIT_1,
    TXT_VAR_IS_FUNC_PTR     = BIT_2,



    TXT_VAR_AUTO_FREE_USER_HDL = BIT_31,

    //No need for simplify
    //TXT_VAR_IS_MORE_ARGU    = ,
} TXT_VAR_FLAG_t;

class TextVar{
public:
    std::string name;
    UniVar *pUniVar; //use delete() if auto release is on

    u32 flags; //TXT_VAR_FLAG_t
    u32 passingVarNum; // 0xFFFFFFFF for max num
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
        passingVarNum=0;
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

    TextVar *AddPair(std::string str, TextVar *pTextVar = NULL);
    TextVar *AddPair(const char *str, TextVar *pTextVar = NULL){std::string cppStr=str;return AddPair(cppStr, pTextVar);};
    TextVar *Find(std::string str);
    TextVar *Find(const char *str){std::string cppStr=str;return Find(cppStr);};

    // http://www.cplusplus.com/reference/map/map/erase/
    int Erase(std::string name, TextVar **outTextVar =NULL);
    int Erase(const char *name, TextVar **outTextVar =NULL){std::string cppStr=name;return Erase(cppStr, outTextVar);};

    void Dump(void) {
        std::map<std::string, TextVar *>::iterator it;
        for (it=varDB.begin(); it!=varDB.end(); it++) {
            it->second->TextVarDump();
        }
    };
    //int AddPair(const char *str, TextCall_CB_t cb, void *userHdl_0 = NULL, void *userHdl_1 = NULL);
    //int RemovePair(const char *str, int doRelease = 1);
};

void TextVar_Demo(void);


#define _TEXT_VAR_HPP_INCLUDED_
#endif//_TEXT_VAR_HPP_INCLUDED_

