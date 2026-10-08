#ifndef PROCESSING_H
#define PROCESSING_H

enum ProcessingErr_t {
    PROCESSING_OK,
    UNKNOWN_COMMAND,
    INVALID_OPERAND,
    HLT_COMMAND
};

ProcessingErr_t Processing(StackElem_t **IP, Stack_t *stk, int NumCommands);
ProcessingErr_t RunCommand(StackElem_t **IP, Stack_t *stk);
void ProcessingErrMsg(ProcessingErr_t err);

#endif