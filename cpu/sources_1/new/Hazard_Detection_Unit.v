`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/05 23:11:12
// Design Name: 
// Module Name: Hazard_Detection_Unit
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


module Hazard_Detection_Unit(
    input [4:0] IF_ID_RegRs1,
    input [4:0] IF_ID_RegRs2,
    input [4:0] ID_EX_RegRd,
    input ID_EX_MemRead,
    output reg stop,rst
    );
    always@(*)
    begin
    if(ID_EX_MemRead&&((IF_ID_RegRs1!=0&&IF_ID_RegRs1==ID_EX_RegRd)||(IF_ID_RegRs2!=0&&IF_ID_RegRs2==ID_EX_RegRd)))
    begin
    stop=1'b1;
    rst=1'b1;
    end
    else
    begin
    stop=1'b0;
    rst=1'b0;
    end
    end
    
endmodule
