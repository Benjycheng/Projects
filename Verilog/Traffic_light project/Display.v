module Display( 
	input [7:0] num2, num1, num0,
	output [6:0] HEX5, HEX4, HEX3, HEX2, HEX1, HEX0
	);
	
	wire [4:0] secs_1, secs_2, mins_1, mins_2, hours_1, hours_2;

	BtD sec(.in(num0), .num1(secs_1), .num2(secs_2));
	BtD mins(.in(num1), .num1(mins_1), .num2(mins_2));
	BtD hours(.in(num2), .num1(hours_1), .num2(hours_2));

	SSeg ss0(.digit(secs_1), .SSEG(HEX1));
	SSeg ss1(.digit(secs_2), .SSEG(HEX0));
	SSeg ss2(.digit(mins_1), .SSEG(HEX3));
	SSeg ss3(.digit(mins_2), .SSEG(HEX2));
	SSeg ss4(.digit(hours_1), .SSEG(HEX5));
	SSeg ss5(.digit(hours_2), .SSEG(HEX4));
		
endmodule 