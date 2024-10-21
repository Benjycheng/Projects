module Flash_control(input clk,  in, output [3:0] out); 

wire flash;
wire [1:0] mode;

DeMUX4
	dm(.in(flash),.sel(mode), .out(out));

Flash #(.MAX(25_000_000))
	f(.clk(clk), .out(flash));

Mode 
	m(.clk(clk), .in(in), .mode(mode));
	

	
endmodule