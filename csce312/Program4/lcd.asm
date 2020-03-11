//JORGE FARINACCI -- 125008668
//Largest Common Divisor: Implement a program that calculates the largest common divisor (lcd)
//of two given non-negative integers ‘a’ and ‘b’, which are stored in R0 and R1. The lcd is stored in R2

@ITERATOR
M = 0
@R2
M = 0

(GCDLOOP)
//UPDATE ITERATOR : ITERATOR = ITERATOR + 1
@ITERATOR
M = M + 1

//CHECK IF ITERATOR == RAM[0]? IF SO END PROGRAM
D = M
@R0
D = D - M
@PROGRAMEND
D;JEQ

//CHECK IF ITERATOR == RAM[1]? IF SO END PROGRAM
@ITERATOR
D = M
@R1
D = D - M
@PROGRAMEND
D;JEQ

//PROGRAM TO COMPUTE RAM[0] % iterator
@R0
D = M
@a
M = D
@ITERATOR
D = M
@b
M = D
@INDICATOR
M = 0
//CALL  RAM[0] % iterator
@MODULOSTART
0;JMP
(RESULT0)
//STORE RESULT OF CALL
@MODULO
D = M
@MODRES0
M = D
//PROGRAM TO COMPUTE RAM[1] % iterator
@R1
D = M
@a
M = D
@ITERATOR
D = M
@b
M = D
@INDICATOR
M = 1
//CALL  RAM[1] % iterator
@MODULOSTART
0;JMP
(RESULT1)
//STORE RESULT OF CALL
@MODULO
D = M
@MODRES1
M = D

//CHECK IF RAM[0] % iterator == 0
@MODRES0
D = M
@POSSIBLEGCD
D;JEQ
//IF NOT GO BACK TO START OF LOOP
@GCDLOOP
0;JMP
//CHECK IF RAM[1] % iterator == 0
(POSSIBLEGCD)
@MODRES1
D = M
@ISGCD
D;JEQ
//IF NOT GO BACK TO START OF LOOP
@GCDLOOP
0;JMP

//IF NEW GCD FOND UPDATE GCD: RAM[2] = ITERATOR
(ISGCD)
@ITERATOR
D = M
@R2
M = D
@GCDLOOP
0;JMP


(MODULOSTART)
@MODULO
M = 0
@a
D = M
@MODULOEND
D; JEQ

(LOOP)
@b
D = D - M
@ODD
D; JLT
@MODULO
M = M + 1
@EVEN
D; JEQ
@LOOP
0; JMP

(ODD)
@b
D = D + M
@MODULO
M = D
@MODULOEND
0;JMP

(EVEN)
@MODULO
M = 0
//END OF CALL TO MODULO
(MODULOEND)

//RETURN TO RIGHT POINT IN GCDLOOP
@INDICATOR
D = M
@RESULT0
D;JEQ
@RESULT1
D;JGT
(PROGRAMEND)
