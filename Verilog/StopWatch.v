module StopWatch (
	input clk, reset, startStop,
	output [5:0] mins, secs,
	output [6:0] hundredths
	);
	localparam N = 500_000;
	localparam BW = $clog2(N);
	wire [BW - 1: 0]tick;
	wire pause_pulse, reset_pulse;
	
	Counter #(.MAX(N - 1), .WIDTH(BW))
		divider(.clk(clk), .enable(1'b1), .cnt(tick));
		
	Counter #(.MAX(99), .WIDTH(7), .UP(1))
		cs(.clk(clk), .enable(tick == 0), .reset(reset_pulse), .pause(pause_pulse), .cnt(hundredths));
		
	Counter #(.MAX(59), .WIDTH(6), .UP(1))
		cm(.clk(clk), .enable(hundredths == 99 && tick == 0), .reset(reset_pulse), .pause(pause_pulse), .cnt(secs));
		
	Counter #(.MAX(59), .WIDTH(6), .UP(1))
		ch(.clk(clk), .enable(secs == 59 && hundredths == 99 && tick == 0), .reset(reset_pulse), .pause(pause_pulse), .cnt(mins));
		
	Toggle t(.clk(clk), .in(startStop), .out(pause_pulse));
	
	RisingEdgeDetector r(.clk(clk), .in(reset), .out(reset_pulse));
	
	
	
endmodule 
