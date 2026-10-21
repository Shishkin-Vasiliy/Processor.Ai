#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Processor.h"
#include "Assembler.h"

AssemblyErr_t Assembly(Asm_files Asm_Files, Processor_t *Processor)
{
    const char *Asm_file = Asm_Files.Asm_file;
    const char *ByteCode_file = Asm_Files.ByteCode_file;

    FILE *ByteCode_ptr = fopen(ByteCode_file, "w");

    char *buf = POIZON_CHAR_PTR;

    int fd = OpenFile(Asm_file);
    ssize_t file_size = GetFileSize(Asm_file);
    if (file_size < 0)
        return BAD_INDEX_PTR;

    char **index = (char **)calloc((size_t)file_size, sizeof(char *));

    if (!index)
    {
        AssemblyErrMsg(BAD_INDEX_PTR, Asm_file, 0);
        size_t nlines = (size_t)file_size / sizeof(char *);
        FREEALL(ByteCode_ptr, fd, index, nlines, buf, (size_t)file_size);
        return BAD_INDEX_PTR;
    }

    size_t nlines = ReadFromFile(&buf, index, (size_t)file_size, fd);
    if (nlines == 0)
    {
        AssemblyErrMsg(BAD_INDEX_PTR, Asm_file, 0);
        FREEALL(ByteCode_ptr, fd, index, nlines, buf, (size_t)file_size);
        return BAD_INDEX_PTR;
    }

    PRINT_STRINGS(index, nlines, "READ");

    Processor -> CodeSegment = (StackElem_t *)calloc(2 * nlines, sizeof(StackElem_t));

    if (!(Processor -> CodeSegment))
    {
        AssemblyErrMsg(BAD_CODESEG_PTR, Asm_file, 0);
        FREEALL(ByteCode_ptr, fd, index, nlines, buf, (size_t)file_size);
        return BAD_CODESEG_PTR;
    }

    AssemblyErr_t AsmErr = ASSEMBLY_SUCCESS;

    static int Labels[MAX_LABELS] = {};

    for (size_t i = 0; i < MAX_LABELS; i++)
        Labels[i] = -1;

    for (size_t N = 1; N <= 2; N++)
    {
        Processor -> IP = 0;
        for (size_t i = 0; i < nlines; i++)
        {
            if (IsEmptyLine(index[i]))
                continue;
            AsmErr = AssembleInstruct(index[i], Processor, Labels, N, ByteCode_ptr);
            if (AsmErr)
            {
                AssemblyErrMsg(AsmErr, Asm_file, i + 1);
                FREEALL(ByteCode_ptr, fd, index, nlines, buf, (size_t)file_size);
                return AsmErr;
            }
        }
    }  
     
    Processor -> CodeLen = Processor -> IP;    

    Processor -> IP = 0;

    FREEALL(ByteCode_ptr, fd, index, nlines, buf, (size_t)file_size);

    return ASSEMBLY_SUCCESS;
}

