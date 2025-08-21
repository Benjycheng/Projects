module Traffic_light(input CLOCK_50, input [3:0]KEY, input [9:0]SW, output [6:0]HEX0, HEX1, output [9:0] LEDR);


wire [7:0] cnt;

	Time #(.MAX(10))
	 t(.clk(CLOCK_50), .minus_pulse(KEY[0]), .secs(cnt));
	
	Display 
		d(.num0(cnt), .HEX0(HEX0), .HEX1(HEX1));

	endmodule
	
	
	

	
	
	
	
	
	
	
	
	
	
	
