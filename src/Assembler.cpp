#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Processor.h"
#include "Assembler.h"

AssemblyErr_t Assembly(AsmContext_t AsmContext, Processor_t *Processor)
{
    const char *Asm_file = AsmContext.Asm_file;
    const char *ByteCode_file = AsmContext.ByteCode_file;

    FILE *ByteCode_ptr = fopen(ByteCode_file, "w");

    char *buf = POIZON_CHAR_PTR;

    int fd = OpenFile(Asm_file);
    size_t file_size = GetFileSize(Asm_file);

    char **index = (char **)calloc(file_size, sizeof(char *));

    if (!index)
    {
        AssemblyErrMsg(BAD_INDEX_PTR, Asm_file, 0);
        size_t nlines = file_size / sizeof(char *);
        FREEALL(ByteCode_ptr, fd, index, nlines, buf, file_size);
        return BAD_INDEX_PTR;
    }

    size_t nlines = ReadFromFile(&buf, index, file_size, fd);
    if (nlines == 0)
    {
        AssemblyErrMsg(BAD_INDEX_PTR, Asm_file, 0);
        FREEALL(ByteCode_ptr, fd, index, nlines, buf, file_size);
        return BAD_INDEX_PTR;
    }

    PrintStrings(index, nlines, "READ");

    Processor -> CodeSegment = (StackElem_t *)calloc(2 * nlines, sizeof(StackElem_t));

    if (!(Processor -> CodeSegment))
    {
        AssemblyErrMsg(BAD_CODESEG_PTR, Asm_file, 0);
        FREEALL(ByteCode_ptr, fd, index, nlines, buf, file_size);
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
                FREEALL(ByteCode_ptr, fd, index, nlines, buf, file_size);
                return AsmErr;
            }
        }
    }  
    
    Processor -> CodeLen = Processor -> IP;    

    Processor -> IP = 0;

    FREEALL(ByteCode_ptr, fd, index, nlines, buf, file_size);

    return ASSEMBLY_SUCCESS;
}

AssemblyErr_t AssembleInstruct(const char *string, Processor_t *Processor, int *Labels, size_t PassNum, FILE *ByteCode_ptr)
{
    char InstructBuf[MAX_INSTRUCT_LEN] = {};
    char OperandBuf[MAX_INSTRUCT_LEN] = {};
    char LabelBuf[MAX_INSTRUCT_LEN] = {};
    StackElem_t InstructVal = 0;
    REGS RegNum = NUM_REGS;
    int LabelNum = 0;
    int NumElems = 0;
    InstructNum_t InstructNum = UNKNOWN_num;

    NumElems = sscanf(string, ":%s", LabelBuf);

    // обработка меток
    if (string[0] == ':')
    {
        if (NumElems == 1)
        {
            LabelNum = HashString(LabelBuf);
            Labels[LabelNum] = Processor -> IP;
            return ASSEMBLY_SUCCESS;
        }
        else 
            return SYNTAX_ERR;
    }

    NumElems = sscanf(string, "%s %s", InstructBuf, OperandBuf);

    // инструкция без аргумента 
    if (NumElems == 1)
    {
        InstructNum = GetInstructNum(InstructBuf);
        if ((InstructNum == UNKNOWN_num) || (InstructNum == PUSH_num) 
        ||  (InstructNum == PUSHr_num)   || (InstructNum == POPr_num)
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
                fprintf(ByteCode_ptr, "%d\n", InstructNum);
            Processor -> CodeSegment[Processor -> IP] = (StackElem_t)InstructNum;
            Processor -> IP += 1;
            return ASSEMBLY_SUCCESS;
        }
    }

    // инструкция с аргументом
    else if (NumElems > 1)
    {
        InstructNum = GetInstructNum(InstructBuf);
        if ((InstructNum == UNKNOWN_num) || ((InstructNum != PUSH_num)
        &&  (InstructNum != PUSHr_num)   && (InstructNum != POPr_num)
        &&  (InstructNum != JMP_num)     && (InstructNum != JA_num)
        &&  (InstructNum != JAE_num)     && (InstructNum != JB_num)
        &&  (InstructNum != JBE_num)     && (InstructNum != JE_num)
        &&  (InstructNum != JNE_num)     && (InstructNum != CALL_num)))
        {
            return SYNTAX_ERR;
        }
        if (PassNum == 2 && ByteCode_ptr)
            fprintf(ByteCode_ptr, "%d ", InstructNum);

        Processor -> CodeSegment[Processor -> IP] = (StackElem_t)InstructNum;

        // аргумент - метка
        if (OperandBuf[0] == ':')
        {
            if (sscanf(OperandBuf, ":%s", LabelBuf) != 1)
                return SYNTAX_ERR;
            
            LabelNum = HashString(LabelBuf);    

            if (PassNum == 2 && ByteCode_ptr)
                fprintf(ByteCode_ptr, "%lg\n", (StackElem_t)Labels[LabelNum]);

            Processor -> CodeSegment[Processor -> IP + 1] = (StackElem_t)Labels[LabelNum];
            Processor -> IP += 2;
            return ASSEMBLY_SUCCESS;
        }

        // аргумент число или регистр
        else
        {
            char *EndPtr = POIZON_CHAR_PTR;
            InstructVal = strtod(OperandBuf, &EndPtr);

            // аргумент - регистр
            if (EndPtr == OperandBuf)
            {
                if (InstructNum != PUSHr_num && InstructNum != POPr_num)
                    return SYNTAX_ERR;

                RegNum = GetRegNum(OperandBuf);
                if (RegNum == NUM_REGS)
                    return SYNTAX_ERR;

                if (PassNum == 2 && ByteCode_ptr)
                    fprintf(ByteCode_ptr, " %d\n", RegNum);

                Processor -> CodeSegment[Processor -> IP + 1] = (StackElem_t)RegNum;
                Processor -> IP += 2;
                return ASSEMBLY_SUCCESS;
            }

            // аргумент - число
            else 
            {   
                if (InstructNum == PUSHr_num || InstructNum == POPr_num)
                    return SYNTAX_ERR;
                
                if (PassNum == 2 && ByteCode_ptr)
                    fprintf(ByteCode_ptr, " %lg\n", InstructVal);

                Processor -> CodeSegment[Processor -> IP + 1] = InstructVal;
                Processor -> IP += 2;
                return ASSEMBLY_SUCCESS;
            }
        }    
    }

    else
        return SYNTAX_ERR;
}

int HashString(char *LabelBuf)
{
    int i = 0;
    unsigned int Hash = 0;
    int p = 31;

    for (; LabelBuf[i] != '\0'; i++)
    {
        Hash = Hash * p + LabelBuf[i];
    }

    return (int) (Hash % MAX_LABELS);
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
    else if (strcmp(InstructBuf, "PUSHr") == 0)
        return PUSHr_num;
    else if (strcmp(InstructBuf, "POPr") == 0)
        return POPr_num;
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




