#ifndef ASM_H
#define ASM_H

#define MAX_INSTRUCT_LEN 100
#define MAX_LABELS 503

enum AssemblyErr_t {
    ASSEMBLY_SUCCESS = 0,
    SYNTAX_ERR,
    BAD_CODESEG_PTR,
    BAD_INDEX_PTR
};

struct Asm_files {
    const char *Asm_file;
    const char *ByteCode_file;
};

struct AsmCtx_t {
    char InstructBuf[MAX_INSTRUCT_LEN];
    char RegBuf[MAX_INSTRUCT_LEN];
    char LabelBuf[MAX_INSTRUCT_LEN];
    StackElem_t InstructVal;
    int RamIndex;
    REGS RegNum;
    int LabelNum;
    int NumElems;
    InstructNum_t InstructNum;
};

AssemblyErr_t Assembly(Asm_files Asm_Files, Processor_t *Processor);
AssemblyErr_t AssembleInstruct(const char *string, Processor_t *Processor, int *Labels, size_t PassNum, FILE *ByteCode_ptr);
InstructNum_t GetInstructNum(const char *InstructBuf);
REGS GetRegNum(const char *OperandBuf);
void AssemblyErrMsg(AssemblyErr_t AsmErr, const char *Asm_file, size_t LineNum);
int HashString(char *LabelBuf);

AssemblyErr_t AssembleLabel(Processor_t *Processor, AsmCtx_t *AsmCtx, int *Labels);
AssemblyErr_t AssembleNoArgs(Processor_t *Processor, AsmCtx_t *AsmCtx, FILE *ByteCode_ptr, size_t PassNum);
AssemblyErr_t AssembleDblArg(Processor_t *Processor, AsmCtx_t *AsmCtx, FILE *ByteCode_ptr, size_t PassNum);
AssemblyErr_t AssembleLabelArg(Processor_t *Processor, AsmCtx_t *AsmCtx, FILE *ByteCode_ptr, size_t PassNum, int *Labels);
AssemblyErr_t AssembleRegArg(Processor_t *Processor, AsmCtx_t *AsmCtx, FILE *ByteCode_ptr, size_t PassNum);
AssemblyErr_t AssembleRamIndex(Processor_t *Processor, AsmCtx_t *AsmCtx, FILE *ByteCode_ptr, size_t PassNum);
AssemblyErr_t AssembleRamRegIndex(Processor_t *Processor, AsmCtx_t *AsmCtx, FILE *ByteCode_ptr, size_t PassNum);

#endif
