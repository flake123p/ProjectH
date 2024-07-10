

#ifndef _LIB_ERROR_HPP_INCLUDED_

#include <stdarg.h>

// ============================== Debug ==============================

// ============================== Define ==============================




#ifdef DO_NOT_USE_EXIT
#define EXIT(a) return (a)
#else
#define EXIT(a) exit(a)
#endif

extern int rc;

#define EXIT_IF(a) if(a){LibError_PrintExtErrorMessage("[EXT ERROR]:");perror("perror()");LibError_PrintErrorMessage(a);EXIT(a);}
#define EXIT_MSG_IF(a, ...) if(a){LibError_PrintExtErrorMessage("[EXT ERROR]:");perror("perror()");printf(__VA_ARGS__);EXIT(a);}
#define EXIT_LOC_IF(a) if(a){LibError_PrintExtErrorMessage("[EXT ERROR]:");perror("perror()");LibError_PrintErrorMessage(a);printf(">>> EXIT: in %s(), %s:line %d\n",__FUNCTION__,__FILE__,__LINE__);EXIT(a);}

#define EXIT_CHK(a, b)  a=b;EXIT_LOC_IF(a)

#define EXIT_WHEN(a) if(a){LibError_PrintExtErrorMessage("[EXT ERROR]:");perror("perror()");printf(">>> EXIT: in %s(), line %d\n",__FUNCTION__,__LINE__);EXIT(0xFF);}

void LibError_PrintErrorMessage(int retVal, bool verbose = false);
void LibError_Demo(void);

int LibError_SetExtErrorMessage(const char *errorStr, ...);
int LibError_PrintExtErrorMessage(const char *precStr);

#define _LIB_ERROR_HPP_INCLUDED_
#endif//_LIB_ERROR_HPP_INCLUDED_


