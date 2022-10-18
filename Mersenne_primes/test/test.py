fp = open('data.txt', 'r')
a = 0
b = 0
e = 0
f = 0
h = 0
k = 0
for i in range(0, 32):
    a = a + ((2 ** 32) ** i) * int(fp.readline())
for i in range(0, 32):
    b = b + ((2 ** 32) ** i) * int(fp.readline())
c = a + b
d = a * b
j = int(fp.readline())
for i in range(0, j + 1):
    e = e + ((2 ** 32) ** i) * int(fp.readline())
j = int(fp.readline())
for i in range(0, j + 1):
    f = f + ((2 ** 32) ** i) * int(fp.readline())
g = 2 ** int(fp.readline()) - 1
j = int(fp.readline())
for i in range(0, j + 1):
    h = h + ((2 ** 32) ** i) * int(fp.readline())
for i in range(1, 1001):
    if int(fp.readline()) == 2 ** i - 1:
        k = k + 1
if c == e:
    print("Add is true")
if d == f:
    print("Multiplication is true")
if (d % g) == h or g == h:
    print("Mod is true")
if k == 1000:
    print("String operation is true")