AssemblyErr_t AssembleInstruct(const char *string, Processor_t *Processor, int *Labels, size_t PassNum, FILE *ByteCode_ptr)
{
    AsmCtx_t AsmCtx = {
        .InstructBuf = {},
        .RegBuf = {},
        .LabelBuf = {},
        .InstructVal = 0,
        .RegNum = NUM_REGS,
        .LabelNum = 0,
        .NumElems = 0,
        .InstructNum = UNKNOWN_num
    };

    AssemblyErr_t err = ASSEMBLY_SUCCESS;

    int NumElems = 0;

    NumElems = sscanf(string, "%s %lg", AsmCtx.InstructBuf, &(AsmCtx.InstructVal));
    if (NumElems == 2)
    {
        err = AssembleDblArg(Processor, &AsmCtx, ByteCode_ptr, PassNum);
        if (!err)
            return ASSEMBLY_SUCCESS;
    }
    
    NumElems = sscanf(string, "%s :%s", AsmCtx.InstructBuf, AsmCtx.LabelBuf);
    if (NumElems == 2)
    {
        err = AssembleLabelArg(Processor, &AsmCtx, ByteCode_ptr, PassNum, Labels);
        if (!err)
            return ASSEMBLY_SUCCESS;
    }
    
    NumElems = sscanf(string, "%s [%d]", AsmCtx.InstructBuf, &(AsmCtx.RamIndex));
    if (NumElems == 2)
    {
        err = AssembleRamIndex(Processor, &AsmCtx, ByteCode_ptr, PassNum);
        if (!err)
            return ASSEMBLY_SUCCESS;
    }
    
    NumElems = sscanf(string, "%s [%99[^]]", AsmCtx.InstructBuf, AsmCtx.RegBuf);
    if (NumElems == 2)
    {
        err = AssembleRamRegIndex(Processor, &AsmCtx, ByteCode_ptr, PassNum);
        if (!err)
            return ASSEMBLY_SUCCESS;
    }
    
    NumElems = sscanf(string, "%s %s", AsmCtx.InstructBuf, AsmCtx.RegBuf);
    if (NumElems == 2)
    {
        err = AssembleRegArg(Processor, &AsmCtx, ByteCode_ptr, PassNum);
        if (!err)
            return ASSEMBLY_SUCCESS;
    }
    
    NumElems = sscanf(string, " :%s", AsmCtx.LabelBuf);
    if (NumElems == 1)
    {
        err = AssembleLabel(Processor, &AsmCtx, Labels);
        if (!err)   
            return ASSEMBLY_SUCCESS;
    }

    NumElems = sscanf(string, "%s", AsmCtx.InstructBuf);
    if (NumElems == 1)
    {
        err = AssembleNoArgs(Processor, &AsmCtx, ByteCode_ptr, PassNum);
        if (!err)
            return ASSEMBLY_SUCCESS;
    }
    
    return SYNTAX_ERR;
}

int HashString(char *LabelBuf)
{
    int i = 0;
    unsigned long long int Hash = 0;

    for (; LabelBuf[i] != '\0'; i++)
    {
        Hash = ((Hash << 5) - Hash) + (unsigned long long)LabelBuf[i];
    }

    return (int)(Hash % MAX_LABELS);
}

InstructNum_t GetInstructNum(const char *InstructBuf)
{
    if (strcmp(InstructBuf, "HLT") == 0)
        return HLT_num;
    else if (strcmp(InstructBuf, "PUSH") == 0)
        return PUSH_num;
    else if (strcmp(InstructBuf, "POP") == 0)
        return POP_num;
    else if (strcmp(InstructBuf, "ADD") == 0)
        return ADD_num;
    else if (strcmp(InstructBuf, "SUB") == 0)
        return SUB_num; 
    else if (strcmp(InstructBuf, "DIV") == 0)
        return DIV_num;
    else if (strcmp(InstructBuf, "MUL") == 0)
        return MUL_num;
    else if (strcmp(InstructBuf, "JMP") == 0)
        return JMP_num;
    else if (strcmp(InstructBuf, "JA") == 0)
        return JA_num;
    else if (strcmp(InstructBuf, "JAE") == 0)
        return JAE_num;
    else if (strcmp(InstructBuf, "JB") == 0)
        return JB_num; 
    else if (strcmp(InstructBuf, "JBE") == 0)
        return JBE_num;
    else if (strcmp(InstructBuf, "JE") == 0)
        return JE_num;
    else if (strcmp(InstructBuf, "JNE") == 0)
        return JNE_num;  
    else if (strcmp(InstructBuf, "OUT") == 0)
        return OUT_num;
    else if (strcmp(InstructBuf, "IN") == 0)
        return IN_num;
    else if (strcmp(InstructBuf, "CALL") == 0)
        return CALL_num;
    else if (strcmp(InstructBuf, "RET") == 0)
        return RET_num;
    else
        return UNKNOWN_num;
}

REGS GetRegNum(const char *OperandBuf)
{
    if (strcmp(OperandBuf, "RAX") == 0)
        return RAX;
    else if (strcmp(OperandBuf, "RBX") == 0)
        return RBX;
    else if (strcmp(OperandBuf, "RCX") == 0)
        return RCX;
    else if (strcmp(OperandBuf, "RDX") == 0)
        return RDX;
    else
        return NUM_REGS;
}

