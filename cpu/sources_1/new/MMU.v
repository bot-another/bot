`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/24 20:31:21
// Design Name: 
// Module Name: MMU
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


module MMU
#(
parameter WriteMiss=0,
parameter WriteHit=1,
parameter ReadHit=2,
parameter ReadMiss=3,
parameter WriteDirtyMiss=4,
parameter ReadDirtyMiss=5
)
(
input [7:0]a,
input [31:0]d,
input [7:0]dpra,
input clk,
input we,
output [31:0]spo,
output [31:0]dpo,
input re,
output stop,
input rst
    );

wire [31:0]cpu_data_read;
wire cpu_ready;
wire [7:0]mem_req_addr;
wire mem_req_rw;
wire mem_req_valid;
wire [127:0]mem_data_write;
wire [127:0]mem_data_read;
wire mem_ready;
wire [7:0]mem_req_addr2;
wire cpu_req_valid;
//reg vaild,vaild1;
//wire vaild_w,vaild1_w;
//assign vaild_w=vaild;
//assign vaild1_w=vaild1;
assign spo = cpu_data_read;
//always@(posedge clk)
//begin   
//    vaild<=re|we;
//    vaild1<=vaild;
//end
//assign cpu_req_valid = vaild_w&~vaild1_w;//取一个时钟周期
assign cpu_req_valid = (re|we)&~cpu_ready;
assign stop = ~cpu_ready&(re|we);
cache cache(.clk(clk),
.rst(rst),
.cpu_req_addr(a),
.cpu_req_rw(we),
.cpu_req_valid(cpu_req_valid),
.cpu_data_write(d),
.cpu_data_read(cpu_data_read),
.cpu_ready(cpu_ready),
//***********************//
.mem_req_addr(mem_req_addr),
.mem_req_rw(mem_req_rw),
.mem_req_valid(mem_req_valid),
.mem_data_write(mem_data_write),
.mem_data_read(mem_data_read),
.mem_ready(mem_ready));
mem mem(.clk(clk),
.rst(rst),
.mem_req_addr(mem_req_addr),
.mem_req_rw(mem_req_rw),
.mem_req_valid(mem_req_valid),
.mem_data_write(mem_data_write),
.mem_data_read(mem_data_read),
.mem_ready(mem_ready),
.mem_req_addr2(dpra),
.data(dpo));

endmodule
//    for(i=0;i<3;i=i+1)
//        cache_data[i]=133'd0;