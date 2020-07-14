

#ifndef _TEXT_CALL_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"

typedef int (*TextCall_CB_t)(LibStringClass *splitedStrAgent, void *userHdl_0, void *userHdl_1);

typedef struct {
    TextCall_CB_t cb;
    void *userHdl_0;
    void *userHdl_1;
} TextCall_t;

class TextCall: virtual public Tracer {
public:
    std::map<std::string,TextCall_t> callbackMap;

    void _clearParameter(void) {
        callbackMap.clear();
    };

    TextCall(int disableTracer = 0) : Tracer("TextCall", disableTracer)
    {
        _clearParameter();
    };
    ~TextCall(){_clearParameter();};
    void Dump(void);
    int AddPair(const char *str, TextCall_CB_t cb, void *userHdl_0 = NULL, void *userHdl_1 = NULL);
    int Start(const char *line, int *cbRet = NULL);
    //int RemovePair(const char *str);
};

void TextCall_Demo(void);


#define _TEXT_CALL_HPP_INCLUDED_
#endif//_TEXT_CALL_HPP_INCLUDED_

