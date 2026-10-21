IN
POP RAX                 ; RAX = n
CALL :fib 
PUSH RBX
OUT
HLT 

:fib 
    PUSH RAX                    ; n == 0
    PUSH 0
    JNE :else1
    PUSH 0
    POP RBX
    RET

    :else1                      ; n == 1
        PUSH RAX
        PUSH 1
        JNE :else2
        PUSH 1
        POP RBX
        RET

        :else2
            PUSH RAX            ; основной случай
            PUSH RAX
            PUSH 1
            SUB                 ; в стеке лежит n n-1
            POP RAX             ; в стеке лежит n а в RAX лежит n-1
            CALL :fib

            POP RAX             ; стек пуст а в RAX лежит n
            PUSH RBX            ; в стеке лежит значение RBX

            PUSH RAX            ; в стеке лежит RBX n 
            PUSH 2              ; в стеке лежит RBX n 2
            SUB                 ; в стеке лежит RBX n-2
            POP RAX             ; в RAX лежит n-2 а в стеке RBX
            CALL :fib 

            PUSH RBX
            ADD
            POP RBX
            
            RET


            

