#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Translate.h"

int Translate(const char *ASM, const char *ByteCode, StackElem_t **CodeSegment)
{
    TranslateErr_t err = TRANSLATION_SUCCESS;

    char *buf = POIZON_CHAR_PTR;
    int nlines = 0;

    int fd = OpenFile(ASM);
    int file_size = GetFileSize(ASM);

    char **index = (char **)calloc(file_size, sizeof(char *));

    if (!index)
    {
        err = BAD_INDEX_PTR;
        TranslateErrMsg(err);
        FreeBuf(&buf, file_size);
        FreeIndex(&index, nlines);
        return 0;
    }

    nlines = ReadFromFile(&buf, index, file_size, fd);
    printf("nlines = %d\n", nlines);

    *CodeSegment = (StackElem_t *)calloc(2 * nlines, sizeof(StackElem_t));
    
    if (!*CodeSegment)
    {   
        err = BAD_CODESEG_PTR;
        TranslateErrMsg(err);
        FreeBuf(&buf, file_size);
        FreeIndex(&index, nlines);
        return 0;
    }
    
    StackElem_t *temp = *CodeSegment;
    
    for (int i = 0; i < nlines; i++)
    {
        err = TranslateCommand(index[i], &temp);
        if (err)
        {
            TranslateErrMsg(err);
            FreeBuf(&buf, file_size);
            FreeIndex(&index, nlines);
            return 0;                        
        }
    }      

    FreeBuf(&buf, file_size);
    FreeIndex(&index, nlines);

    return nlines;
}

TranslateErr_t TranslateCommand(const char *string, StackElem_t **temp)
{
    char Command[MAX_CMD_LEN] = {};
    CmdNum_t CmdNum = UNKNOWN_num;
    StackElem_t CmdVal = 0;

    int num_elems = sscanf(string, "%s %lg", Command, &CmdVal);
    
    if (num_elems > 2 || num_elems < 0)
        return SYNTAX_ERR;

    else if (num_elems == 1)
    {
        CmdNum = GetCmdNum(Command);

        **temp = (StackElem_t)CmdNum;
        (*temp)++;

        return TRANSLATION_SUCCESS;
    }

    else if (num_elems == 2)
    {
        CmdNum = GetCmdNum(Command);

        **temp = (StackElem_t) CmdNum;
        (*temp)++;
        **temp = CmdVal;
        (*temp)++;

        return TRANSLATION_SUCCESS;
    }

    else
        return SYNTAX_ERR;
}

TranslateErr_t FreeCodeSegment(StackElem_t **CodeSegment, int NumCommands)
{
    if (!CodeSegment || !*CodeSegment)
        return BAD_CODESEG_PTR;

    for (int i = 0; i < NumCommands; i++)
    {
        (*CodeSegment)[i] = POIZON_DBL;
    }
    free(*CodeSegment);
    *CodeSegment = POIZON_IP_PTR;

    return TRANSLATION_SUCCESS;
}

CmdNum_t GetCmdNum(const char *Command)
{
    if (strcmp(Command, "HLT") == 0)
        return HLT_num;
    else if (strcmp(Command, "PUSH") == 0)
        return PUSH_num;
    else if (strcmp(Command, "ADD") == 0)
        return ADD_num;
    else if (strcmp(Command, "SUB") == 0)
        return SUB_num; 
    else if (strcmp(Command, "DIV") == 0)
        return DIV_num;
    else if (strcmp(Command, "OUT") == 0)
        return OUT_num; 
    else
        return UNKNOWN_num;
}

void TranslateErrMsg(TranslateErr_t err)
{
    switch(err)
    {
        case TRANSLATION_SUCCESS:
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