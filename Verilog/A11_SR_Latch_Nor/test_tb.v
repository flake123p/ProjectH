//Time unit: 1ns, check every 10ps
`timescale 1ns/10ps
module simple_tb;

	reg   InS, InR;
	wire  OutQ, OutQc;

	sr_latch_nor M1(.Q(OutQ), .Qc(OutQc), .S(InS), .R(InR)); // Instance name required (M1)

	initial
		// Block statement (Execute order: left to right, top to down)
		begin
			$dumpfile("simple.vcd");
			$dumpvars(0, M1);

			#50 {InR, InS} = 2'b00;
			
			#50 {InR, InS} = 2'b01; $display("Set...");
			#50 {InR, InS} = 2'b00;
			
			#50 {InR, InS} = 2'b10; $display("Reset...");
			#50 {InR, InS} = 2'b00;
			
			#50 {InR, InS} = 2'b01; $display("Set + Reset (Skip 0,0 case)");
			#50 {InR, InS} = 2'b10;
			
			#50 {InR, InS} = 2'b11; $display("Set 1,1 ==> if set 0,0 later, iverilog will crash!!");
			#50 {InR, InS} = 2'b01;
		end

	initial begin
		$monitor("time = %3d, InS=%b, InR=%b, OutQ=%b, OutQc=%b", $time, InS, InR, OutQ, OutQc);
	end
	// Total time
	initial #500 $finish;
endmodule