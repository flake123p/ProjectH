/*

1. Don't have to include "#pragma SRC"

2. Produce and assemble .SRC on IDE tool:
    a.[Project window] -> source file (e.g. "func.c")
    b.click mouse right
    c.[Options for file 'func.c'...]
    d.Recheck [Generate Assembler SRC File]
    e.Recheck [Assemble SRC File]

3. You can use "#pragma asm" and "#pragma endasm" now.

*/

#include <REG52.H>

unsigned char gVar = 0;

unsigned char func(void)
{
	return gVar;
}

void isr(void) interrupt 1 using 2
{
  gVar++; //Nothing will be pushed and poped!!!
}

void isr2(void) interrupt 2 using 2
{
  #pragma asm
  JMP $
  #pragma endasm
  B++; //B will be pushed and poped automatically!!!
}