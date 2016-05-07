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
array_order_test(OUT1, OUT2, OUT3, IN1, IN2);  //<- semicolon !!

   input   IN1, IN2;
   output  [0:1]OUT1; //Display order (%b) is [1:0]
   output  [1:0]OUT2;
   output  [1:0]OUT3;
   
   assign OUT1[0] = IN1,
          OUT1[1] = IN2,
		  OUT2[0] = IN1,
		  OUT2[1] = IN2,
		  OUT3 = {IN1, IN2};

endmodule  //<- NO semicolon !!