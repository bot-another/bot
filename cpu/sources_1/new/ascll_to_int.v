`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/25 09:42:42
// Design Name: 
// Module Name: ascll_to_int
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


module ascll_to_int(
input [7:0] asc,
output reg [3:0] bin);
always@(*)
begin
   case(asc)
   48:bin=0;
   49:bin=1;
   50:bin=2;
   51:bin=3;
   52:bin=4;
   53:bin=5;
   54:bin=6;
   55:bin=7;
   56:bin=8;
   57:bin=9;
   97:bin=10;
   98:bin=11;
   99:bin=12;
   100:bin=13;
   101:bin=14;
   102:bin=15;
   default:bin=15;
   endcase
end
endmodule
