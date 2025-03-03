module Fast_advance #(parameter LONG = 5)(
	input clk, 
	input in, 
	output out);

	localparam BWL = $clog2(LONG + 1);
	
	reg [BWL - 1 : 0] cl = 0, next_cl;

	always@ (posedge clk) begin
		cl <= next_cl;
	end
	
	always@ (*) begin
	if(in)begin
		if(cl == 0) begin
			next_cl = LONG;
		end
		else if(cl > 0) begin
			next_cl <= cl - 1;
		end
	end
	else
		next_cl = 0;
	end
		
	assign out = (cl == LONG);
endmodule


