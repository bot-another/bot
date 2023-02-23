`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/24 20:15:41
// Design Name: 
// Module Name: mem
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



module mem(
input clk,
input rst,
input [7:0]mem_req_addr,                        //[1:0]块内地址
input mem_req_rw,
input mem_req_valid,
input [127:0]mem_data_write,
output reg [127:0]mem_data_read,
output reg mem_ready,
input [7:0]mem_req_addr2,
output [31:0]data
    );

reg [31:0] mem [0:255];                 //256个字，64个块
assign data = mem[mem_req_addr2];
integer i;
initial
begin
    for(i=0;i<256;i=i+1)
        mem[i]=32'd0;
end

always@(posedge clk,posedge rst)
if(rst)begin
    mem_ready<=1'b0;
    mem_data_read<=0;
end
else if(mem_req_valid&&mem_req_rw==1'b1)                      //write
begin
    mem[mem_req_addr+3]<=mem_data_write[127:96];
    mem[mem_req_addr+2]<=mem_data_write[95:64];
    mem[mem_req_addr+1]<=mem_data_write[63:32];
    mem[mem_req_addr]<=mem_data_write[31:0];
    mem_ready<=1'b1;
end
else if(mem_req_valid&&mem_req_rw==1'b0)                      //read
begin
    mem_data_read<={mem[mem_req_addr+3],mem[mem_req_addr+2],mem[mem_req_addr+1],mem[mem_req_addr]};
    mem_ready<=1'b1;
end
else 
    mem_ready<=1'b0;

endmodule
