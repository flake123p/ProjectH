//Time unit: 1ns, check every 10ps
`timescale 1ns/10ps
module simple_tb;

	reg   A, B;
	wire  O1;

	simple M1(A, B, O1); // Instance name required (M1)

	initial
		// Block statement (Execute order: left to right, top to down)
		begin
			$dumpfile("simple.vcd");
			$dumpvars(0, M1);
			$monitor("A = %b, B = %b | O1 = %b ", A, B, O1);
			A = 1'b0; B=1'b0;
			#50 A = 1'b0; B=1'b1;
		end

	// Total time is 80
	initial #80 $finish;
endmodule