

#ifndef _TEXT_CALL_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"
#include <vector>
#include "TextVar.hpp"

//std::vector<std::string> standAloneArgs;

typedef int (*TextCall_CALL_CB_t)(void *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1);

typedef int (*TextCall_CB_t)(void *textCallDB, LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1);

typedef struct {
    TextCall_CB_t cb;
    void *userHdl_0;
    void *userHdl_1;
} TextCall_t;

class TextCallDB: virtual public Tracer {
public:
    std::map<std::string,TextCall_t> callbackMap;
    TextCallDB *textCallDB;
    TextVarDB  *textVarDB;

    void _clearParameter(void) {
        callbackMap.clear();
    };

    TextCallDB(int disableTracer = 0) : Tracer("TextCall", disableTracer)
    {
        _clearParameter();
    };
    ~TextCallDB(){_clearParameter();};
    void Dump(void);
    int AddPair(const char *str, TextCall_CB_t cb, void *userHdl_0 = NULL, void *userHdl_1 = NULL);
    int Start(const char *line, int *cbRet = NULL);
    //int RemovePair(const char *str);
};

void TextCall_Demo(void);


#define _TEXT_CALL_HPP_INCLUDED_
#endif//_TEXT_CALL_HPP_INCLUDED_

