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
mux_2to1_beh(OUT, IN1, IN2, IN3);  //<- semicolon !!

   input   IN1, IN2, IN3;
   output  OUT;
   wire    [1:0]temp;
   
   // Do AND on an array = Do AND on every bit!!!!!!!!!!!!!!!!
   assign temp = {IN1, IN2},
          OUT = &{temp[1:0], IN3};

endmodule  //<- NO semicolon !!