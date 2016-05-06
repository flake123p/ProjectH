//Time unit: 1ns, check every 10ps
`timescale 1ns/10ps
module simple_tb;

	reg   In1, In2;
	wire  [1:0]O1;
	wire  [1:0]O2;
	wire  [1:0]O3;

	array_order_test M1(O1, O2, O3, In1, In2); // Instance name required (M1)

	initial
		// Block statement (Execute order: left to right, top to down)
		begin
			$dumpfile("simple.vcd");
			$dumpvars(0, M1);

			#50 In1 = 1'b0; In2 = 1'b0;
			$display("time = %0d, In1=%b, In2=%b, O1=%b, O2=%b, O3=%b", $time, In1, In2, O1, O2, O3);
			#50 In1 = 1'b0; In2 = 1'b1;
			$display("time = %0d, In1=%b, In2=%b, O1=%b, O2=%b, O3=%b", $time, In1, In2, O1, O2, O3);
			#50 In1 = 1'b1; In2 = 1'b0;
			$display("time = %0d, In1=%b, In2=%b, O1=%b, O2=%b, O3=%b", $time, In1, In2, O1, O2, O3);
			#50 In1 = 1'b1; In2 = 1'b1;
			$display("time = %0d, In1=%b, In2=%b, O1=%b, O2=%b, O3=%b", $time, In1, In2, O1, O2, O3);
		end

	// Total time
	initial #500 $finish;
endmodule