//Time unit: 1ns, check every 10ps
`timescale 1ns/10ps
module simple_tb;

	reg   In1, In2, Select;
	wire  O1, O2, O_Tri;

	mux_tri M1(O1, O2, O_Tri, In1, In2, Select); // Instance name required (M1)

	initial
		// Block statement (Execute order: left to right, top to down)
		begin
			$dumpfile("simple.vcd");
			$dumpvars(0, M1);

			#50 Select = 1'b0; In1 = 1'b0; In2 = 1'b0;
			#50 Select = 1'b0; In1 = 1'b0; In2 = 1'b1;
			#50 Select = 1'b0; In1 = 1'b1; In2 = 1'b0;
			#50 Select = 1'b0; In1 = 1'b1; In2 = 1'b1;
			#50 Select = 1'b1; In1 = 1'b0; In2 = 1'b0;
			#50 Select = 1'b1; In1 = 1'b0; In2 = 1'b1;
			#50 Select = 1'b1; In1 = 1'b1; In2 = 1'b0;
			#50 Select = 1'b1; In1 = 1'b1; In2 = 1'b1;
		end

	// Total time
	initial #500 $finish;
endmodule