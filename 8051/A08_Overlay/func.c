/*

[[[ THIS EXAMPLE STILL HAS A BUG! ]]]

Background:
Two independant functions use big local array variable, use overlaying to save memory usage.

(1)
if ARRAY_SIZE == 100, will cause build error: --> Overlay Failed!!!!!
--------------------------------------------------------------
*** ERROR L107: ADDRESS SPACE OVERFLOW
    SPACE:   DATA    
    SEGMENT: ?DT?SUB_FUNC2?FUNC
    LENGTH:  0064H
--------------------------------------------------------------

(2)
if ARRAY_SIZE == 50, will build correctly. But waste memory space!! --> Overlay Failed!!!!!

*/

#define ARRAY_SIZE (50)


#include <REG52.H>

unsigned char gVar = 0;

void sub_func1(void)
{
  unsigned char ary[ARRAY_SIZE];
  unsigned char i;
  
  for (i=0; i<ARRAY_SIZE; i++) {
    ary[i] = i + 1;
  }
  
  for (i=0; i<ARRAY_SIZE; i++) {
    gVar += ary[i];
  }
}

void sub_func2(void)
{
  unsigned char ary[ARRAY_SIZE];
  unsigned char i;
  
  for (i=0; i<ARRAY_SIZE; i++) {
    ary[i] = 255 - i;
  }
  
  for (i=0; i<ARRAY_SIZE; i++) {
    gVar += ary[i];
  }
}

unsigned char func(void)
{
  sub_func1();
  sub_func2();
  
  return gVar;
}
