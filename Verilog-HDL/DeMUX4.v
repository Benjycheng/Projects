module DeMUX4(input [1:0] sel, input in, output reg [3:0] out);	
	always @(*) begin
	out = 4'b0000;
	out[sel] = in;
	end
endmodule