#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Translate.h"
#include "Processing.h"

ProcessingErr_t Processing(StackElem_t **IP, Stack_t *stk, int NumCommands)
{
    ProcessingErr_t err = PROCESSING_OK;

    while (NumCommands-- > 0)
    {
        err = RunCommand(IP, stk);
        
        printf("Err = %d\n", err);
        if (err)
        {
            ProcessingErrMsg(err);
            return err;
        }
    }
    return err;
}

ProcessingErr_t RunCommand(StackElem_t **IP, Stack_t *stk)
{
    CmdNum_t CmdNum = (CmdNum_t)(**IP);
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
            return HLT_COMMAND;
        }
        break;

        case UNKNOWN_num:
        {
            return UNKNOWN_COMMAND;
        }

        default:
            return PROCESSING_OK;;
    }
}

void ProcessingErrMsg(ProcessingErr_t err)
{
    switch(err)
    {
        case PROCESSING_OK:
            return;
            break;

        case UNKNOWN_COMMAND:
            printf("Unknown Command\n");
            break;

        case INVALID_OPERAND:
            printf("Invalid Operand\n");
            break;

        case HLT_COMMAND:
            return;
            break

        default:
            ;
    }
}
