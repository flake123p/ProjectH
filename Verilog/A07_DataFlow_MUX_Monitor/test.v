/*
Modeling:
	Switch Level [MOS simulation]
	Gate Level modeling [ primitive logic gate ]
	Dataflow   modeling [ assign ]
	Behavioral modeling [ always ] [ Sequential Circuit ]
	
	Dataflow + Behavioral => RTL (Register transfer level)
*/

/*
Basic Gates
    and, nand, or, nor, xor, xnor, not, buf
  Tri-state
    bufif0, bufif1, notif0, notif1
*/

module 
mux_2to1_df(OUT, IN1, IN2, Select);  //<- semicolon !!

   input   IN1, IN2, Select;
   output  OUT;
   
   assign OUT = Select ? IN1 : IN2;

endmodule  //<- NO semicolon !!