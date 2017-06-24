
#include <stdio.h>

#include "LibErrorCorrect.h"

int LibErrorCorrect_BlockCode_31RepetitiveCode(int inputBit, int outputBuf[])
{
  switch(inputBit)
  {
    case 0:
      outputBuf[0] = 0;
      outputBuf[1] = 0;
      outputBuf[2] = 0;
      break;

    case 1:
      outputBuf[0] = 1;
      outputBuf[1] = 1;
      outputBuf[2] = 1;
      break;

    default:
      break;
  }

  return 0;
}

int LibErrorCorrect_ConvolutionalCode_211(int inputBit, int outputBuf[])
{
  static int inputM0 = 0;
  
  outputBuf[0] = inputBit;
  outputBuf[1] = inputM0 ^ inputBit;
  inputM0 = inputBit;

  return 0;
}

void LibErrorCorrect_DemoBasic_01(void)
{
  size_t i;
  int inputBuf[] = {1,0,1,1,1};
  int outputBuf[2];

  for (i=0; i<sizeof(inputBuf)/sizeof(int); i++) {
    LibErrorCorrect_ConvolutionalCode_211(inputBuf[i], outputBuf);
    printf("%d%d, ", outputBuf[0], outputBuf[1]);
  }
  printf("\n");
}