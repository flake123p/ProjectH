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
mux_2to1_beh(OUT, IN1, IN2, Select);  //<- semicolon !!

   input   IN1, IN2, Select;
   output  OUT;
   reg     OUT; //Behavioral modeling output must be type: reg
   
   //always @ (IN1, IN2, Select)  //SAME
   always @ (IN1 or IN2 or Select)
     if(Select)
	   OUT = IN1;
     else
	   OUT = IN2;

endmodule  //<- NO semicolon !!