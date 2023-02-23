`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/24 12:56:09
// Design Name: 
// Module Name: mux4
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


module mux4#(parameter width = 32)
(
    input [width-1:0]a,b,c,d,
    input [1:0]sel,
    output reg[width-1:0]e
);
always@(*)
begin
    if(sel==2'b00)e=a;
    else if(sel==2'b01)e=b;
    else if(sel==2'b10)e=c;
    else e=d;
end
endmodule
