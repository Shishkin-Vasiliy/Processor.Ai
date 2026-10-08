#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Assembly.h"

int Assembly(const char *ASM, const char *ByteCode, StackElem_t **CodeSegment, int *CountInstructs)
{
    FILE *ByteCode_ptr = fopen(ByteCode, "w");

    const int ZERO_CODE_LEN = 0;
    AssemblyErr_t err = ASSEMBLY_SUCCESS;
    char *buf = POIZON_CHAR_PTR;
    int CodeLen = ZERO_CODE_LEN;

    int fd = OpenFile(ASM);
    int file_size = GetFileSize(ASM);

    char **index = (char **)calloc(file_size, sizeof(char *));

    if (!index)
    {
        err = BAD_INDEX_PTR;
        AssemblyErrMsg(err);
        FreeBuf(&buf, file_size);
        FreeIndex(&index, int (file_size / sizeof(char *)));     //////
        return ZERO_CODE_LEN;
    }

    *CountInstructs = ReadFromFile(&buf, index, file_size, fd);

    *CodeSegment = (StackElem_t *)calloc(2 * (*CountInstructs), sizeof(StackElem_t));
    
    if (!*CodeSegment)
    {   
        err = BAD_CODESEG_PTR;
        AssemblyErrMsg(err);
        FreeBuf(&buf, file_size);
        FreeIndex(&index, *CountInstructs);
        return ZERO_CODE_LEN;
    }
    
    StackElem_t *temp = *CodeSegment;
    
    for (int i = 0; i < *CountInstructs; i++)
    {
        err = AssembleInstruction(index[i], &temp, ByteCode_ptr, &CodeLen);
        if (err)
        {
            AssemblyErrMsg(err);
            FreeBuf(&buf, file_size);
            FreeIndex(&index, *CountInstructs);
            fclose(ByteCode_ptr);
            return ZERO_CODE_LEN;                        
        }
    }  
    printf("CodeLen = %d\n", CodeLen);    

    fclose(ByteCode_ptr);
    FreeBuf(&buf, file_size);
    FreeIndex(&index, *CountInstructs);

    return CodeLen;
}

AssemblyErr_t AssembleInstruction(const char *string, StackElem_t **temp, FILE *ByteCode_ptr, int *CodeLen)
{
    char Instruct[MAX_INSTRUCT_LEN] = {};
    InstructNum_t InstructNum = UNKNOWN_num;
    StackElem_t InstructVal = 0;

    int NumElems = sscanf(string, "%s %lg", Instruct, &InstructVal);
    
    if (NumElems > 2 || NumElems < 0)
        return SYNTAX_ERR;

    else if (NumElems == 1)
    {
        InstructNum = GetInstructNum(Instruct);

        if (ByteCode_ptr)
            fprintf(ByteCode_ptr, "%-2d\n", InstructNum);

        **temp = (StackElem_t)InstructNum;
        (*temp)++;

        (*CodeLen) += 1;

        return ASSEMBLY_SUCCESS;
    }

    else if (NumElems == 2)
    {
        InstructNum = GetInstructNum(Instruct);

        if (ByteCode_ptr)
            fprintf(ByteCode_ptr, "%-2d %lg\n", InstructNum, InstructVal);

        **temp = (StackElem_t) InstructNum;
        (*temp)++;
        **temp = InstructVal;
        (*temp)++;

        (*CodeLen) += 2;
 
        return ASSEMBLY_SUCCESS;
    }

    else
        return SYNTAX_ERR;
}

AssemblyErr_t FreeCodeSegment(StackElem_t **CodeSegment, int CountInstructs)
{
    if (!CodeSegment || !*CodeSegment)
        return BAD_CODESEG_PTR;

    for (int i = 0; i < CountInstructs; i++)
    {
        (*CodeSegment)[i] = POIZON_DBL;
    }
    free(*CodeSegment);
    *CodeSegment = POIZON_IP_PTR;

    return ASSEMBLY_SUCCESS;
}

InstructNum_t GetInstructNum(const char *Instruct)
{
    if (strcmp(Instruct, "HLT") == 0)
        return HLT_num;
    else if (strcmp(Instruct, "PUSH") == 0)
        return PUSH_num;
    else if (strcmp(Instruct, "ADD") == 0)
        return ADD_num;
    else if (strcmp(Instruct, "SUB") == 0)
        return SUB_num; 
    else if (strcmp(Instruct, "DIV") == 0)
        return DIV_num;
    else if (strcmp(Instruct, "OUT") == 0)
        return OUT_num; 
    else
        return UNKNOWN_num;
}

void AssemblyErrMsg(AssemblyErr_t err)
{
    switch(err)
    {
        case ASSEMBLY_SUCCESS:
            break;

        case SYNTAX_ERR:
            printf("Syntax Error\n");
            break;

        case BAD_CODESEG_PTR:
            printf("Invalid pointer at Code Segment\n");
            break;

        case BAD_INDEX_PTR:
            printf("Invalid pointer at index of commands (strings)\n");

        default:
            ;
    }
}