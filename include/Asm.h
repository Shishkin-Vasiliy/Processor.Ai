#ifndef ASM_H
#define ASM_H

#define MAX_INSTRUCT_LEN 100
#define MAX_LABELS 10

enum AssemblyErr_t {
    ASSEMBLY_SUCCESS = 0,
    SYNTAX_ERR,
    BAD_CODESEG_PTR,
    BAD_INDEX_PTR
};

struct AsmContext_t {
    const char *Asm_file;
    const char *ByteCode_file;
};

AssemblyErr_t Assembly(AsmContext_t AsmContext, Processor_t *Processor);
AssemblyErr_t AssembleInstruct(const char *string, Processor_t *Processor, int *Labels);
InstructNum_t GetInstructNum(const char *InstructBuf);
REGS GetRegNum(const char *OperandBuf);

#endif
