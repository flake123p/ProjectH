/*
Modeling:
	Switch Level [MOS simulation]
	Gate Level modeling [ primitive logic gate ]
	Dataflow   modeling [ assign ]
	Behavioral modeling [ always ] [ Sequential Circuit ]
	
	Dataflow + Behavioral => RTL (Register transfer level)

	
Basic Gates (8 + 4)
    and, nand, or, nor, xor, xnor, not, buf
  Tri-state
    bufif0, bufif1, notif0, notif1
*/

module 
d_latch(Q, Qc, D, En);  //<- semicolon !!

   input   D, En;
   output  Q, Qc;
   wire    S, R;
   
   assign S  = ~(D & En);
   assign R  = ~(~D & En);
   assign Q  = ~(S & Qc);
   assign Qc = ~(R & Q);

endmodule  //<- NO semicolon !!