#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Processor.h"
#include "Assembler.h"
#include "DisAssembler.h"

int main(int argc, char **argv)
{
    AsmContext_t AsmContext = {
    .Asm_file = "code.asm",
    .ByteCode_file = "code.txt"
    };

    if (argc == 2)
        AsmContext.Asm_file = argv[1];

    DisAsmContext_t DisAsmContext = {
        .DisAsm_file = "code.disasm"
    };

    Stack_t DblStk = {};
    Stack_t RetStk = {};

    Processor_t Processor = {};
    ProcessorInit(&DblStk, &RetStk, &Processor);

    AssemblyErr_t AsmErr = Assembly(AsmContext, &Processor);
    if (AsmErr)
        printf("AsmErr = <%d>\n", AsmErr);

    Processing(&Processor);

    DisAssemblyErr_t DisAsmErr = DisAssembly(DisAsmContext, &Processor);
    if (DisAsmErr)
        printf("DisAsmErr = %d\n", DisAsmErr);

    ProcessorDeinit(&Processor);
    return 0;
}




