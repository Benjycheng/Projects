module SSeg( input [3:0] digit, input enable, output reg [6:0] SSEG);

	always@(*) begin 
		if(enable) SSEG = 7'b1111111;
		
		else case(digit)	
			4'd0: SSEG = 7'b1000000;
			4'd1: SSEG = 7'b1111001;
			4'd2: SSEG = 7'b0100100;
			4'd3: SSEG = 7'b0110000;
			3'd4: SSEG = 7'b0011001;
			4'd5: SSEG = 7'b0010010;
			4'd6: SSEG = 7'b0000010;
			4'd7: SSEG = 7'b1111000;
			4'd8: SSEG = 7'b0000000;
			4'd9: SSEG = 7'b0010000;
			
			default: SSEG = 7'b1111111;
		endcase

		
	end
endmodule
