#include "Includes.h"
#include "Stack.h"
#include "Processor.h"

typedef ProcessingErr_t (*Instruct)(Processor_t *Processor);

static const Instruct InstructionTable[] = {
    HLT,
    PUSH,
    POP,
    ADD,
    SUB,
    DIV,
    MUL,
    PUSHr,
    POPr,
    JMP,
    JA,
    JAE,
    JB,
    JBE,
    JE,
    JNE,
    OUT,
    IN,
    CALL,
    RET
};

void ProcessorInit(Stack_t *DblStk, Stack_t *RetStk, Processor_t *Processor)
{
    STACK_CTOR(*DblStk, STACK_MIN_CAPACITY);
    STACK_CTOR(*RetStk, STACK_MIN_CAPACITY);
    Processor -> DblStk = DblStk;
    Processor -> RetStk = RetStk;
    Processor -> CodeSegment = POIZON_CODESEG_PTR;
    Processor -> IP = 0;
    for (size_t i = 0; i < NUM_REGS; i++)
        Processor -> Regs[i] = 0;
}

void ProcessorDeinit(Processor_t *Processor)
{
    StackDTor(Processor -> DblStk);
    StackDTor(Processor -> RetStk);
    free(Processor -> CodeSegment);
    Processor -> CodeSegment = POIZON_CODESEG_PTR;
    Processor -> IP = 0;
    for (size_t RegNum = 0; RegNum < NUM_REGS; RegNum++)
        Processor -> Regs[RegNum] = 0;
}

ProcessingErr_t Processing(Processor_t *Processor)
{
    StackElem_t *CodeSegment = Processor -> CodeSegment;

    while ((InstructNum_t)CodeSegment[Processor -> IP] != HLT_num)
    {   
        ProcessingErr_t err = RunInstruct(Processor);
        if (err)
        {
            ProcessingErrMsg(err);
            return err;
        }
    }
    Processor -> IP = 0;
    return PROCESSING_OK;
}

ProcessingErr_t RunInstruct(Processor_t *Processor)
{
    int IP = Processor -> IP;
    if (IP < 0)
        return SEGMENTATION_FAULT;

    ProcessingErr_t err = InstructionTable[(int)((Processor -> CodeSegment)[IP])](Processor); 
    if (err)
        return err;
    
    return PROCESSING_OK;
}

ProcessingErr_t HLT(Processor_t *Processor)
{
    return HLT_INSTRUCT;
}

ProcessingErr_t PUSH(Processor_t *Processor)
{
    StackElem_t *CodeSegment = Processor -> CodeSegment;
    Stack_t *DblStk = Processor -> DblStk;
    int IP = Processor -> IP;
    StackElem_t val = CodeSegment[IP + 1];

    StackErr_t err = StackPush(DblStk, val);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }
    Processor -> IP += 2;
    return PROCESSING_OK;
}

ProcessingErr_t POP(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t val = 0;
    StackErr_t err = StackPop(DblStk, &val);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }
    Processor -> IP += 1;
    return PROCESSING_OK;
}

ProcessingErr_t ADD(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t op1 = 0;
    StackElem_t op2 = 0;
    StackElem_t res = 0;
    StackErr_t err = StackPop(DblStk, &op2);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    err = StackPop(DblStk, &op1);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    res = op1 + op2;
    err = StackPush(DblStk, res);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    Processor -> IP += 1;
    return PROCESSING_OK;
}

ProcessingErr_t SUB(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t op1 = 0;
    StackElem_t op2 = 0;
    StackElem_t res = 0;
    StackErr_t err = StackPop(DblStk, &op2);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    err = StackPop(DblStk, &op1);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    res = op1 - op2;
    err = StackPush(DblStk, res);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    Processor -> IP += 1;
    return PROCESSING_OK;
}

ProcessingErr_t DIV(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t op1 = 0;
    StackElem_t op2 = 0;
    StackElem_t res = 0;
    StackErr_t err = StackPop(DblStk, &op2);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    err = StackPop(DblStk, &op1);
    
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }
    if (DblCmp(op2, 0.0) != 0)
    {
        res = op1 / op2;
        err = StackPush(DblStk, res);
        
        if (err)
        {
            printf("StackErr = %d\n", err);
            return STACK_CORRUPTED;
        }
    }
    Processor -> IP += 1;
    return PROCESSING_OK;
}

ProcessingErr_t MUL(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t op1 = 0;
    StackElem_t op2 = 0;
    StackElem_t res = 0;
    StackErr_t err = StackPop(DblStk, &op2);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    err = StackPop(DblStk, &op1);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    res = op1 * op2;
    err = StackPush(DblStk, res);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    Processor -> IP += 1;
    return PROCESSING_OK;
}

ProcessingErr_t PUSHr(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    int IP = Processor -> IP;
    StackElem_t *CodeSegment = Processor -> CodeSegment;
    StackElem_t *Regs = Processor -> Regs;
    REGS RegNum = (REGS)CodeSegment[IP + 1];

    StackElem_t val = Regs[RegNum];

    StackErr_t err = StackPush(DblStk, val);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    Processor -> IP += 2;
    return PROCESSING_OK;
}

ProcessingErr_t POPr(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    int IP = Processor -> IP;
    StackElem_t *CodeSegment = Processor -> CodeSegment;
    REGS RegNum = (REGS)CodeSegment[IP + 1];

    StackErr_t err = StackPop(DblStk, Processor -> Regs + RegNum);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    Processor -> IP += 2;
    return PROCESSING_OK;
}

