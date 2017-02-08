


#ifndef _MY_MACROFUNCS_H_INCLUDED_



#define USE_MY_ASSERT (1)
#if USE_MY_ASSERT
#include <stdio.h>
#define BASIC_ASSERT(a) if(a){;}else{printf(" Assertion failed: in %s(), line %d\n",__FUNCTION__,__LINE__);while(1){;}}
#else
#include <assert.h>
#define BASIC_ASSERT assert
#endif

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

#define PRINT_NEXT_LINE printf("\n");
#define PRINT_FUNC  printf("%s",__func__);
#define PRINT_LINE  printf("%d",__LINE__);
#define PRINT_FILE  printf("%s",__FILE__);
#define PRINT_FUNCx PRINT_FUNC PRINT_NEXT_LINE
#define PRINT_LINEx PRINT_LINE PRINT_NEXT_LINE
#define PRINT_FILEx PRINT_FILE PRINT_NEXT_LINE

#define _MY_MACROFUNCS_H_INCLUDED_
#endif//_MY_MACROFUNCS_H_INCLUDED_

