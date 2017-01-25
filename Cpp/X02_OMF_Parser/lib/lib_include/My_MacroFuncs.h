


#ifndef _MY_MACROFUNCS_H_INCLUDED_



#define USE_MY_ASSERT (1)
#if USE_MY_ASSERT
#include <stdio.h>
#define BASIC_ASSERT(a) if(a){;}else{printf(" Assertion failed: in %s(), line %d\n",__FUNCTION__,__LINE__);while(1){;}}
#else
#include <assert.h>
#define BASIC_ASSERT assert
#endif

#define DUMPS(a) printf(#a " = %s\n", a)
#define DUMPD(a) printf(#a " = %d\n", a)
#define DUMPU(a) printf(#a " = %u\n", a)
#define DUMPX(a) printf(#a " = 0x%X\n", a)

#define ARRAYDUMPS(a,length) for(unsigned int xi=0; xi<(unsigned int)length; xi++){printf(#a"[%d] = %s\n", xi, a[xi]);}
#define ARRAYDUMPD(a,length) for(unsigned int xi=0; xi<(unsigned int)length; xi++){printf(#a"[%d] = %d\n", xi, a[xi]);}
#define ARRAYDUMPU(a,length) for(unsigned int xi=0; xi<(unsigned int)length; xi++){printf(#a"[%d] = %u\n", xi, a[xi]);}
#define ARRAYDUMPX(a,length) for(unsigned int xi=0; xi<(unsigned int)length; xi++){printf(#a"[%d] = 0x%X\n", xi, a[xi]);}

#define _MY_MACROFUNCS_H_INCLUDED_
#endif//_MY_MACROFUNCS_H_INCLUDED_

