`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/14 17:24:10
// Design Name: 
// Module Name: control
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


module control
#(parameter 
ADDI=7'b0010011,
ADD=7'b0110011,
JAL=7'b1101111,
BEQ=7'b1100011,
LW=7'b0000011,
SW=7'b0100011,
JALR=7'b1100111,
AUIPC=7'b0010111
)
(
input hit,
input [31:0]in,
output[31:0]ctrl
    );
reg MemWrite, MemRead, RegWrite;
reg ALUop; //只有beq是减 1
reg [1:0] RegSrc;//写回选择00PC4,01memr,10y写回寄存器
reg ALUSrc;//立即数 //0选择rd2,1选择imm
reg jal,beq,blt,jalr,ALUSrc0;
wire [6:0] opcode;
wire [2:0] funct3;
wire [6:0] funct7;
assign opcode=in[6:0];
assign funct3=in[14:12];
assign funct7=in[31:25];
always@(*)
begin
    case(in[6:0])
    ADDI:begin 
    MemWrite <= 0;
    MemRead <= 0; 
    RegWrite <= 1; 
    RegSrc <= 2;
    ALUSrc0 <= 0;
    ALUSrc <= 1;
    ALUop <= 0;
    jal <= 0;
    beq <= 0;
    blt <= 0;
    jalr <= 0;
    end
    ADD:begin 
    MemWrite <= 0;
    MemRead <= 0; 
    RegWrite <= 1; 
    RegSrc <= 2; 
    ALUSrc0 <= 0;
    ALUSrc <= 0;
    ALUop <= funct7==0? 0:1;
    jal <= 0;
    beq <= 0;
    blt <= 0;
    jalr <= 0;
    end
    JAL:begin
    MemWrite <= 0;
    MemRead <= 0;
    RegWrite <= 1;
    RegSrc <= 0; 
    ALUSrc0 <= 0;
    ALUSrc <= 1;
    ALUop <= 0;
    jal <= 1;
    beq <= 0;
    blt <= 0;
    jalr <= 0;
    end
    JALR:begin
    MemWrite <= 0;
    MemRead <= 0;
    RegWrite <= 1;
    RegSrc <= 0; 
    ALUSrc0 <= 0;
    ALUSrc <= 1;
    ALUop <= 0;
    jal <= 0;
    beq <= 0;
    blt <= 0;
    jalr <= 1;
    end
    BEQ:begin
    MemWrite <= 0;
    MemRead <= 0;
    RegWrite <= 0;
    RegSrc <= 0; 
    ALUSrc0 <= 0;
    ALUSrc <= 0;
    ALUop <= 1;
    jal <= 0;
    beq <= funct3==0? 1:0;
    blt <= funct3==0? 0:1;
    jalr <= 0;
    end
    LW:begin
    MemWrite <= 0;
    MemRead <= 1;
    RegWrite <= 1;
    RegSrc <= 1; 
    ALUSrc0 <= 0;
    ALUSrc <= 1;
    ALUop <= 0;
    jal <= 0;
    beq <= 0;
    blt <= 0;
    jalr <= 0;
    end
    SW:begin
    MemWrite <= 1;
    MemRead <= 0;
    RegWrite <= 0;
    RegSrc <= 0; 
    ALUSrc0 <= 0;
    ALUSrc <= 1;
    ALUop <= 0;
    jal <= 0;
    beq <= 0;
    blt <= 0;
    jalr <= 0;
    end
    AUIPC:begin
    MemWrite <= 0;
    MemRead <= 0; 
    RegWrite <= 1; 
    RegSrc <= 2;
    ALUSrc0 <= 1;
    ALUSrc <= 1;
    ALUop <= 0;
    jal <= 0;
    beq <= 0;
    blt <= 0;
    jalr <= 0;
    end
    default:begin 
    MemWrite <= 0;
    MemRead <= 0;
    RegWrite <= 0;
    RegSrc <= 0; 
    ALUSrc0 <= 0;
    ALUSrc <= 0;
    ALUop <= 0;
    jal <= 0;
    beq <= 0;
    blt <= 0;
    jalr <= 0;
    end
    endcase
end
assign ctrl={hit,3'b0,4'b0,4'b0,1'b0,RegWrite,RegSrc,2'b0,MemRead,MemWrite,blt,jalr,jal,beq,2'b0,ALUSrc0,ALUSrc,3'b0,ALUop};
endmodule
