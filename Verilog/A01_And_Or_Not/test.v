/*
Module:
	Switch Level
	Gate Level
	Data Level
	Behavioral Level(Algorithmic)
	
	Data Level + Behavioral Level => RTL (Register transfer level)
*/

// Port List = Parameters. Input at beginning, output at ending.
module simple(IN1, IN2, OUT_AND, OUT_OR, OUT_NOT_IN1);

   input   IN1, IN2;
   output  OUT_AND, OUT_OR, OUT_NOT_IN1;

   and G1(OUT_AND, IN1, IN2);
   or  G2(OUT_OR,  IN1, IN2);
   not G3(OUT_NOT_IN1, IN1);
   
endmodule