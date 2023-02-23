module register
#(parameter WIDTH=32)
(
input clk,we,
input [4:0]wa,ra0,ra1,ra2,
input [WIDTH-1:0]wd,
output [WIDTH-1:0]rd0,rd1,rd2
    );
reg [WIDTH-1:0] regfile[0:31];
integer i;
    initial begin
      for(i = 0; i < 32; i = i + 1)
        regfile[i] = 0;
    end
assign rd0 = (we == 1)? ( (wa == ra0) ? ( (ra0 == 0) ? 0 : wd) : regfile[ra0] ) :  regfile[ra0];
assign rd1 = (we == 1)? ( (wa == ra1) ? ( (ra1 == 0) ? 0 : wd) : regfile[ra1] ) :  regfile[ra1];
assign rd2 = (we == 1)? ( (wa == ra2) ? ( (ra2 == 0) ? 0 : wd) : regfile[ra2] ) :  regfile[ra2];
always@(posedge  clk) 
begin
    if(we && wa != 0) regfile[wa] <= wd;
end
endmodule