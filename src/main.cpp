#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Translate.h"
#include "Processing.h"

int main(void)
{
    const char *ASM = "code.ASM";
    const char *ByteCode = "code.txt";

    StackElem_t *CodeSegment = POIZON_IP_PTR;

    int NumCommands = Translate(ASM, ByteCode, &CodeSegment);     
       
    printf("CODESEGMENT\n");
    for (int i = 0; i < (NumCommands * 2); i++)
    {
        printf("[%d] = %lg\n", i, CodeSegment[i]);
    }

    StackElem_t *IP = CodeSegment;

    Stack_t stk = {};
    size_t capacity = 17;

    StackErr_t StkErr = STACK_CTOR(stk, capacity);
    ERROR_MSG(&stk, StkErr);

    ProcessingErr_t PrErr = Processing(&IP, &stk, NumCommands);

    FreeCodeSegment(&CodeSegment, NumCommands);
    StackDTor(&stk); 
    
    return 0;
}