void AssemblyErrMsg(AssemblyErr_t AsmErr, const char *Asm_file, size_t LineNum)
{
    switch(AsmErr)
    {
        case ASSEMBLY_SUCCESS:
            break;

        case SYNTAX_ERR:
            printf("Assembly Error: Syntax Error at line %lu in file %s\n", LineNum, Asm_file);
            break;

        case BAD_CODESEG_PTR:
            printf("Assembly Error: Invalid Pointer at CodeSegment\n");
            break;

        case BAD_INDEX_PTR:
            printf("Assembly Error: Invalid Pointer at String Index\n");    
            break;

        default:
            ;
    } 
}

AssemblyErr_t AssembleLabel(Processor_t *Processor, AsmCtx_t *AsmCtx, int *Labels)
{
    AsmCtx -> LabelNum = HashString(AsmCtx -> LabelBuf);
    Labels[AsmCtx -> LabelNum] = Processor -> IP;
    return ASSEMBLY_SUCCESS;
}

AssemblyErr_t AssembleNoArgs(Processor_t *Processor, AsmCtx_t *AsmCtx, FILE *ByteCode_ptr, size_t PassNum)
{
    AsmCtx -> InstructNum = GetInstructNum(AsmCtx -> InstructBuf);
    InstructNum_t InstructNum = AsmCtx -> InstructNum;
    if ((InstructNum == UNKNOWN_num) || (InstructNum == PUSH_num) 
    ||  (InstructNum == JMP_num)     || (InstructNum == JA_num) 
    ||  (InstructNum == JA_num)      || (InstructNum == JAE_num)
    ||  (InstructNum == JB_num)      || (InstructNum == JBE_num)
    ||  (InstructNum == JE_num)      || (InstructNum == JNE_num)
    ||  (InstructNum == CALL_num))         
    {
        return SYNTAX_ERR;
    }
    else
    {
        if (PassNum == 2 && ByteCode_ptr)
            fprintf(ByteCode_ptr, "%d\n", AsmCtx -> InstructNum);
        Processor -> CodeSegment[Processor -> IP] = (StackElem_t)(InstructNum);
        Processor -> IP += 1;
        return ASSEMBLY_SUCCESS;
    }
}

AssemblyErr_t AssembleDblArg(Processor_t *Processor, AsmCtx_t *AsmCtx, FILE *ByteCode_ptr, size_t PassNum)
{
    AsmCtx -> InstructNum = GetInstructNum(AsmCtx -> InstructBuf);
    InstructNum_t InstructNum = AsmCtx -> InstructNum;
    if ((InstructNum == UNKNOWN_num) || ((InstructNum != PUSH_num)
    &&  (InstructNum != JMP_num)     && (InstructNum != JA_num)
    &&  (InstructNum != JAE_num)     && (InstructNum != JB_num)
    &&  (InstructNum != JBE_num)     && (InstructNum != JE_num)
    &&  (InstructNum != JNE_num)     && (InstructNum != CALL_num)
    &&  (InstructNum != POP_num)))
    {
        return SYNTAX_ERR;
    }
    
    Processor -> CodeSegment[Processor -> IP] = (StackElem_t)(InstructNum | DBLMASK);
    Processor -> CodeSegment[Processor -> IP + 1] = AsmCtx -> InstructVal;

    if (PassNum == 2 && ByteCode_ptr)
    {
        fprintf(ByteCode_ptr, "%d %lg\n", AsmCtx -> InstructNum | DBLMASK, Processor -> CodeSegment[Processor -> IP + 1]);
    }
    Processor -> IP += 2;

    return ASSEMBLY_SUCCESS;
}

AssemblyErr_t AssembleLabelArg(Processor_t *Processor, AsmCtx_t *AsmCtx, FILE *ByteCode_ptr, size_t PassNum, int *Labels)
{
    AsmCtx -> InstructNum = GetInstructNum(AsmCtx -> InstructBuf);
    InstructNum_t InstructNum = AsmCtx -> InstructNum;
    if ((InstructNum == UNKNOWN_num) || ((InstructNum != JMP_num)     
    && (InstructNum != JA_num)       &&  (InstructNum != JAE_num)     
    && (InstructNum != JB_num)       &&  (InstructNum != JBE_num) 
    && (InstructNum != JE_num)       &&  (InstructNum != JNE_num)
    && (InstructNum != CALL_num)))
    {
        return SYNTAX_ERR;
    }
    AsmCtx -> LabelNum = HashString(AsmCtx -> LabelBuf);    
    if (PassNum == 2 && ByteCode_ptr)
    {
        fprintf(ByteCode_ptr, "%d %lg\n", AsmCtx -> InstructNum, (StackElem_t)Labels[AsmCtx -> LabelNum]);
    }

    Processor -> CodeSegment[Processor -> IP] = (StackElem_t)InstructNum;
    Processor -> CodeSegment[Processor -> IP + 1] = (StackElem_t)Labels[AsmCtx -> LabelNum];
    Processor -> IP += 2;

    return ASSEMBLY_SUCCESS;
}

