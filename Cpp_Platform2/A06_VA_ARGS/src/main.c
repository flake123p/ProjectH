
#include "Everything_App.hpp"
/*

Stringizing operator (#)
Token-pasting operator (##)

// make string
#define MKSTR(s) #s

// result = ab
#define CONCAT(a, b) a##b

*/

// error only 1 argu, e.g.: fo("123"), because it became fo("123",)
#define fo(f, ...) printf(f, __VA_ARGS__)
// use ## to eliminate ','
#define foo(f, ...) printf(f, ##__VA_ARGS__)

int sum(int num, ...)
{
    va_list args;
    va_start(args, num);
    
    int i, sum = 0;
    for(i = 0; i < num; i++){
        sum += va_arg(args, int);
    }
    
    va_end(args);
    
    return sum;
}

int main(int argc, char *argv[])
{
    {
        // error
        //fo("123\n");
        // ok
        foo("123\n");
    }

    {
        #define soo(num, ...) sum(num, ##__VA_ARGS__)
        int ret = soo(1, 99);
        DUMPND(ret);
    }

    return 0;
}


