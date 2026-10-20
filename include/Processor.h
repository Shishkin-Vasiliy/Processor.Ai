#ifndef PROCESSOR_H
#define PROCESSOR_H

#define POIZON_CODESEG_PTR ((StackElem_t *)222)
#define RAM_SIZE 200

#define DBLMASK 0x20
#define REGMASK 0x40
#define MEMMASK 0x80

enum ProcessingErr_t {
    PROCESSING_OK,
    STACK_CORRUPTED,
    UNKNOWN_INSTRUCT,
    INVALID_OPERAND,
    SEGMENTATION_FAULT
};

enum InstructNum_t {
    HLT_num = 0,
    PUSH_num,
    POP_num,
    ADD_num,
    SUB_num,
    DIV_num,
    MUL_num,
    JMP_num,
    JA_num,
    JAE_num,
    JB_num,
    JBE_num,
    JE_num,
    JNE_num,
    OUT_num,
    IN_num,
    CALL_num,
    RET_num,
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


#endif