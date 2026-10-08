#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Assembly.h"
#include "Processing.h"

int main(void)
{
    const char *ASM = "code.asm";
    const char *ByteCode = "code.txt";
    const char *DASM = "code.disasm";

    StackElem_t *CodeSegment = POIZON_IP_PTR;

    int CountInstructs = 0;
    int CodeLen = Assembly(ASM, ByteCode, &CodeSegment, &CountInstructs);     
       
    printf("CODESEGMENT\n");
    for (int i = 0; i < CodeLen; i++)
    {
        printf("[%d] = %lg\n", i, CodeSegment[i]);
    }

    StackElem_t *IP = CodeSegment;

    int CountInstructsDASM = DisAssembly(IP, CodeLen, DASM);

    if (CountInstructsDASM == ZERO_INSTRUCTS)
        printf("DisAssembly failed\n");

    Stack_t stk = {};
    size_t capacity = 17;

    StackErr_t StkErr = STACK_CTOR(stk, capacity);
    ERROR_MSG(&stk, StkErr);

    ProcessingErr_t PrErr = Processing(&IP, &stk, CountInstructs);
    if (PrErr)
        ProcessingErrMsg(PrErr);

    FreeCodeSegment(&CodeSegment, CountInstructs);
    StackDTor(&stk); 
    
    return 0;
}




