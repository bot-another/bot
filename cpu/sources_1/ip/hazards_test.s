start:
sw x0, 0x408(x0)    #out1=0

#test data hazards
addi x1, x0, 1  #x1=1
addi x2, x1, 1  #x2=2
add x3, x1, x2  #x3=3
add x4, x1, x3  #x4=4
add x5, x1, x4  #x5=5
sw x5, 0x408(x0) #out1=5

add x6, x1, x2  #x6=3
add x6, x6, x3  #x6=6
add x6, x6, x4  #x6=10
add x6, x6, x5  #x6=15
sw x6, 0x408(x0)    #out1=15

#test load-use hazard
lw x7, 0x40C(x0) #x7=in
addi x8, x7, 1   #x8=in+1
addi x9, x8, -1  #x9=in
sw x9, 0x408(x0)    #out1=in

#test control hazard
beq x9, x0, start  #if (in==0) start
add x10, x9, x5
add x10, x10, x6
sw x10, 0x408(x0)    #out1=in+20
test:
sw x1 0(zero)
sw x2 4(zero)
sw x3 8(zero)
sw x4 12(zero)
sw x5 16(zero)
sw x6 20(zero)
sw x7 24(zero)
sw x8 28(zero)
sw x9 32(zero)
lw x9 0(zero)
lw x8 4(zero)
lw x7 8(zero)
lw x6 12(zero)
lw x5 16(zero)
lw x4 20(zero)
lw x3 24(zero)
lw x2 28(zero)
lw x1 32(zero)
a:jal x0,d
e:jal x0 a
c:jal x0 e
d:jal x0 c
#do not execute
add x11, x9, x10
add x12, x10, x11
add x13, x11, x12
