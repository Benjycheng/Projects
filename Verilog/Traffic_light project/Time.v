module Time
	#(parameter MAX = 1)(
	input clk, minus_pulse, output [7:0] secs);
	localparam N = 50_000_000;
	localparam BW = $clog2(N);
	wire [BW - 1: 0]tick;
	

	Counter #(.MAX(N - 1), .WIDTH(BW))
		divider(.clk(clk), .enable(1'b1), .cnt(tick));
		
	 cdt_Counter  #(.MAX(MAX), .WIDTH(BW), .UP(0))
		cs(.clk(clk), .enable(tick == 0), .minus(minus_pulse), .cnt(secs));
		

	
endmodule
	
	
	
	
module Counter 
	#(parameter MAX = 1, WIDTH = 1, UP = 1)(
	input clk,
	input enable,
	output reg [WIDTH - 1: 0] cnt
	);
	
	always @(posedge clk) begin
		if (enable && UP) cnt <= (cnt == MAX) ? 1'd0 : (cnt +1'd1);
		else if (enable && !UP) cnt <= (cnt == 1'd0) ? MAX : (cnt - 1'd1);
		
	end
endmodule
 
 module cdt_Counter 
	#(parameter MAX = 1, WIDTH = 1, UP = 1)(
	input clk,
	input enable,
	input minus,
	output reg [WIDTH - 1: 0] cnt
	);
	
	always @(posedge clk) begin
		if(!minus && cnt == 1'd0) cnt <= MAX;
		else if (enable) cnt <= (cnt == 1'd0) ? 1'd0: (cnt - 1'd1);
	
		
	end
endmodule