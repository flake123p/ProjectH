

#ifndef _LIB_ERROR_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================

extern int rc;

#define EXIT_IF_ERROR(a) if(a){LibError_PrintErrorMessage(a);exit(a);}

void LibError_PrintErrorMessage(int retVal, bool verbose = false);
void LibError_Demo(void);

#define _LIB_ERROR_HPP_INCLUDED_
#endif//_LIB_ERROR_HPP_INCLUDED_


