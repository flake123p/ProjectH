 /*
Module:
	Switch Level
	Gate Level
	Data Level
	Behavioral Level(Algorithmic)
	
	Data Level + Behavioral Level => RTL (Register transfer level)
*/

// Circuit with boolean expressions
module simple(IN1, IN2, OUT_AND, OUT_OR, OUT_NOT_IN1);

   input   IN1, IN2;
   output  OUT_AND, OUT_OR, OUT_NOT_IN1;

   // implicit combinational logic
   assign OUT_AND  = IN1 && IN2;
   assign OUT_OR   = IN1 || IN2;
   assign OUT_NOT_IN1 = !IN1;
   
endmodule