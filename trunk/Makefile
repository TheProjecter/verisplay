all: 
	gcc -c -fpic lcd_vpi.c
	gcc -shared -o lcd_vpi.vpi lcd_vpi.o -lglut -lvpi
	iverilog -olcd_tb_fast.vvp lcd_tb_fast.v
	iverilog -olcd_tb_slow.vvp lcd_tb_slow.v

run_fast:
	vvp -M. -mlcd_vpi lcd_tb_fast.vvp
	
run_slow:
	vvp -M. -mlcd_vpi lcd_tb_slow.vvp
