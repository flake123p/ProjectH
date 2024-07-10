/*
	asm ( assembler template 
		: output operands             (optional) 
		: input operands              (optional) 
		: list of clobbered registers (optional) 
		);
	
//
// gcc.gnu.org ============= https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html =============================
//
asm [volatile] ( AssemblerTemplate
				  : OutputOperands
				  [ : InputOperands
				  [ : Clobbers ] ])
 
asm [volatile] goto ( AssemblerTemplate
					   :
					   : InputOperands
					   : Clobbers
					   : GotoLabels)

Keyword: asm or __asm__
	The asm keyword is a GNU extension. 
	When writing code that can be compiled with -ansi and the various -std options, 
	use __asm__ instead of asm.

Qualifiers:
	volatile
		The typical use of extended asm statements is to manipulate input values to produce output values. 
		However, your asm statements may also produce side effects. 
		If so, you may need to use the volatile qualifier to disable certain optimizations.
	goto
		This qualifier informs the compiler that the asm statement may perform a jump to one of the labels listed in the GotoLabels. 

Parameters:
	AssemblerTemplate
		This is a literal string that is the template for the assembler code.
		It is a combination of fixed text and tokens that refer to the input, output, and goto parameters.
		//KEY POINT
		Multi-line: use "\n\t", e.g.:
			asm ("mov %1, %0\n\t"
				 "add $1, %0"
				 : "=r" (dst)
				 : "r" (src));
		
	OutputOperands
		A comma-separated list of the C variables modified by the instructions in the AssemblerTemplate.
		An empty list is permitted.
		
	InputOperands
		A comma-separated list of C expressions read by the instructions in the AssemblerTemplate.
		An empty list is permitted.
		
	Clobbers
		A comma-separated list of registers or other values changed by the AssemblerTemplate, beyond those listed as outputs.
		An empty list is permitted. 
		
	GotoLabels
		When you are using the goto form of asm, this section contains the list of all C labels to 
		which the code in the AssemblerTemplate may jump.
		
		asm statements may not perform jumps into other asm statements, only to the listed GotoLabels. 
		GCC's optimizers do not know about other jumps; therefore they cannot take account of them when deciding how to optimize.
	
	//KEY POINT
	The total number of input + output + goto operands is limited to 30.
	
Remarks
	Note that extended asm statements must be inside a function. 
	Only basic asm may be outside functions (see Basic Asm). 
	Functions declared with the naked attribute also require basic asm (see Function Attributes). //Flake:???

	
6.44.2.1 Volatile
	GCC's optimizers sometimes discard asm statements if they determine there is no need for the output variables. 
	Also, the optimizers may move code out of loops if they believe that the code will always return the same result 
	(i.e. none of its input values change between calls). 
	Using the volatile qualifier disables these optimizations. 
	asm statements that have no output operands, including asm goto statements, are implicitly volatile.

	GCC's optimizers do not treat this code like the non-volatile code in the earlier examples. 
	They do not move it out of loops or omit it on the assumption that the result from a previous call is still valid.

	Note that the compiler can move even volatile asm instructions relative to other code, including across jump instructions. 
	For example, on many targets there is a system register that controls the rounding mode of floating-point operations. 
	Setting it with a volatile asm, as in the following PowerPC example, does not work reliably.

		asm volatile("mtfsf 255, %0" : : "f" (fpenv));
		sum = x + y;
	 
	The compiler may move the addition back before the volatile asm. 
	To make it work as expected, add an artificial dependency to the asm by referencing a variable in the subsequent code, 
	for example:

		asm volatile ("mtfsf 255,%1" : "=X" (sum) : "f" (fpenv));
		sum = x + y;

	Under certain circumstances, GCC may duplicate (or remove duplicates of) your assembly code when optimizing. 
	This can lead to unexpected duplicate symbol errors during compilation if your asm code defines symbols or labels. 
	Using ‘%=’ (see AssemblerTemplate) may help resolve this problem.


6.44.2.2 Assembler Template
	A combination that works in most places is a newline to break the line, plus a tab character to move to the instruction field 
	(written as ‘\n\t’). Some assemblers allow semicolons as a line separator. 
	However, note that some assembler dialects use semicolons to start a comment.
	
	Do not expect a sequence of asm statements to remain perfectly consecutive after compilation, 
	even when you are using the volatile qualifier. 
	If certain instructions need to remain consecutive in the output, put them in a single multi-instruction asm statement.
	
	Accessing data from C programs without using input/output operands 
	(such as by using global symbols directly from the assembler template) may not work as expected. 
	Similarly, calling functions directly from an assembler template requires a detailed understanding of the target assembler and ABI.

	Since GCC does not parse the assembler template, it has no visibility of any symbols it references. 
	This may result in GCC discarding those symbols as unreferenced unless they are also listed as input, output, or goto operands.


Special format strings
	In addition to the tokens described by the input, output, and goto operands, 
	these tokens have special meanings in the assembler template:
		‘%%’
			Outputs a single ‘%’ into the assembler code. 
		‘%=’
			Outputs a number that is unique to each instance of the asm statement in the entire compilation. 
			This option is useful when creating local labels and referring to them multiple times in a single template that 
			generates multiple assembler instructions. 
		‘%{’
		‘%|’
		‘%}’
			Outputs ‘{’, ‘|’, and ‘}’ characters (respectively) into the assembler code. When unescaped, 
			these characters have special meaning to indicate multiple assembler dialects, as described below.


Multiple assembler dialects in asm templates

	--- Skip for now ---


6.44.2.3 Output Operands
	An asm statement has zero or more output operands indicating the names of C variables modified by the assembler code.

	In this i386 example, old (referred to in the template string as %0) and *Base (as %1) are outputs and Offset (%2) is an input:

		 bool old;
		 
		 __asm__ ("btsl %2,%1\n\t" // Turn on zero-based bit #Offset in Base.
				  "sbb %0,%0"      // Use the CF to calculate old.
			: "=r" (old), "+rm" (*Base)
			: "Ir" (Offset)
			: "cc");
		 
		 return old;

	Operands are separated by commas. Each operand has this format:

		 [ [asmSymbolicName] ] constraint (cvariablename)

		asmSymbolicName
			Specifies a symbolic name for the operand. 
			Reference the name in the assembler template by enclosing it in square brackets (i.e. ‘%[Value]’). 
			The scope of the name is the asm statement that contains the definition. 
			Any valid C variable name is acceptable, including names already defined in the surrounding code. 
			No two operands within the same asm statement can use the same symbolic name.
			
			When not using an asmSymbolicName, use the (zero-based) position of the operand in the list of 
				operands in the assembler template. 
			For example if there are three output operands, use ‘%0’ in the template to refer to the first, 
				‘%1’ for the second, and ‘%2’ for the third. 
			
		constraint
			A string constant specifying constraints on the placement of the operand; See Constraints, for details.

			Output constraints must begin with either ‘=’ (a variable overwriting an existing value) or ‘+’ (when reading and writing). 
			When using ‘=’, do not assume the location contains the existing value on entry to the asm, 
				except when the operand is tied to an input; see Input Operands.

			After the prefix, there must be one or more additional constraints (see Constraints) that describe where the value resides. 
			//KEY POINT
			Common constraints include ‘r’ for register and ‘m’ for memory. 
			When you list more than one possible location (for example, "=rm"), 
				the compiler chooses the most efficient one based on the current context. 
			If you list as many alternates as the asm statement allows, you permit the optimizers to produce the best possible code. 
			If you must use a specific register, 
				but your Machine Constraints do not provide sufficient control to select the specific register you want, 
				local register variables may provide a solution (see Local Register Variables). 
		
		cvariablename
			Specifies a C lvalue expression to hold the output, typically a variable name. 
			The enclosing parentheses are a required part of the syntax.
		
		When the compiler selects the registers to use to represent the output operands, 
			it does not use any of the clobbered registers (see Clobbers).

		Operands using the ‘+’ constraint modifier count as two operands (that is, both as input and output) 
			towards the total maximum of 30 operands per asm statement.
		
Out -- "=r", "=a", "=X",
In  -- "=", "f",
*/
#include <stdio.h>

