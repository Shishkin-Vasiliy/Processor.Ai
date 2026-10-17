#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Processor.h"
#include "Assembler.h"
#include "DisAssembler.h"

int main(int argc, char **argv)
{
    Asm_files Asm_Files = {
    .Asm_file = "code.asm",
    .ByteCode_file = "code.txt"
    };

    if (argc == 2)
        Asm_Files.Asm_file = argv[1];

    DisAsm_files DisAsm_Files = {
        .DisAsm_file = "code.disasm"
    };

    Stack_t DblStk = {};
    Stack_t RetStk = {};

    Processor_t Processor = {};
    ProcessorInit(&DblStk, &RetStk, &Processor);

    AssemblyErr_t AsmErr = Assembly(Asm_Files, &Processor);
    if (AsmErr)
        printf("AsmErr = <%d>\n", AsmErr);

    Processing(&Processor);

    //DisAssemblyErr_t DisAsmErr = DisAssembly(DisAsm_Files, &Processor);
    //if (DisAsmErr)
    //    printf("DisAsmErr = %d\n", DisAsmErr);

    ProcessorDeinit(&Processor);
    return 0;
}




