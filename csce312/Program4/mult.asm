//JORGE FARINACCI -- 125008668
// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
@R2
M = 0
@LOOPINVARIANT
M = 0
(LOOP)

@LOOPINVARIANT
D=M// Set D to loopinvariant
@R1
D=D-M // LOOP CONDITION: D = LOOPINVARIANT - RAM[1]
@END
D;JEQ //JMP FROM LOOP IF D = LOOPINVARIANT - RAM[1] = 0

@R2
D=M // SET D TO RAM[2]
@R0
D=D+M // D = RAM[2] + RAM[0]
@R2
M=D// RAM[2] = D

@LOOPINVARIANT
M = M + 1 //INCREMENT LOOP INVARIANT BY 1
@LOOP
0;JMP //RETURN TO START OF LOOP
(END)
