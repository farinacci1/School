//JORGE FARINACCI -- 125008668
// Modulo: Implement a program that calculates the modulo of two given numbers ‘a’ and ‘b’, which is a%b in math. The
// value of ‘a’ is stored in R0, and the value of ‘b’ is stored in R1. Value ‘a’ is a non-negative
// integer and ‘b’ is a positive integer. The modulo value is stored in R2.


@R2
M = 0
@R0
D = M
@END
D; JEQ

(LOOP)
@R1
D = D - M
@ODD
D, JLT
@R2
M = M + 1
@EVEN
D, JEQ

@LOOP
0; JMP

(ODD)
@R1
D = D + M
@R2
M = D
@END
0;JMP
(EVEN)
@R2
M = 0
(END)
