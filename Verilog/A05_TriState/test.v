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
mux_tri(OUT1, OUT2, OUT_TRI, IN1, IN2, IN_SELECT);  //<- semicolon !!

   input   IN1, IN2, IN_SELECT;
   output  OUT1, OUT2, OUT_TRI;
   tri     OUT_TRI;
   
   bufif1(OUT1, IN1, IN_SELECT);
   bufif1(OUT_TRI, IN1, IN_SELECT);
   bufif0(OUT2, IN2, IN_SELECT);
   bufif0(OUT_TRI, IN2, IN_SELECT);

endmodule  //<- NO semicolon !!