ProcessingErr_t JMP(Processor_t *Processor)
{
    int IP = Processor -> IP;
    StackElem_t *CodeSegment = Processor -> CodeSegment;
    Processor -> IP = (int)(CodeSegment[IP + 1]);
    return PROCESSING_OK;
}

ProcessingErr_t JA(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t a = 0;
    StackElem_t b = 0;
    StackErr_t err = StackPop(DblStk, &a);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }
    
    err = StackPop(DblStk, &b);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    ProcessingErr_t JmpErr = PROCESSING_OK;

    if (DblCmp(b, a) > 0)
        JmpErr = JMP(Processor);
    else 
        Processor -> IP += 2;
    return JmpErr;
}

ProcessingErr_t JAE(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t a = 0;
    StackElem_t b = 0;
    StackErr_t err = StackPop(DblStk, &a);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }
    
    err = StackPop(DblStk, &b);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    ProcessingErr_t JmpErr = PROCESSING_OK;
    if (DblCmp(b, a) > 0 || DblCmp(a, b) == 0)
        JmpErr = JMP(Processor);
    else 
        Processor -> IP += 2;
    return JmpErr;
}

ProcessingErr_t JB(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t a = 0;
    StackElem_t b = 0;
    StackErr_t err = StackPop(DblStk, &a);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }
    
    err = StackPop(DblStk, &b);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    ProcessingErr_t JmpErr = PROCESSING_OK;
    if (DblCmp(b, a) < 0)
        JmpErr = JMP(Processor);
    else 
        Processor -> IP += 2;
    return JmpErr;
}

ProcessingErr_t JBE(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t a = 0;
    StackElem_t b = 0;
    StackErr_t err = StackPop(DblStk, &a);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }
    
    err = StackPop(DblStk, &b);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }
    ProcessingErr_t JmpErr = PROCESSING_OK;
    if (DblCmp(b, a) < 0 || DblCmp(a, b) == 0)
        JmpErr = JMP(Processor);
    else 
        Processor -> IP += 2;
    return JmpErr;
}

ProcessingErr_t JE(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t a = 0;
    StackElem_t b = 0;
    StackErr_t err = StackPop(DblStk, &a);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }
    
    err = StackPop(DblStk, &b);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    ProcessingErr_t JmpErr = PROCESSING_OK;
    if (DblCmp(b, a) == 0)
        JmpErr = JMP(Processor);
    else 
        Processor -> IP += 2;
    return JmpErr;
}

ProcessingErr_t JNE(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t a = 0;
    StackElem_t b = 0;
    StackErr_t err = StackPop(DblStk, &a);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }
    
    err = StackPop(DblStk, &b);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }

    ProcessingErr_t JmpErr = PROCESSING_OK;
    if (DblCmp(b, a) != 0)
        JmpErr = JMP(Processor);
    else 
        Processor -> IP += 2;
    return JmpErr;
}

ProcessingErr_t OUT(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t val = 0;
    StackErr_t err = StackPop(DblStk, &val);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }
    printf("OUT: %lg\n", val);
    Processor -> IP += 1;
    return PROCESSING_OK;
}

ProcessingErr_t IN(Processor_t *Processor)
{
    Stack_t *DblStk = Processor -> DblStk;
    StackElem_t val = 0;
    printf("IN:\n");
    int NumElems = scanf("%lg", &val);
    if (NumElems != 1)
        return INVALID_OPERAND;
    StackErr_t err = StackPush(DblStk, val);
    if (err)
    {
        printf("StackErr = %d\n", err);
        return STACK_CORRUPTED;
    }
    Processor -> IP += 1;
    return PROCESSING_OK;
}

ProcessingErr_t CALL(Processor_t *Processor)
{
    int RetAdress = Processor -> IP + 2;
    StackErr_t StkErr = StackPush(Processor -> RetStk, (StackElem_t)RetAdress);
    if (StkErr)
    {
        printf("StackErr = %d\n", StkErr);
        return STACK_CORRUPTED;
    }
    ProcessingErr_t JmpErr = JMP(Processor);
    if (JmpErr)
        return JmpErr;
        
    return PROCESSING_OK;
}

ProcessingErr_t RET(Processor_t *Processor)
{
    StackElem_t RetAdress = 0;
    StackErr_t StkErr = StackPop(Processor -> RetStk, &RetAdress);
    if (StkErr)
    {
        printf("StackError = %d\n", StkErr);
        return STACK_CORRUPTED;
    }
    Processor -> IP = (int)RetAdress;
    return PROCESSING_OK;
}

void ProcessingErrMsg(ProcessingErr_t err)
{
    switch(err)
    {
        case PROCESSING_OK:
            break;

        case STACK_CORRUPTED:
            printf("Processing Error: Stack Corrupted\n");
            break;

        case UNKNOWN_INSTRUCT:
            printf("Processing Error: Unknown Instruct\n");
            break;

        case HLT_INSTRUCT:
            break;

        case INVALID_OPERAND:
            printf("Processing Error: Invalid Operand\n");
            break;

        case SEGMENTATION_FAULT:
            printf("Processing Error: Segmentation Fault\n");
            break;

        default:
            ;
    }
}
