module LCD(x,y,r,g,b);

input[9:0] x;
input[9:0] y;
input[7:0] r;
input[7:0] g;
input[7:0] b;

wire[9:0] x;
wire[9:0] y;
wire[7:0] r;
wire[7:0] g;
wire[7:0] b;

initial
begin

forever begin
	#5 $plot(x,y,r,g,b);
end

end
  	  
endmodule
