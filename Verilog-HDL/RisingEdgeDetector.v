module RisingEdgeDetector(input clk, input in, output out);
	reg prev = 0;
	wire next_prev;
	
	always @(posedge clk)
		prev <= next_prev;
		
	assign next_prev = in;
	
	assign out = (!prev && in);
endmodule 