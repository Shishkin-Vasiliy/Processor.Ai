PUSH 0
PUSH         ; инициализация счетчика RAX = 0

:start
PUSH 10
PUSH RAX
JA :cycle
HLT

:cycle
PUSH RAX
POP RBX         ; получили значение ячейки равное значению счетчика
PUSH RBX
POP [RAX]
PUSH 1
PUSH RAX
ADD
POP RAX           ; RAX = RAX + 1
JMP :start
