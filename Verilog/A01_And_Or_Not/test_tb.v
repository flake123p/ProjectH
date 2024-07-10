//Time unit: 1ns, check every 10ps
`timescale 1ns/10ps
module simple_tb;

   reg   A, B;

   wire  O1, O2, O3;

   initial
     begin
        $dumpfile("simple.vcd");
        $dumpvars(0, s);
        $monitor("A = %b, B = %b | O1 = %b O2 = %b O3 = %b ", A, B, O1, O2, O3);
        #50 A = 1'b0; B=1'b0;
        #50 A = 1'b0; B=1'b1;
        #50 A = 1'b1; B=1'b0;
        #50 A = 1'b1; B=1'b1;
        #50 $finish;
     end

   simple s(A, B, O1, O2, O3);

endmodule