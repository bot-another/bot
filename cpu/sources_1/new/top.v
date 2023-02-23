`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/14 19:17:15
// Design Name: 
// Module Name: top
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


module top(
input run,step,valid,rst,clk,
input[4:0] in,
output[1:0]check,
output[4:0]out0,
output ready,
output [2:0]an,
output [3:0]seg
    );
    wire clk_cpu;
    //Debug_BUS
    wire[7:0]m_rf_addr;
    wire[31:0]rf_data;
    wire[31:0]m_data;
    //IO_BUS
    wire[7:0] io_addr;
    wire[31:0] io_dout;
    wire io_we;
    wire[31:0] io_din;
    //ต๗สิ
    wire[31:0] pcin, pc, pcd, pce;
    wire[31:0] ir, imm, mdr;
    wire[31:0] a, b, y, bm, yw;
    wire[4:0]  rd, rdm, rdw;
    wire[31:0] ctrl, ctrlm, ctrlw;
    PDU_PL top_pdu(clk,rst,run,step,clk_cpu,valid,in,check,out0,an,seg,ready,io_addr,io_dout,io_we,io_din,m_rf_addr,rf_data,m_data,
    pcin,pc,pcd,pce,ir,imm,mdr,a,b,y,bm,yw,rd,rdm,rdw,ctrl,ctrlm,ctrlw);
    cpu_pl top_cpu(clk_cpu,rst,io_addr,io_dout,io_we,io_din,m_rf_addr,rf_data,m_data,pc,pcd,ir,pcin,
    pce,a,b,imm,rd,ctrl,y,bm,rdm,ctrlm,yw,mdr,rdw,ctrlw);
endmodule
