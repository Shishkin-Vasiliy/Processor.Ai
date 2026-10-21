IN
CALL :factorial
OUT 
HLT

:factorial
POP RAX
PUSH RAX
PUSH 1       
    JA :recursion
    PUSH 1

RET

:recursion
PUSH RAX
PUSH RAX
PUSH 1
SUB
CALL :factorial
MUL
RET