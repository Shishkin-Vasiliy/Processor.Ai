#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Processor.h"
#include "Asm.h"
#include "DisAsm.h"

int main(void)
{
    AsmContext_t AsmContext = {
    .Asm_file = "code.asm",
    .ByteCode_file = "code.txt"
    };

    DisAsmContext_t DisAsmContext = {
        .DisAsm_file = "code.disasm"
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

    DisAssemblyErr_t DisAsmErr = DisAssembly(DisAsmContext, &Processor);
    if (DisAsmErr)
        printf("DisAsmErr = %d\n", DisAsmErr);

    ProcessorDeinit(&Processor);
    return 0;
}




