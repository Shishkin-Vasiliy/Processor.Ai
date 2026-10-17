#ifndef PROCESSOR_H
#define PROCESSOR_H

#define POIZON_CODESEG_PTR ((StackElem_t *)222)
#define RAM_SIZE 100

enum ProcessingErr_t {
    PROCESSING_OK,
    STACK_CORRUPTED,
    UNKNOWN_INSTRUCT,
    INVALID_OPERAND,
    SEGMENTATION_FAULT
};

enum InstructNum_t {
    HLT_num = 0,
    PUSH_num = 1,
    POP_num = 2,
    ADD_num = 3,
    SUB_num = 4,
    DIV_num = 5,
    MUL_num = 6,
    PUSHr_num = 7,
    POPr_num = 8,
    JMP_num = 9,
    JA_num = 10,
    JAE_num = 11,
    JB_num = 12,
    JBE_num = 13,
    JE_num = 14,
    JNE_num = 15,
    OUT_num = 16,
    IN_num = 17,
    CALL_num = 18,
    RET_num = 19,
    PUSHm_num = 20,
    POPm_num = 21,
    UNKNOWN_num
};

enum REGS {
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3,
    NUM_REGS = 4
};

struct Processor_t {
    Stack_t *DblStk;
    Stack_t *RetStk;
    StackElem_t *CodeSegment;
    int IP;
    int CodeLen;
    StackElem_t Regs[NUM_REGS]; 
    StackElem_t *RAM;          ///// TODO initialize with calloc
};

ProcessingErr_t Processing(Processor_t *Processor);
ProcessingErr_t RunInstruct(Processor_t *Processor);
void ProcessingErrMsg(ProcessingErr_t err);
void ProcessorInit(Stack_t *DblStk, Stack_t *RetStk, Processor_t *Processor);
void ProcessorDeinit(Processor_t *Processor);
void ProcessorDump(Processor_t *Processor);
int GetWindowStart(int CodeLen, const int WindowSize, int IP);

ProcessingErr_t HLT(Processor_t *Processor);
ProcessingErr_t PUSH(Processor_t *Processor);
ProcessingErr_t POP(Processor_t *Processor);
ProcessingErr_t ADD(Processor_t *Processor);
ProcessingErr_t SUB(Processor_t *Processor);
ProcessingErr_t DIV(Processor_t *Processor);
ProcessingErr_t MUL(Processor_t *Processor);
ProcessingErr_t PUSHr(Processor_t *Processor);
ProcessingErr_t POPr(Processor_t *Processor);
ProcessingErr_t JMP(Processor_t *Processor);
ProcessingErr_t JA(Processor_t *Processor);
ProcessingErr_t JAE(Processor_t *Processor);
ProcessingErr_t JB(Processor_t *Processor);
ProcessingErr_t JBE(Processor_t *Processor);
ProcessingErr_t JE(Processor_t *Processor);
ProcessingErr_t JNE(Processor_t *Processor);
ProcessingErr_t OUT(Processor_t *Processor);
ProcessingErr_t IN(Processor_t *Processor);
ProcessingErr_t CALL(Processor_t *Processor);
ProcessingErr_t RET(Processor_t *Processor);
ProcessingErr_t PUSHm(Processor_t *Processor);
ProcessingErr_t POPm(Processor_t *Processor);

#endif