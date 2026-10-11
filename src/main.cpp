#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Processor.h"
#include "Asm.h"

int main(void)
{
    AsmContext_t AsmContext = {
    .Asm_file = "code.asm",
    .ByteCode_file = "code.txt"
    };

    Stack_t stk = {};

    Processor_t Processor = {};
    ProcessorInit(&stk, &Processor);

    AssemblyErr_t AsmErr = Assembly(AsmContext, &Processor);
    if (AsmErr)
        printf("AsmErr = <%d>\n", AsmErr);

    ProcessingErr_t PrErr = Processing(&Processor);
    if (PrErr)
        printf("PrErr = %d\n", PrErr);

    ProcessorDeinit(&Processor);
    return 0;
}




