module Toggle (input clk, in, output reg out);
	always @(posedge in)
		if (in) out <= !out;
endmodule