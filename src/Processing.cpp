#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Assembly.h"
#include "Processing.h"

ProcessingErr_t Processing(StackElem_t **IP, Stack_t *stk, int CountInstructs)
{
    ProcessingErr_t err = PROCESSING_OK;

    while (CountInstructs-- > 0)
    {
        err = RunInstruct(IP, stk);

        if (err)
        {
            ProcessingErrMsg(err);
            return err;
        }
    }
    return err;
}

ProcessingErr_t RunInstruct(StackElem_t **IP, Stack_t *stk)
{
    InstructNum_t CmdNum = (InstructNum_t)(**IP);
    printf("CmdNum = %d\n", CmdNum);

    switch (CmdNum)
    {
        case PUSH_num:
        {
            StackPush(stk, (*IP)[1]); 
            (*IP) += 2;
            return PROCESSING_OK;
        }
        break;

        case ADD_num:
        {
            StackElem_t op1 = 0;
            StackElem_t op2 = 0;
            StackElem_t res = 0;
            StackPop(stk, &op2);
            StackPop(stk, &op1);
            res = op1 + op2;
            StackPush(stk, res);
            (*IP)++;
            return PROCESSING_OK;
        }
        break;

        case SUB_num:
        {
            StackElem_t op1 = 0;
            StackElem_t op2 = 0;
            StackElem_t res = 0;
            StackPop(stk, &op2);
            StackPop(stk, &op1);
            res = op1 - op2;
            StackPush(stk, res);
            (*IP)++;
            return PROCESSING_OK;
        }
        break;

        case DIV_num:
        {
            StackElem_t op1 = 0;
            StackElem_t op2 = 0;
            StackElem_t res = 0;
            StackPop(stk, &op2);
            StackPop(stk, &op1);
            if (DblCmp(op2, 0.0) != 0)
            {
                res = op1 / op2;
                StackPush(stk, res);
            }
            (*IP)++;
            return PROCESSING_OK;
        }

        case OUT_num:
        {
            StackElem_t res = 0;
            StackPop(stk, &res);
            printf("res = %lg\n", res);
            StackPush(stk, res);
            (*IP)++;
            return PROCESSING_OK;
        }
        break;

        case HLT_num:
        {
            (*IP)++;
            return HLT_INSTRUCT;
        }
        break;

        case UNKNOWN_num:
        {
            return UNKNOWN_INSTRUCT;
        }

        default:
            return PROCESSING_OK;
    }
}

void ProcessingErrMsg(ProcessingErr_t err)
{
    switch(err)
    {
        case PROCESSING_OK:
            return;
            break;

        case UNKNOWN_INSTRUCT:
            printf("Unknown Instruct\n");
            break;

        case INVALID_OPERAND:
            printf("Invalid Operand\n");
            break;

        case HLT_INSTRUCT:
            return;
            break;

        default:
            ;
    }
}
