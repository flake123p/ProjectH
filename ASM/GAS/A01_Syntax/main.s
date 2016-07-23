/* =======================================================================
	The lines beginning with periods, like ".file", ".def", or ".ascii" 
		are assembler directives -- commands that tell the assembler how 
		to assemble the file.

	The lines beginning with some text followed by a colon, like "_main:", 
		are labels, or named locations in the code. 

	The other lines are assembly instructions.

	The ".file" and ".def" directives are for debugging. 
	We can leave them out: 
======================================================================= */

	.file	"main.c"
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
	
LC0:
	.ascii "Hello CLLL\0"
/* =======================================================================
	((( LC0: )))
	This code declares a label, then places some raw ASCII text into 
		the program, starting at the label's location.               '
	
	The "\12" specifies a line-feed character, while the "\0" specifies 
		a null character at the end of the string; 
	
	C routines mark the end of strings with null characters, and since
		we are going to call a C string routine, we need this character 
		here. 
	
	(NOTE! String in C is an array of datatype Char (Char[]) and does 
		not exist in any other form, but because one would understand 
		strings as a single entity from the majority of programming 
		languages, it is clearer to express it this way).
======================================================================= */
	
	.text
	/* ===================================================================
		((( .text )))
		This line declares the start of a section of code. 

		You can name sections using this directive, which gives you 
			fine-grained control over where in the executable the 
			resulting machine code goes, which is useful in some cases, 
			like for programming embedded systems.

		Using ".text" by itself tells the assembler that the following 
			code goes in the default section, which is sufficient for 
			most purposes.
	=================================================================== */

	.globl	_main
	/* ===================================================================
		((( .globl _main )))
		This line tells the assembler that the label "_main" is a global 
			label, which allows other parts of the program to see it. 

		In this case, the linker needs to be able to see the "_main" label, 
			since the startup code with which the program is linked calls 
			"_main" as a subroutine.
	=================================================================== */

	.def	_main;	.scl	2;	.type	32;	.endef

_main:
/* =======================================================================
	((( _main: )))
	This line declares the "_main" label, marking the place that is called 
		from the startup code.
======================================================================= */
LFB10:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$16, %esp
	call	___main
	movl	$LC0, (%esp)
	call	_puts
	movl	$22, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE10:
	.ident	"GCC: (GNU) 4.8.1"
	.def	_puts;	.scl	2;	.type	32;	.endef
