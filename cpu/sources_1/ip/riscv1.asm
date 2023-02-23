la s8 back
addi x2 zero 1
addi x3 zero 2
addi x4 zero 4
addi x5 zero 8
addi x6 zero 16
addi x7 zero 32
addi t6 zero 15
again:
lw x1 0x408(zero)
blt x0 x1 judge
jal x0 again
judge:
and x8 x2 x1
beq x8 x2 N1
and x8 x3 x1
beq x8 x3 blue1
jal x0 N3
N1:
and x8 x3 x1
beq x8 x3 N2
blue:
jal s10 color
lw x1 0x408(zero)
and x8 x2 x1
beq x8 zero again
beq t6 x10 blue
addi x10 x10 1
jal x0 blue
blue1:
jal s10 color
lw x1 0x408(zero)
and x8 x3 x1
beq x8 zero again
beq zero x10 blue1
addi x11 zero 1
sub x10 x10 x11
jal x0 blue1
N2:
lw x1 0x408(zero)
and x8 x2 x1
beq x8 zero again
lw x1 0x408(zero)
and x8 x3 x1
beq x8 zero again
jal x0 N2
N3:
and x8 x4 x1
beq x8 x4 N4
and x8 x5 x1
beq x8 x5 green1
jal x0 N6
N4:
and x8 x5 x1
beq x8 x5 N5
green:
jal s10 color
lw x1 0x408(zero)
and x8 x4 x1
beq x8 zero again
beq t6 x9 green
addi x9 x9 1
jal x0 green
green1:
jal s10 color
lw x1 0x408(zero)
and x8 x5 x1
beq x8 zero again
beq zero x9 green1
addi x11 zero 1
sub x9 x9 x11
jal x0 green1
N5:
lw x1 0x408(zero)
and x8 x4 x1
beq x8 zero again
lw x1 0x408(zero)
and x8 x5 x1
beq x8 zero again
jal x0 N5
N6:
and x8 x6 x1
beq x8 x6 N7
and x8 x7 x1
beq x8 x7 red1
jal x0 again
N7:
and x8 x7 x1
beq x8 x7 N8
red:
jal s10 color
lw x1 0x408(zero)
and x8 x6 x1
beq x8 zero again
beq t6 x16 red
addi x16 x16 1
jal x0 red
red1:
jal s10 color
lw x1 0x408(zero)
and x8 x7 x1
beq x8 zero again
beq zero x16 red1
addi x11 zero 1
sub x16 x16 x11
jal x0 red1
N8:
lw x1 0x408(zero)
and x8 x6 x1
beq x8 zero again
lw x1 0x408(zero)
and x8 x7 x1
beq x8 zero again
jal x0 N8
color:
jal x0 slow
back:
addi s11 zero 0
add s11 s11 x10
add t5 zero x9
add t4 zero x16
add x9 x9 x9
add x9 x9 x9
add x9 x9 x9
add x9 x9 x9
add x16 x16 x16
add x16 x16 x16
add x16 x16 x16
add x16 x16 x16
add x16 x16 x16
add x16 x16 x16
add x16 x16 x16
add x16 x16 x16
add s11 s11 x16
add s11 s11 x9
sw s11 0x408(zero)
add x9 zero t5
add x16 zero t4
jalr x0 0(s10)
slow:
addi s9 zero 0
addi t3 zero 1024
add t3 t3 t3
add t3 t3 t3
add t3 t3 t3
add t3 t3 t3
add t3 t3 t3
add t3 t3 t3
add t3 t3 t3
add t3 t3 t3
add t3 t3 t3
add t3 t3 t3
add t3 t3 t3
add t3 t3 t3
slow1:
addi s9 s9 1
blt s9 t3 slow1
jalr x0 0(s8)
