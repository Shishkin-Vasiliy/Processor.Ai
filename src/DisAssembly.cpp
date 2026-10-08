#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Assembly.h"

int DisAssembly(StackElem_t *IP, int CodeLen, const char *DASM)
{
    FILE *DASM_ptr = fopen(DASM, "w");
    InstructNum_t InstructNum = UNKNOWN_num;
    StackElem_t InstructVal = 0;
    int TwoArgsFlag = 0;
    char buf[MAX_INSTRUCT_LEN] = {};
    int CountInstructs = 0;

    if (!DASM_ptr)
        return ZERO_INSTRUCTS;

    while (CodeLen > 0)
    {
        InstructNum = DisAssembleInstruct(IP, &TwoArgsFlag, buf);
        if (InstructNum != UNKNOWN_num)
        {
            CountInstructs++;
            if (TwoArgsFlag)
            {
                IP++;
                InstructVal = *IP;
                IP++;
                TwoArgsFlag = 0;
                CodeLen -= 2;
                fprintf(DASM_ptr, "%s %lg\n", buf, InstructVal);
            }
            else 
            {
                IP++;
                CodeLen--;
                fprintf(DASM_ptr, "%s\n", buf);
            }
        }
        else
        {
            fclose(DASM_ptr);
            return ZERO_INSTRUCTS;
        }
    }

    fclose(DASM_ptr);

    return CountInstructs;
}

InstructNum_t DisAssembleInstruct(StackElem_t *IP, int *TwoArgsFlag, char *buf)
{
    InstructNum_t InstructNum = (InstructNum_t)(*IP);

    switch(InstructNum)
    {
        case HLT_num:
        {
            if (*TwoArgsFlag)
                *TwoArgsFlag = 0;
            strncpy(buf, "HLT", MAX_INSTRUCT_LEN);
            printf("DASM instruct = <%s>\n", buf);
            return HLT_num;                       // в других свичах убрать break
        }

        case PUSH_num:
        {
            if (!*TwoArgsFlag)
                *TwoArgsFlag = 1;
            strncpy(buf, "PUSH", MAX_INSTRUCT_LEN);
            printf("DASM instruct = <%s>\n", buf);
            return PUSH_num;
        }

        case ADD_num:
        {
            if (*TwoArgsFlag)
                *TwoArgsFlag = 0;
            strncpy(buf, "ADD", MAX_INSTRUCT_LEN);
            printf("DASM instruct = <%s>\n", buf);
            return ADD_num; 
        }

        case SUB_num:
        {
            if (*TwoArgsFlag)
                *TwoArgsFlag = 0;
            strncpy(buf, "SUB", MAX_INSTRUCT_LEN);
            printf("DASM instruct = <%s>\n", buf);
            return SUB_num; 
        }

        case DIV_num:
        {
            if (*TwoArgsFlag)
                *TwoArgsFlag = 0;
            strncpy(buf, "DIV", MAX_INSTRUCT_LEN);
            printf("DASM instruct = <%s>\n", buf);
            return DIV_num; 
        }

        case OUT_num:
        {
            if (*TwoArgsFlag)
                *TwoArgsFlag = 0;
            strncpy(buf, "OUT", MAX_INSTRUCT_LEN);
            printf("DASM instruct = <%s>\n", buf);
            return OUT_num; 
        }

        case UNKNOWN_num:
        {
            if (*TwoArgsFlag)
                *TwoArgsFlag = 0;
            return UNKNOWN_num; 
        }

        default:
            return UNKNOWN_num;
    }
}