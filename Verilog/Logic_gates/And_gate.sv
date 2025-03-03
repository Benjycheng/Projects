module and_gate_tb;
  
reg a, b;
wire c;
  
and_gate uut(a,b,c);
  
initial begin
a = 0; b = 0;
#10
  $display("%d %d %d",a ,b, c);
b = 0; b = 1;
#10
  $display("%d %d %d",a ,b, c);
a = 1; b = 0;
#10
  $display("%d %d %d",a ,b, c);
b = 1; b = 1;
#10
  $display("%d %d %d",a ,b, c);
$finish();
end
  
endmodule
