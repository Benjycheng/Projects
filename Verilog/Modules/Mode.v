module Mode (
    input clk,                
    input in,           
    output reg [1:0] mode,
	 output [9:7] LEDR
);

// State encoding
parameter NORMAL = 2'b00;
parameter SECONDS = 2'b01;
parameter MINUTES = 2'b10;
parameter HOURS = 2'b11;

reg [1:0] state = 2'b00;

// State transition logic
always @(posedge in) begin
        case (state) 
            NORMAL: state <= SECONDS;

            SECONDS: state <= MINUTES;

            MINUTES: state <= HOURS;

            HOURS: state <= NORMAL;
        endcase
end


always @(*) begin
	case(state)
		NORMAL: mode = 2'b00;
		SECONDS: mode = 2'b01;
		MINUTES: mode = 2'b10;
		HOURS: mode = 2'b11;
		default: mode = 2'b00;
	endcase
end
endmodule
