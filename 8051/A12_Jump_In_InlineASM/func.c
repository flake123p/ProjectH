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
  #pragma asm
  NOP
	NOP
	NOP
	NOP
	NOP
  JMP $-2
  #pragma endasm
		
	return gVar;
}
