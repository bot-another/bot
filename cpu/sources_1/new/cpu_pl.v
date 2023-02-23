`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/04 20:24:51
// Design Name: 
// Module Name: cpu_pl
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

module  cpu_pl (
  input clk, 
  input rst,

  //IO_BUS
  output [7:0] io_addr,      //led和seg的地址
  output [31:0] io_dout,     //输出led和seg的数据
  output io_we,                 //输出led和seg数据时的使能信号
  input [31:0] io_din,        //来自sw的输入数据

  //Debug_BUS
  input [7:0] m_rf_addr,   //存储器(MEM)或寄存器堆(RF)的调试读口地址
  output [31:0] rf_data,    //从RF读取的数据
  output [31:0] m_data,    //从MEM读取的数据

  //PC/IF/ID 流水段寄存器
  output [31:0] pc,
  output [31:0] pcd,
  output [31:0] ir,
  output [31:0] pcin,

  // ID/EX 流水段寄存器
  output [31:0] pce,
  output [31:0] a,
  output [31:0] b,
  output [31:0] imm,
  output [4:0] rd,
  output [31:0] ctrl,

  // EX/MEM 流水段寄存器
  output [31:0] y,
  output [31:0] bm,
  output [4:0] rdm,
  output [31:0] ctrlm,

  // MEM/WB 流水段寄存器
  output [31:0] yw,
  output [31:0] mdr,
  output [4:0] rdw,
  output [31:0] ctrlw
);
wire wait_stop;//cache_miss
//IF信号
wire [31:0]hit_nouse0,nouse1,nouse2,nouse3,nouse4,nouse5,nouse6;
wire [31:0]ir0,pc4;
wire [31:0]pcsrc;//beq or jump
reg [1:0]pcsel;//
wire stop,rst_IF_ID;/////////////
wire[31:0]pc4d;
//IF
wire [31:0]brpc_update,prepc_update,pcout,pcin0,pc_error0,pc_error1,pc_jalr;
wire is_br_jal;
wire really,hit;
BHT BHT_jump(clk,rst,pc,brpc_update,prepc_update,is_br_jal,really,pcout,hit);

assign brpc_update = pce;
assign prepc_update = pcsrc;
assign is_br_jal = ctrl[8]|ctrl[9]|ctrl[11];


//reg [3:0] im0[0:255];
//reg [3:0] im1[0:255];
//reg [3:0] im2[0:255];
//reg [3:0] im3[0:255];
//reg [3:0] im4[0:255];
//reg [3:0] im5[0:255];
//reg [3:0] im6[0:255];
//reg [3:0] im7[0:255];
//integer i;
//initial begin for(i=0;i<256;i=i+1)begin
// im0[i]=0;
// im1[i]=0; 
// im2[i]=0;
// im3[i]=0; 
// im4[i]=0;
// im5[i]=0; 
// im6[i]=0;
// im7[i]=0; 
// end
// end
//// initial begin $readmemh("D:/",im); end
//assign ir0={im0[pc[9:2]],im1[pc[9:2]],im2[pc[9:2]],im3[pc[9:2]],im4[pc[9:2]],im5[pc[9:2]],im6[pc[9:2]],im7[pc[9:2]]};
//wire rx_vld;
//wire [7:0] rx_data;
//wire [3:0] in_data;
//reg [7:0] in_addr;
//reg [2:0] position;
//initial in_addr=0;
//initial position=0;
//rx in_rx(.clk(rxclk),.rst(1'b0),.rx(rx),.rx_vld(rx_vld),.rx_data(rx_data));
//ascll_to_int ASCIItoINT(.asc(rx_data),.bin(in_data));
//always@(posedge rxclk)
//begin
//   if(rx_vld)
//   begin
//      case(position)
//      3'd0:im0[in_addr]<=in_data;
//      3'd1:im1[in_addr]<=in_data;
//      3'd2:im2[in_addr]<=in_data;
//      3'd3:im3[in_addr]<=in_data;
//      3'd4:im4[in_addr]<=in_data;
//      3'd5:im5[in_addr]<=in_data;
//      3'd6:im6[in_addr]<=in_data;
//      3'd7:im7[in_addr]<=in_data;
//      endcase
//      position<=position+1'b1;
//      if(position==3'd7)
//         in_addr<=in_addr+1'b1;
//   end
//end



mux2 pcmux(pc4,pcout,hit,pcin0);
mux4 pcmux1(pcin0,pc_error0,pc_error1,pc_jalr,pcsel,pcin);
PCregister PC(pcin,clk,rst,stop|wait_stop,pc);
add pcadd(pc,32'b0100,pc4);
I_mem Imem(pc[9:2],32'b0,clk,1'b0,ir0);
SegmentRegister IF_ID({hit,31'b0,pc,pc4,32'b0,32'b0,ir0,32'b0},clk,rst_IF_ID,stop|wait_stop,hit_nouse0,pcd,pc4d,nouse1,nouse2,ir,nouse3);


wire RegWrite,rst_ID_EX,rst_ID_EX0,rst_ID_EX1;
wire [31:0]wd,rd1,rd2,immd,ctrld,pc4e,ire;
register registerfile(clk,RegWrite,rdw,ir[19:15],ir[24:20],m_rf_addr[4:0],wd,rd1,rd2,rf_data);
Imm Immnumber(ir,immd);
control Control(hit_nouse0[31],ir,ctrld);
SegmentRegister ID_EX({ctrld,pcd,pc4d,rd1,rd2,ir,immd},clk,rst_ID_EX,wait_stop,ctrl,pce,pc4e,a,b,ire,imm);
assign rd=ire[11:7];
Hazard_Detection_Unit Hazard_Detection(ir[19:15],ir[24:20],ire[11:7],ctrl[13],stop,rst_ID_EX0);

wire [31:0]rs2_or_imm,alunum1,alunum2,aluresult,irm,irw,rs1_or_pc;
wire [1:0]afwd,bfwd;
wire zero,less;
wire [31:0]pc4m;
wire jalr;
mux2 rd2_imm(alunum2,imm,ctrl[4],rs2_or_imm);
add addar(pce,imm,pcsrc);
mux3 fwda(a,y,wd,afwd,alunum1);
mux3 fwdb(b,y,wd,bfwd,alunum2);
mux2 rs1_pc(alunum1,pce,ctrl[5],rs1_or_pc);
Forwarding_Unit forwarding_unit(ctrlm[18],ctrlw[18],ire[19:15],ire[24:20],irm[11:7],irw[11:7],afwd,bfwd);
alu ALU(rs1_or_pc,rs2_or_imm,ctrl[0],aluresult,zero,less);
assign rst_ID_EX1 = ((really&~ctrl[31])|(~really&ctrl[31])|jalr);
assign rst_ID_EX = (rst_ID_EX0|rst_ID_EX1)&(~wait_stop);//注意cachemiss,这里不能flush
assign rst_IF_ID = ((really&~ctrl[31])|(~really&ctrl[31])|jalr)&(~wait_stop);
assign jalr = ctrl[10];
assign pc_jalr = aluresult;
assign pc_error0 = pc4e;
assign pc_error1 = pcsrc;
always@(*)
begin
    if(jalr)
    pcsel<=2'b11;
    else if(~really&ctrl[31])
    pcsel<= 2'b01;
    else if(really&~ctrl[31])
    pcsel <= 2'b10;
    else
    pcsel <= 2'b00;
end
assign really = (zero&ctrl[8])|ctrl[9]|(ctrl[11]&less);
SegmentRegister EX_MEM({ctrl,32'b0,pc4e,aluresult,alunum2,ire,32'b0},clk,1'b0,wait_stop,ctrlm,nouse4,pc4m,y,bm,irm,nouse5);
assign rdm=irm[11:7];

wire [31:0]lw_data,pc4w,bmw;
wire mem_write;
assign mem_write=ctrlm[12]&~y[10];
MMU Dmmu(y[9:2],bm,m_rf_addr,clk,mem_write,lw_data,m_data,ctrlm[13]&~y[10],wait_stop,rst);
//D_mem Dmem(y[9:2],bm,m_rf_addr,clk,mem_write,lw_data,m_data);
SegmentRegister MEM_WB({ctrlm,bm,pc4m,lw_data,y,irm,32'b0},clk,wait_stop,1'b0,ctrlw,bmw,pc4w,mdr,yw,irw,nouse6);
assign rdw=irw[11:7];

wire [31:0]wrdata;
mux2 mdr_or_io(mdr,io_din,yw[10],wrdata);
mux3 rfwd(pc4w,wrdata,yw,ctrlw[17:16],wd);
assign RegWrite=ctrlw[18];
assign io_addr=yw[7:0];
assign io_dout=bmw;
assign io_we=yw[10]&ctrlw[12];
endmodule
