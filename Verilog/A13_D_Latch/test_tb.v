//Time unit: 1ns, check every 10ps
`timescale 1ns/10ps
module simple_tb;

	reg   InD, InEn;
	wire  OutQ, OutQc;

	d_latch M1(.Q(OutQ), .Qc(OutQc), .D(InD), .En(InEn)); // Instance name required (M1)

	initial
		// Block statement (Execute order: left to right, top to down)
		begin
			$dumpfile("simple.vcd");
			$dumpvars(0, M1);

			#50 {InD, InEn} = 2'b00;
			#50 {InD, InEn} = 2'b10; $display("Modify D when disabled");
			#50 {InD, InEn} = 2'b11; $display("Enable with set");
			#50 {InD, InEn} = 2'b01; $display("Enable with reset");
			#50 {InD, InEn} = 2'b10; $display("Change to set when disabled");
		end

	initial begin
		$monitor("time = %3d, InD=%b, InEn=%b, OutQ=%b, OutQc=%b", $time, InD, InEn, OutQ, OutQc);
	end
	// Total time
	initial #500 $finish;
endmodule