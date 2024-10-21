module Flash #(parameter MAX = 1)(
	input clk, 
	output out);
	
	localparam BW = $clog2(MAX);
	wire [BW - 1: 0] counter;
	
	Counter #(.MAX(MAX), .WIDTH(BW))
		f(.clk(clk), .enable(1), .cnt(counter));
		
	
	assign out = (counter < 5_000_000);


endmodule

