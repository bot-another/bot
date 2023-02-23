module mux3#(parameter width = 32)
(
    input [width-1:0]a,b,c,
    input [1:0]sel,
    output reg[width-1:0]d
);
always@(*)
begin
    if(sel==2'b00)d=a;
    else if(sel==2'b01)d=b;
    else if(sel==2'b10)d=c;
    else d=0;
end
endmodule