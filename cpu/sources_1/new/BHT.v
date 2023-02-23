`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/24 12:47:47
// Design Name: 
// Module Name: BHT
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


module BHT(
input clk,
input rst,
input [31:0] pc,
input [31:0] brpc_update,
input [31:0] prepc_update,
input is_br_jal,
input really,
output reg [31:0] pcout,
output hit);
reg [1:0] ptr;
reg [31:0] brpc[0:3];
reg [31:0] prepc[0:3];
reg [1:0] state[0:3];
integer i;
initial
begin
   ptr=0;
   for(i=0;i<4;i=i+1)
   begin
      brpc[i]=0;
      prepc[i]=0;
      state[i]=0;
   end
end
assign hit=((pc==brpc[0])&state[0][1])|((pc==brpc[1])&state[1][1])|((pc==brpc[2])&state[2][1])|((pc==brpc[3])&state[3][1]);
always@(*)
begin
   if((pc==brpc[0]) && state[0][1])
      pcout=prepc[0];
   else if((pc==brpc[1]) && state[1][1])
      pcout=prepc[1];
   else if((pc==brpc[2]) && state[2][1])
      pcout=prepc[2];
   else if((pc==brpc[3]) && state[3][1])
      pcout=prepc[3];
   else
      pcout=pc;
end
always@(posedge clk or posedge rst)
begin
   if(rst)
   begin
      ptr<=0;
      for(i=0;i<4;i=i+1)
      begin
         brpc[i]<=0;
         prepc[i]<=0;
         state[i]<=0;
      end
   end
   else if(is_br_jal)
   begin
      if(really)
      begin
         if(brpc_update==brpc[0])
         begin
            case(state[0])
            2'b00:state[0]<=2'b01;
            2'b01:state[0]<=2'b11;
            2'b10:state[0]<=2'b11;
            2'b11:state[0]<=2'b11;
            endcase
         end
         else if(brpc_update==brpc[1])
         begin
            case(state[1])
            2'b00:state[1]<=2'b01;
            2'b01:state[1]<=2'b11;
            2'b10:state[1]<=2'b11;
            2'b11:state[1]<=2'b11;
            endcase
         end
         else if(brpc_update==brpc[2])
         begin
            case(state[2])
            2'b00:state[2]<=2'b01;
            2'b01:state[2]<=2'b11;
            2'b10:state[2]<=2'b11;
            2'b11:state[2]<=2'b11;
            endcase
         end
         else if(brpc_update==brpc[3])
         begin
            case(state[3])
            2'b00:state[3]<=2'b01;
            2'b01:state[3]<=2'b11;
            2'b10:state[3]<=2'b11;
            2'b11:state[3]<=2'b11;
            endcase
         end
         else
         begin
            brpc[ptr]<=brpc_update;
            prepc[ptr]<=prepc_update;
            state[ptr]<=2'b01;
            ptr<=ptr+2'b1;
         end
      end
      else
      begin
         if(brpc_update==brpc[0])
         begin
            case(state[0])
            2'b00:state[0]<=2'b00;
            2'b01:state[0]<=2'b00;
            2'b10:state[0]<=2'b00;
            2'b11:state[0]<=2'b10;
            endcase
         end
         else if(brpc_update==brpc[1])
         begin
            case(state[1])
            2'b00:state[1]<=2'b00;
            2'b01:state[1]<=2'b00;
            2'b10:state[1]<=2'b00;
            2'b11:state[1]<=2'b10;
            endcase
         end
         else if(brpc_update==brpc[2])
         begin
            case(state[2])
            2'b00:state[2]<=2'b00;
            2'b01:state[2]<=2'b00;
            2'b10:state[2]<=2'b00;
            2'b11:state[2]<=2'b10;
            endcase
         end
         else if(brpc_update==brpc[3])
         begin
            case(state[3])
            2'b00:state[3]<=2'b00;
            2'b01:state[3]<=2'b00;
            2'b10:state[3]<=2'b00;
            2'b11:state[3]<=2'b10;
            endcase
         end
      end
   end
end
endmodule
