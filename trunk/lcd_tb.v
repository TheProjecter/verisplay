`include "lcd.v"

module lcd_testbench();


reg[9:0] x;
reg[9:0] y;
reg[7:0] r;
reg[7:0] g;
reg[7:0] b;

initial begin
	$init_lcd(480,272,1);
	x=0;
	y=0;
	r=255;
	g=255;
	b=255;
	forever begin
		#10;
		
		if(x==479) begin
			x=0;
			y=y+1;
		end
		else x=x+1;	
		if(y==272) y=0;
		
		if( (x%48<=24) && (y%48<=24) ) begin
			r=150;
			g=0;
			b=0;
		end
		else if( (x%48>=24) && (y%48<=24) ) begin
			r=0;
			g=0;
			b=0;
		end
		else if( (x%48<=24) && (y%48>=24) ) begin
			r=0;
			g=0;
			b=0;
		end
		else if( (x%48>=24) && (y%48>=24) ) begin
			r=150;
			g=0;
			b=0;
		end
		
	
	end
end
  	
LCD lcd(x,y,r,g,b);
  
endmodule
