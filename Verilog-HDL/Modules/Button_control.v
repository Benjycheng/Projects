module Button_control(input clk, plus, minus, changemode, output plus_out, minus_out, output [1:0]mode);

	Fast_advance #(.LONG(10_000_000))
		pp(.clk(clk), .in(plus), .out(plus_out));
		
	Fast_advance #(.LONG(10_000_000))
		mp(.clk(clk), .in(minus), .out(minus_out));
		
	Mode 
		m(.clk(clk), .in(changemode), .mode(mode));
		
endmodule