AssemblyErr_t AssembleRegArg(Processor_t *Processor, AsmCtx_t *AsmCtx, FILE *ByteCode_ptr, size_t PassNum)
{
    AsmCtx -> InstructNum = GetInstructNum(AsmCtx -> InstructBuf);
    InstructNum_t InstructNum = AsmCtx -> InstructNum;
    if ((InstructNum == UNKNOWN_num) || ((InstructNum != PUSH_num)
    &&  (InstructNum != POP_num)))
    {
        return SYNTAX_ERR;
    }
    AsmCtx -> RegNum = GetRegNum(AsmCtx -> RegBuf);    
    Processor -> CodeSegment[Processor -> IP] = (StackElem_t)(InstructNum | REGMASK);
    Processor -> CodeSegment[Processor -> IP + 1] = (StackElem_t)(AsmCtx -> RegNum);

    if (PassNum == 2 && ByteCode_ptr)
    {
        fprintf(ByteCode_ptr, "%d %lg\n", AsmCtx -> InstructNum | REGMASK, Processor -> CodeSegment[Processor -> IP + 1]);
    }
    Processor -> IP += 2;
    return ASSEMBLY_SUCCESS;
}

AssemblyErr_t AssembleRamIndex(Processor_t *Processor, AsmCtx_t *AsmCtx, FILE *ByteCode_ptr, size_t PassNum)
{
    AsmCtx -> InstructNum = GetInstructNum(AsmCtx -> InstructBuf);
    InstructNum_t InstructNum = AsmCtx -> InstructNum;
    if ((InstructNum == UNKNOWN_num) || ((InstructNum != PUSH_num)
    &&  (InstructNum != POP_num)))
    {
        return SYNTAX_ERR;
    }
    Processor -> CodeSegment[Processor -> IP] = (StackElem_t)(InstructNum | MEMMASK);
    Processor -> CodeSegment[Processor -> IP + 1] = (StackElem_t)(AsmCtx -> RamIndex);

    if (PassNum == 2 && ByteCode_ptr)
    {
        fprintf(ByteCode_ptr, "%d %lg\n",  AsmCtx -> InstructNum | MEMMASK, Processor -> CodeSegment[Processor -> IP + 1]);
    }
    Processor -> IP += 2;
    return ASSEMBLY_SUCCESS;
}

AssemblyErr_t AssembleRamRegIndex(Processor_t *Processor, AsmCtx_t *AsmCtx, FILE *ByteCode_ptr, size_t PassNum)
{
    AsmCtx -> InstructNum = GetInstructNum(AsmCtx -> InstructBuf);
    InstructNum_t InstructNum = AsmCtx -> InstructNum;
    if ((InstructNum == UNKNOWN_num) || ((InstructNum != PUSH_num)
    &&  (InstructNum != POP_num)))
    {
        return SYNTAX_ERR;
    }
    AsmCtx -> RegNum = GetRegNum(AsmCtx -> RegBuf);
    Processor -> CodeSegment[Processor -> IP] = (StackElem_t)(InstructNum | REGMASK | MEMMASK);
    Processor -> CodeSegment[Processor -> IP + 1] = (StackElem_t)(AsmCtx -> RegNum);

    if (PassNum == 2 && ByteCode_ptr)
    {
        fprintf(ByteCode_ptr, "%d %lg\n",  AsmCtx -> InstructNum | REGMASK | MEMMASK, Processor -> CodeSegment[Processor -> IP + 1]);
    }
    Processor -> IP += 2;

    return ASSEMBLY_SUCCESS;
}