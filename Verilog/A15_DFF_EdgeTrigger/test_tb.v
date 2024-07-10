//Time unit: 1ns, check every 10ps
`timescale 1ns/10ps
module simple_tb;

	reg   inD, inClk;
	wire  outQ;

	dff_edge_trigger M1(.outQ(outQ), .inD(inD), .inClk(inClk)); // Instance name required (M1)

	initial
		// Block statement (Execute order: left to right, top to down)
		begin
			inClk = 1'b0;
			$printtimescale;
			$dumpfile("simple.vcd");
			$dumpvars(0, M1);
			{inD} = 1'b0;
			repeat(27) begin
				#6 inD = ~inD;
			end
		end

	initial begin
		$monitor("time = %3d, inD=%b, inClk=%b, outQ=%b", $time, inD, inClk, outQ);
	end
	
	// Total time
	initial 
		begin
			#300 $finish;
		end
	
	always
		begin
			#20 inClk = ~inClk;
		end
endmodule