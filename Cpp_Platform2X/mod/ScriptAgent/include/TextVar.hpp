

#ifndef _TEXT_VAR_HPP_INCLUDED_

#include "UniVariable.hpp"

typedef enum {
    TXT_VAR_IS_AUTO_FREE    = BIT_0,
    TXT_VAR_IS_IN_USE       = BIT_1,

    //No need for simplify
    //TXT_VAR_IS_MORE_ARGU    = BIT_2,
} TXT_VAR_FLAG_t;

typedef struct TextVar_t {
    UniVariable *pUniVar; //use delete() if auto release is on
    u32 flags; //TXT_VAR_FLAG_t
    std::string *nextArgu; //if isInUse == 0, nextArgu is NULL!
    /*
        void *pMETA_DATA
        High Level IPC Lock
    */
} TextVar_t;

class TextVar: virtual public Tracer {
public:
    std::map<std::string, TextVar_t> varMap;

    void _clearParameter(void) {
        //TODO: delete all uniVariables
        varMap.clear();
    };

    TextVar(int disableTracer = 0) : Tracer("TextVar", disableTracer)
    {
        //_clearParameter();
    };
    ~TextVar(){_clearParameter();};
    void Dump(void) {
        //std::map<std::string, TextVar_t>::iterator it;
        //for (it=argMap.begin(); it!=argMap.end(); it++) {
        //    printf("%2u, %s\n", it->first, it->second.c_str());
        ///}
    };
    //int AddPair(const char *str, TextCall_CB_t cb, void *userHdl_0 = NULL, void *userHdl_1 = NULL);
    //int RemovePair(const char *str, int doRelease = 1);
};

void TextVar_Demo(void);


#define _TEXT_VAR_HPP_INCLUDED_
#endif//_TEXT_VAR_HPP_INCLUDED_

