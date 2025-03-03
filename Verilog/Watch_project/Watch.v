module Watch(input CLOCK_50, input [3:0]KEY, input [9:0]SW, output [6:0]HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, output [9:0] LEDR);
wire [3:0] flash;
wire [7:0] cdt_num0, cdt_num1, cdt_num2;
wire [7:0] sw_num0, sw_num1, sw_num2;
wire[7:0] secs, mins, hours;
wire [7:0] display1, display2, display3;
wire [3:0] key0, key1, key2;
wire [1:0] mode;

Clock 
	c(.clk(CLOCK_50), .mode(key2[1]), .dec(key1[1]), .inc(key0[1]), .secs(secs), .mins(mins), .hours(hours), .flash(flash));

StopWatch (.clk(CLOCK_50), .reset(key1[2]), .startStop(key0[2]), .hundredths(sw_num0), .secs(sw_num1), .mins(sw_num2));

Countdowntimer
	cdt(.clk(CLOCK_50), .inc(key0[3]), .pause(key1[3]), .reset(key2[3]), .secs(cdt_num0), .mins(cdt_num1), .hours(cdt_num2));

Display
	d(.num2(display3), .num1(display2), .num0(display1), .blank(flash), .HEX0(HEX0), .HEX1(HEX1), .HEX2(HEX2), .HEX3(HEX3), .HEX4(HEX4), .HEX5(HEX5), .mode(mode));

mux4 #(.WIDTH(8))
	digit1(.a(), .b(secs), .c(sw_num0), .d(cdt_num0), .sel(mode), .out(display1));

mux4 #(.WIDTH(8))
	digit2(.a(), .b(mins), .c(sw_num1), .d(cdt_num1), .sel(mode), .out(display2));
	
mux4 #(.WIDTH(8))
	digit3(.a(), .b(hours), .c(sw_num2), .d(cdt_num2), .sel(mode), .out(display3));
	
Mode (.clk(CLOCK_50), .in(!KEY[3]), .mode(mode));
Mode (.clk(CLOCK_50), .in(!KEY[3]), .mode(LEDR[1:0]));
	
DeMUX4(.sel(mode), .in(!KEY[0]), .out(key0));
DeMUX4(.sel(mode), .in(!KEY[1]), .out(key1));
DeMUX4(.sel(mode), .in(!KEY[2]), .out(key2));
	

	
endmodule
