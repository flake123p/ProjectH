 /*
Module:
	Switch Level
	Gate Level
	Data Level
	Behavioral Level(Algorithmic)
	
	Data Level + Behavioral Level => RTL (Register transfer level)
*/

// UPD, User Defined Primitives
primitive UDP_MY_XOR(OUT1, IN1, IN2); //<- semicolon !!, Output must be at first!!
	output OUT1;
	input IN1, IN2;
	// Truth table
	table
	//	IN1	IN2	:	OUT1
		0	0	:	0; //<- semicolon !!
		0	1	:	1; //<- semicolon !!
		1	0	:	1; //<- semicolon !!
		1	1	:	0; //<- semicolon !!
	endtable
endprimitive //<- NO semicolon !!

module simple(IN1, IN2, OUT_XOR);  //<- semicolon !!

   input   IN1, IN2;
   output  OUT_XOR;

   UDP_MY_XOR(OUT_XOR, IN1, IN2); //<- semicolon !!
   
endmodule  //<- NO semicolon !!