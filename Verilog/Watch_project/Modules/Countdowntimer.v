module Countdowntimer(input clk, inc, reset, pause, output [7:0] secs, mins, hours);
	localparam N = 50_000_000;
	localparam BW = $clog2(N);
	wire plus_pulse;
	wire reset_pulse;
	wire pause_pulse;

	wire [ BW - 1 : 0] tick;

	
	Counter #(.MAX(N - 1), .WIDTH(BW))
		divider(.clk(clk), .enable(1'b1), .cnt(tick));
		
	cdt_Counter #(.MAX(59), .WIDTH(6), .UP(0))
		cs(.clk(clk), .enable(tick == 0), .plus(plus_pulse), .pause(pause_pulse), .reset(reset_pulse), .cnt(secs));
		
	cdt_Counter #(.MAX(59), .WIDTH(6), .UP(0))
		cm(.clk(clk), .enable(secs == 59 && tick == 0), .pause(pause_pulse), .reset(reset_pulse), .cnt(mins));
		
	cdt_Counter #(.MAX(23), .WIDTH(6), .UP(0))
		ch(.clk(clk), .enable(mins == 59 && secs == 59 && tick == 0), .pause(pause_pulse), .reset(reset_pulse), .cnt(hours));
		
	Fast_advance #(.LONG(5_000_000))
		pplus(.clk(clk), .in(inc), .out(plus_pulse));
		
	RisingEdgeDetector r(.clk(clk), .in(reset), .out(reset_pulse));
	
	Toggle t(.clk(clk), .in(pause), .out(pause_pulse));
		
	
endmodule 



module cdt_Counter 
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
		else if (enable && !UP) cnt <= (cnt == 1'd0) ? 1'd0 : (cnt - 1'd1);
		
	end
endmodule
