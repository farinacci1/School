//JORGE FARINACCI -- 125008668
// Division: Write a program to calculate the quotient from a division operation. The values
// of dividend ‘a’ and divisor ‘b’ are stored in R0 and R1, respectively. Assume that the
// dividend ‘a’ is a non-negative integer, and the divisor ‘b’ is a positive integer. Store the
// quotient in R2. Ignore the remainder.*/
// divide R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

@R2
M = 0
@R0
D = M
@END
D, JEQ

(LOOP)
@R1
D = D - M
@END
D, JLT
@R2
M = M + 1
@END
D, JEQ

@LOOP
0, JMP


(END)
