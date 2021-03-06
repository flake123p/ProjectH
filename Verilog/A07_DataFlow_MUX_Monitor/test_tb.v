//Time unit: 1ns, check every 10ps
`timescale 1ns/10ps
module simple_tb;

	reg   In1, In2, Select;
	wire  Out;

	mux_2to1_df M1(Out, In1, In2, Select); // Instance name required (M1)

	initial
		// Block statement (Execute order: left to right, top to down)
		begin
			$dumpfile("simple.vcd");
			$dumpvars(0, M1);

			#100 {Select, In1, In2} = 3'b000;
			repeat(7)
			begin
				#50 {Select, In1, In2} = {Select, In1, In2} + 1;
			end
		end

	initial begin
		$monitor("time = %0d, Select=%b, In1=%b, In2=%b, Out=%b", $time, Select, In1, In2, Out);
	end
	// Total time
	initial #500 $finish;
endmodule