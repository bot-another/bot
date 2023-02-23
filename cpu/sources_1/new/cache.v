`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/24 20:14:49
// Design Name: 
// Module Name: cache
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


module cache(
input clk,
input rst,
//cpu<->cache
input [7:0]cpu_req_addr,
input cpu_req_rw,
input cpu_req_valid,
input [31:0]cpu_data_write,
output reg [31:0]cpu_data_read,
output reg cpu_ready,
//cache<->memory
output reg [7:0]mem_req_addr,
output reg mem_req_rw,
output reg mem_req_valid,
output reg [127:0]mem_data_write,
input [127:0]mem_data_read,
input mem_ready
);

parameter V = 133;
parameter D = 132;
parameter TagMSB = 131;
parameter TagLSB = 128;
parameter BlockMSB = 127;
parameter BlockLSB = 0 ;

parameter IDLE=0;
parameter CompareTag=1;
parameter Allocate=2;
parameter WriteBack=3;
parameter RST=4;


reg [133:0] cache_data[0:3];           //133:V,132:D,[131:128]:TAG,[127:0]DATA
reg [2:0]state,next_state;
wire hit;

wire [1:0]cpu_req_index;
wire [3:0]cpu_req_tag;
wire [1:0]cpu_req_offset;

assign cpu_req_offset=cpu_req_addr[1:0];
assign cpu_req_index=cpu_req_addr[3:2];
assign cpu_req_tag=cpu_req_addr[7:4];

integer i;
//初始化cache
initial
begin
    for(i=0;i<3;i=i+1)
        cache_data[i]=0;
end

always@(posedge clk,posedge rst)
if(rst)
begin
    state<=RST;
end
else
    state<=next_state;
//
always@(*)
case(state)
    RST:next_state=IDLE;
    IDLE:if(cpu_req_valid)
            next_state=CompareTag;
         else
            next_state=IDLE;
    CompareTag:if(hit)
                   next_state=IDLE;
               else if(cache_data[cpu_req_index][V:D]==2'b11)               //if the block is valid and dirty then go to WriteBack
                   next_state=WriteBack;
               else 
                   next_state=Allocate;
    Allocate:if(mem_ready)
                   next_state=CompareTag;
             else
                   next_state=Allocate;
    WriteBack:if(mem_ready)
                   next_state=Allocate;
              else
                   next_state=WriteBack;
      default:next_state=IDLE;
endcase

//always@(*)
//if(state==CompareTag)
//    if(cache_data[cpu_req_index][V]&&cache_data[cpu_req_index][TagMSB:TagLSB]==cpu_req_tag)
//        hit=1'b1;
//    else
//        hit=1'b0;
//else
//    hit=1'b0;
assign hit = cache_data[cpu_req_index][V]&(cache_data[cpu_req_index][TagMSB:TagLSB]==cpu_req_tag)? 1:0;
always@(posedge clk)
if(state==RST)
    for(i=0;i<3;i=i+1)
        cache_data[i]<=0; 
else if(state==IDLE)
    begin
    cpu_ready<=1'b0;
    mem_req_valid=1'b0;
    end
else if(state==CompareTag)
    if(hit)
        if(cpu_req_rw==1'b0)              //read hit
        begin
            cpu_ready<=1'b1;
            cpu_data_read<=cache_data[cpu_req_index][cpu_req_offset*32 +:32];
        end
        else                               //write hit,置脏位为1
        begin
            cpu_ready<=1'b1;
            cache_data[cpu_req_index][cpu_req_offset*32 +:32]=cpu_data_write;
            cache_data[cpu_req_index][D]=1'b1;
        end
    else
        cpu_ready<=1'b0;   
else if(state==Allocate)                 //read new block from memory to cache
    if(!mem_ready)
    begin
        mem_req_addr<={cpu_req_addr[7:2],2'b00};
        mem_req_rw<=1'b0;
        mem_req_valid<=1'b1; 
    end
    else
    begin
        mem_req_valid<=1'b0;
        cache_data[cpu_req_index][BlockMSB:BlockLSB]<=mem_data_read;
        cache_data[cpu_req_index][V:D]<=2'b10;
        cache_data[cpu_req_index][TagMSB:TagLSB]<=cpu_req_tag;
    end
else if(state==WriteBack)                          //write dirty block to memory
    if(!mem_ready)
    begin
        mem_req_addr<={cache_data[cpu_req_index][TagMSB:TagLSB],cpu_req_index,2'b00};
        mem_req_rw<=1'b1;
        mem_data_write<=cache_data[cpu_req_index][BlockMSB:BlockLSB];
        mem_req_valid<=1'b1;
    end
    else
    begin
        mem_req_valid<=1'b0;
    end
else
begin
    cpu_ready<=1'b0;
    mem_req_valid=1'b0;
end
endmodule