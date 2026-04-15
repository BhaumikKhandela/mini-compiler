; MIPS-like pseudo assembly
LI R1, 5
STORE R1, a
LI R1, 10
STORE R1, b
LI R1, 5
LI R2, 10
ADD R3, R1, R2
STORE R3, t1
LOAD R1, t1
STORE R1, c
