module Time(input clk, plus_pulse, minus_pulse, input [1:0] mode, output [7:0] secs, mins ,hours);
	localparam N = 50_000_000;
	localparam BW = $clog2(N);
	wire [BW - 1: 0]tick;
	

	Counter #(.MAX(N - 1), .WIDTH(BW))
		divider(.clk(clk), .enable(1'b1), .cnt(tick));
		
	Counter #(.MAX(59), .WIDTH(BW), .UP(1))
		cs(.clk(clk), .enable(tick == 0 && mode == 2'b00), .plus(plus_pulse && mode == 2'b01), .minus(minus_pulse && mode == 2'b01), .cnt(secs));
		
	Counter #(.MAX(59), .WIDTH(BW), .UP(1))
		cm(.clk(clk), .enable(secs == 59 && tick == 0 && mode == 2'b00), .plus(plus_pulse && mode == 2'b10), .minus(minus_pulse && mode == 2'b10), .cnt(mins));
		
	Counter #(.MAX(23), .WIDTH(BW), .UP(1))
		ch(.clk(clk), .enable(secs == 59 && mins == 59 && tick == 0 && mode == 2'b00), .plus(plus_pulse && mode == 2'b11), .minus(minus_pulse && mode == 2'b11), .cnt(hours));
	
endmodule
	
	
	
	
module Counter 
	#(parameter MAX = 1, WIDTH = 1, UP = 1)(
	input clk,
	input reset,
	input pause,
	input enable,
	input plus,
	input minus,
	output reg [WIDTH - 1: 0] cnt
	);
	
	always @(posedge clk) begin
		if(reset) cnt <= 1'd0;
		if(pause) cnt <= cnt;
		else if(plus && !minus) cnt <= (cnt == MAX) ? 1'd0 : (cnt + 1'd1);
		else if(!plus && minus) cnt <= (cnt == 1'd0) ? MAX : (cnt - 1'd1);
		else if(plus && minus) cnt <= cnt;
		else if (enable && UP) cnt <= (cnt == MAX) ? 1'd0 : (cnt +1'd1);
		else if (enable && !UP) cnt <= (cnt == 1'd0) ? MAX : (cnt - 1'd1);
		
	end
endmodule
