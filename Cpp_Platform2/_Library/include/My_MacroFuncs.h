


#ifndef _MY_MACROFUNCS_H_INCLUDED_



#define USE_MY_ASSERT (1)
#if USE_MY_ASSERT
#include <stdio.h>
#define BASIC_ASSERT(a) if(a){;}else{printf(" Assertion failed: in %s(), line %d\n",__FUNCTION__,__LINE__);while(1){;}}
#define CALLER_ASSERT(a) if(a){;}else{printf(" Assertion failed: in %s(), line %d\n Caller: %s()",__FUNCTION__,__LINE__,caller);while(1){;}}
#define CALLER_ASSERT2(caller,a) if(a){;}else{printf(" Assertion failed: in %s(), line %d\n Caller: %s()",__FUNCTION__,__LINE__,caller);while(1){;}}
#else
#include <assert.h>
#define BASIC_ASSERT assert
#define CALLER_ASSERT(a) if(a){;}else{printf("Caller: %s()",caller);assert(0);}
#define CALLER_ASSERT2(caller,a) if(a){;}else{printf("Caller: %s()",caller);assert(0);}
#endif

#define NO_ASSERT (0)
#if NO_ASSERT
#undef BASIC_ASSERT
#define BASIC_ASSERT(...)
#undef CALLER_ASSERT
#define CALLER_ASSERT(...)
#undef CALLER_ASSERT2
#define CALLER_ASSERT2(...)
#endif

#define REMOVE_UNUSED_WRANING(a) (a=a)



#define DUMPC(a) printf(#a " = %c\n", a)
#define DUMPS(a) printf(#a " = %s\n", a)
#define DUMPD(a) printf(#a " = %d\n", a)
#define DUMPU(a) printf(#a " = %u\n", a)
#define DUMPX(a) printf(#a " = 0x%X\n", a)

#define ARRAYDUMPC(a,length) for(unsigned int xi=0; xi<(unsigned int)(length); xi++){printf(#a"[%d] = %c\n", xi, a[xi]);}
#define ARRAYDUMPS(a,length) for(unsigned int xi=0; xi<(unsigned int)(length); xi++){printf(#a"[%d] = %s\n", xi, a[xi]);}
#define ARRAYDUMPD(a,length) for(unsigned int xi=0; xi<(unsigned int)(length); xi++){printf(#a"[%d] = %d\n", xi, a[xi]);}
#define ARRAYDUMPU(a,length) for(unsigned int xi=0; xi<(unsigned int)(length); xi++){printf(#a"[%d] = %u\n", xi, a[xi]);}
#define ARRAYDUMPX(a,length) for(unsigned int xi=0; xi<(unsigned int)(length); xi++){printf(#a"[%d] = 0x%X\n", xi, a[xi]);}
#define ARRAYDUMPX2(a,length) for(unsigned int xi=0; xi<(unsigned int)(length); xi++){printf("%02X ", (u32)(a[xi]));}printf("\n");

#define PRINT_NEXT_LINE printf("\n");
#define PRINT_FUNC(a)  printf("%s",__func__);if(a){PRINT_NEXT_LINE}
#define PRINT_LINE(a)  printf("%d",__LINE__);if(a){PRINT_NEXT_LINE}
#define PRINT_FILE(a)  printf("%s",__FILE__);if(a){PRINT_NEXT_LINE}
//#define PRINT_FUNC(1) PRINT_FUNC PRINT_NEXT_LINE
//#define PRINT_LINE(1) PRINT_LINE PRINT_NEXT_LINE
//#define PRINT_FILEn PRINT_FILE PRINT_NEXT_LINE

#ifndef ENABLE_DBG_MSG
#define ENABLE_DBG_MSG (1)
#endif

#ifndef ENABLE_ERR_MSG
#define ENABLE_ERR_MSG (1)
#endif

#if (ENABLE_DBG_MSG)
#define DBG_MSG(...) printf(##__VA_ARGS__)
#else
#define DBG_MSG(...)
#endif

#if (ENABLE_ERR_MSG)
#define DBG_ERR(...) printf(##__VA_ARGS__)
#else
#define DBG_ERR(...)
#endif

/* To close debug message (RE-DEFINE) */
/*
#ifdef ENABLE_DBG_MSG
#undef ENABLE_DBG_MSG
#undef DBG_MSG
#define ENABLE_DBG_MSG (1)
	#if (ENABLE_DBG_MSG)
	#define DBG_MSG(...) printf(##__VA_ARGS__)
	#else
	#define DBG_MSG(...)
	#endif
#endif
*/

#define _MY_MACROFUNCS_H_INCLUDED_
#endif//_MY_MACROFUNCS_H_INCLUDED_