void Copy_Src_To_Dst_And_Add_1_To_Dst(void)
{
	printf("\n--- %s() ---\n", __func__);

	int src = 12;
	int dst = 999;

	asm ("mov %1, %0\n\t"
	     "add $1, %0"
	     : "=r" (dst)
	     : "r" (src));

	printf("%d\n", dst);

}

void Read_Time_Stamp_Counter(void)
{
	printf("\n--- %s() ---\n", __func__);
	
    unsigned int myEax, myEdx;
     
    asm volatile ( "rdtsc\n"    // Returns the time in EDX:EAX.
             "movl %%eax, %0\n\t"
			 "movl %%edx, %1"
             : "=r" (myEax), "=r" (myEdx)
             :
             : "%eax", "%edx");
     
    printf("myEax: 0x%08X,  myEdx: 0x%08X\n", myEax, myEdx);
	 
	asm volatile ( "rdtsc\n"    // Returns the time in EDX:EAX.
             "movl %%eax, %0\n"
			 "movl %%edx, %1"
             : "=r" (myEax), "=r" (myEdx)
             :
             : "%eax", "%edx");
     
    printf("myEax: 0x%08X,  myEdx: 0x%08X\n", myEax, myEdx);
}

int main(int argc, char *argv[])
{
	Copy_Src_To_Dst_And_Add_1_To_Dst();
	Read_Time_Stamp_Counter();
	
	return 0;
}
