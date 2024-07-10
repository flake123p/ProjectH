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
sr_latch_nor(Q, Qc, S, R);  //<- semicolon !!

   input   S, R;
   output  Q, Qc;
   
   // Do AND on an array = Do AND on every bit!!!!!!!!!!!!!!!!
   assign Q  = ~(S & Qc);
   assign Qc = ~(R & Q);

endmodule  //<- NO semicolon !!