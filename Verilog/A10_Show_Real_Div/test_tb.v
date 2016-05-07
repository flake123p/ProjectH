//Time unit: 1ns, check every 10ps
`timescale 1ns/10ps
module simple_tb;

	real real_1 = 5 / 100;   //Integer Division
	real real_2 = 5.0 / 100; //Float Division

	initial
		// Block statement (Execute order: left to right, top to down)
		begin
			$display("real_1 = %f", real_1);
			$display("real_2 = %f", real_2);
		end

endmodule