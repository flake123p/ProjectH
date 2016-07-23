/*
	asm ( assembler template 
		: output operands             (optional) 
		: input operands              (optional) 
		: list of clobbered registers (optional) 
		);
		
	[]Register name：
		Add ”%” before register name.
		* AT&T：%eax
		* Intel：eax
		%0, %1, %2 ... represent the following registers.

	[]Source/Destination ordering：
		src=eax, dst=ebx
		* AT&T:  movl %eax, %ebx
		* Intel: mov  ebx,  eax
	
	[]Operand size:
		b for byte -- movb
		w for word -- movw
		l for long -- movl

	[]Constant/immediate value format: (func_1 func_2)
		Add "$" before ...
		* AT&T：movl $boo, %eax
		* Intel：mov eax, boo
		* Load the address of boo into eax, boo must be static。
*/
#include <stdio.h>

void Simplest_Example_Mov_Constant_To_Variable(void)
{
	printf("\n--- %s() ---\n", __func__);
	
	int x = 0x3366;
	
	// Constant start with "$" -- $0x1F
	__asm__( "movl $0x1F, %0"
	       : "=r" (x)
		   );

	printf("x = 0x%X\n", x);
}

void Load_Address_Of_Static_Variable(void)
{
	printf("\n--- %s() ---\n", __func__);

	static int y __asm__ ("y") = 10; //Flake: 這行在做什麼也沒講
	int z;

    __asm__( "movl $y, %0"
           : "=r" (z));
	
	printf("%%p of y = 0x%p, z = 0x%x, *z = %d\n", &y, z, *((int *)z));
}

void Array_Access(void)
{
	printf("\n--- %s() ---\n", __func__);
	
	int i; 
	static int ary[2][3] __asm__("ary") = {
		{0,  1,  2}, 
		{10, 11, 12} 
	}; 	
	
	// i = ary[1][1] 
	__asm__("movl $12, %%eax;\n" // cal how many size of one row 
	        "movl $8, %%ebx;\n"  // which column 8 = offset 2 x sizeof int
	        "movl ary(%%ebx, %%eax, 1), %0;\n" 
	        : "=r" (i) 
	        : 
	        : "%eax", "%ebx");
			
	printf("i=%d\n", i);
}

int main(int argc, char *argv[])
{
	Simplest_Example_Mov_Constant_To_Variable();
	Load_Address_Of_Static_Variable();
	Array_Access();
	
	return 0;
}
