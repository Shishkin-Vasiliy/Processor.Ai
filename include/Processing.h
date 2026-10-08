#ifndef PROCESSING_H
#define PROCESSING_H

enum ProcessingErr_t {
    PROCESSING_OK,
    UNKNOWN_INSTRUCT,
    INVALID_OPERAND,
    HLT_INSTRUCT
};

ProcessingErr_t Processing(StackElem_t **IP, Stack_t *stk, int CountInstructs);
ProcessingErr_t RunInstruct(StackElem_t **IP, Stack_t *stk);
void ProcessingErrMsg(ProcessingErr_t err);

#endif