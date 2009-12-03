`include "lcd.v"

module main();

reg clk;

initial begin
clk = 0;
$init_lcd(480,272);
#1;
$plot(240,136,255,255,255);
$plot(120,120,255,0,0);
forever begin
#130560; 
$redraw;
end
end

  
endmodule
