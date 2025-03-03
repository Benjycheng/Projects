module Clock(input clk, input mode, inc, dec, output [7:0]secs, mins, hours, output [3:0] flash);
wire [1:0] state;
wire plus_pulse, minus_pulse;

Button_control con(.clk(clk), .plus(inc), .minus(dec), .plus_out(plus_pulse), .minus_out(minus_pulse), .changemode(mode), .mode(state));

Flash_control f(.clk(clk), .in(mode), .out(flash[3:0]));

Time t(.clk(clk), .plus_pulse(plus_pulse), .minus_pulse(minus_pulse), .secs(secs), .mins(mins), .hours(hours), .mode(state));



endmodule
