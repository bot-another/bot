`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/06 19:07:18
// Design Name: 
// Module Name: Forwarding_Unit
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


module Forwarding_Unit(
    input EX_MEM_RegWrite,
    input MEM_WB_RegWrite,
    input [4:0] ID_EX_RegRs1,
    input [4:0] ID_EX_RegRs2,
    input [4:0] EX_MEM_RegRd,
    input [4:0] MEM_WB_RegRd,
    output [1:0]ForwardA,
    output [1:0]ForwardB
    );
reg [1:0]Reg_ForwardA;
reg [1:0]Reg_ForwardB;

always@(*)
begin
    if(EX_MEM_RegWrite && (EX_MEM_RegRd != 0)
      && (EX_MEM_RegRd == ID_EX_RegRs1) )
       Reg_ForwardA <= 1;
    else if(MEM_WB_RegWrite && (MEM_WB_RegRd != 0)
      && (MEM_WB_RegRd == ID_EX_RegRs1)&&~(EX_MEM_RegWrite && (EX_MEM_RegRd!=0)&&(EX_MEM_RegRd==ID_EX_RegRs1)) )
       Reg_ForwardA <= 2;
    else 
       Reg_ForwardA <= 0;
end

always@(*)
begin
     if(EX_MEM_RegWrite && (EX_MEM_RegRd != 0)
      && (EX_MEM_RegRd == ID_EX_RegRs2) )
       Reg_ForwardB <= 1;
     else if(MEM_WB_RegWrite && (MEM_WB_RegRd != 0)
      && (MEM_WB_RegRd == ID_EX_RegRs2)&&~(EX_MEM_RegWrite && (EX_MEM_RegRd!=0)&&(EX_MEM_RegRd==ID_EX_RegRs2)) )
       Reg_ForwardB <= 2;
     else 
       Reg_ForwardB <= 0;
end

assign ForwardA = Reg_ForwardA;
assign ForwardB = Reg_ForwardB;
endmodule
