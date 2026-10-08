#ifndef ASM_H
#define ASM_H

#define POIZON_IP_PTR ((StackElem_t *)222)
#define MAX_INSTRUCT_LEN 100
#define ZERO_INSTRUCTS 0

enum AssemblyErr_t {
    ASSEMBLY_SUCCESS,
    SYNTAX_ERR,
    BAD_CODESEG_PTR,
    BAD_INDEX_PTR
};

enum InstructNum_t {
    HLT_num,
    PUSH_num,
    ADD_num,
    SUB_num,
    DIV_num,
    OUT_num,
    UNKNOWN_num
};

int Assembly(const char *ASM, const char *ByteCode, StackElem_t **CodeSegment, int *CountInstructs);
AssemblyErr_t AssembleInstruction(const char *string, StackElem_t **CodeSegment, FILE *ByteCode_ptr, int *CodeLen);
InstructNum_t GetInstructNum(const char *Instruct);
void AssemblyErrMsg(AssemblyErr_t err);
AssemblyErr_t FreeCodeSegment(StackElem_t **CodeSegment, int CountInstructs);

int DisAssembly(StackElem_t *IP, int CodeLen, const char *DASM);
InstructNum_t DisAssembleInstruct(StackElem_t *IP, int *TwoArgsFlag, char *buf);

#endif