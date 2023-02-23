`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/06 22:40:05
// Design Name: 
// Module Name: sim
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/02 12:28:45
// Design Name: 
// Module Name: no_hazard_test
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module sim();
reg run,step,valid,rst,clk;
reg [4:0] in;
wire [1:0] check;
wire [4:0]out0;
wire [3:0]seg;
wire [2:0]an;
wire ready;
top top_test(
    .run(run), .step(step), .valid(valid), .rst(rst), .clk(clk),
    .in(in), .check(check), .out0(out0), .seg(seg),.an(an)
    );
parameter CYCLE = 1.0;

//initial begin
//    in = 4;
//    #80 in = 0;
//    #1 in = 1;
//    #1 in = 2;
//end

initial  begin
  clk = 0; 
   forever
      #(CYCLE/2)  clk = ~ clk; 
end
integer i;
initial begin
in=0;
run=0;
valid=0;
#5 in=5'b00100;
#1 run=1;
#300 run=0;
#1 in=0;
#2 valid=1;
#2 valid=0;
#2 valid=1;
#2 in=1;
#2 in=0;
for(i=0;i<200;i=i+1)
begin
#2 in=1;
#2 in=0;
end
end

initial begin
    rst = 0;
    #1 rst = 1;
    #1 rst = 0;
end

initial begin
    step = 0;
end

initial begin
    #2000 $finish;
end

endmodule

