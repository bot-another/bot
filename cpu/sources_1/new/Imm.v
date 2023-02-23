`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/13 22:42:03
// Design Name: 
// Module Name: imm
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


module Imm
#(parameter 
ADDI=7'b0010011,
ADD=7'b0110011,
SUB=7'b0110011,
AUIPC=7'b0010111,
JAL=7'b1101111,
JALR=7'b1100111,
BEQ=7'b1100011,
BLT=7'b1100011,
LW=7'b0000011,
SW=7'b0100011)
(
input[31:0]in,
output reg[31:0]data
    );
always@(*)
begin
    case(in[6:0])
    ADDI:begin if(~in[31])data={20'b0,in[31:20]};else data={20'b1111_1111_1111_1111_1111,in[31:20]};end
    AUIPC:data={in[31:12],12'b0};
    JAL:begin if(~in[31])data={11'b0,in[31],in[19:12],in[20],in[30:21],1'b0};else data={11'b1111_1111_111,in[31],in[19:12],in[20],in[30:21],1'b0};end
    JALR:begin if(~in[31])data={20'b0,in[31:20]};else data={20'b1111_1111_1111_1111_1111,in[31:20]};end
    //BLT:begin if(~in[31])data={19'b0,in[31],in[7],in[30:25],in[11:8],1'b0};end
    BEQ:begin if(~in[31])data={19'b0,in[31],in[7],in[30:25],in[11:8],1'b0};else data={19'b1111_1111_1111_1111_111,in[31],in[7],in[30:25],in[11:8],1'b0};end 
    LW:begin if(~in[31])data={20'b0,in[31:20]};else data={20'b1111_1111_1111_1111_1111,in[31:20]};end
    SW:begin if(~in[31])data={20'b0,in[31:25],in[11:7]};else data={20'b1111_1111_1111_1111_1111,in[31:25],in[11:7]};end
    default:data=32'b0;
    endcase
end
endmodule
