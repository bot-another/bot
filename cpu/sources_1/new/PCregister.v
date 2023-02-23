`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/14 16:08:41
// Design Name: 
// Module Name: PCregister
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


module PCregister(
input[31:0]a,
input clk,rst,wr,
output [31:0]b
    );
reg[31:0]PC;
always@(posedge clk,posedge rst)
begin
    if(rst)PC<=32'b0000_0000_0000_0000_0011_0000_0000_0000;
    else if(!wr)PC<=a;
end
assign b=PC;
endmodule
