/*
Module:
	Switch Level
	Gate Level
	Data Level
	Behavioral Level(Algorithmic)
	
	Data Level + Behavioral Level => RTL (Register transfer level)
*/

// Port List = Parameters. Input at beginning, output at ending.
module simple(IN1, IN2, OUT_OR);

   input   IN1, IN2;
   output  OUT_OR;

   or  #(20)G1(OUT_OR,  IN1, IN2);
   
endmodule