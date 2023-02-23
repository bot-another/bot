module mux2#(parameter width = 32)
(
    input [width-1:0]a,b,
    input sel,
    output [width-1:0]c
);
assign c = sel==0? a:b;
endmodule