module BtD(input [7:0] in, output [4:0] num1, num2);
	assign num1 = in / 10;
	assign num2 = in % 10;
endmodule
	
