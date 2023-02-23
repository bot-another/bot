`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/04 20:26:55
// Design Name: 
// Module Name: SegmentRegister
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


module SegmentRegister(
input[223:0]in,
input clk,rst,wr,
output[31:0]ctrl,pc,pc4,a,b,ir,imm
    );
reg[223:0]out;
initial out=0;
always@(posedge clk)
begin
    if(rst)out<=0;
    else if(!wr)out<=in;
    else out<=out;
end
assign imm=out[31:0];
assign ir=out[63:32];
assign b=out[95:64];
assign a=out[127:96];
assign pc4=out[159:128];
assign pc=out[191:160];
assign ctrl=out[223:192];
endmodule
