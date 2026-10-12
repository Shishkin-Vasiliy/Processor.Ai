#ifndef DISASM_H
#define DISASM_H

enum DisAssemblyErr_t {
    DISASSEMBLY_SUCCESS = 0,
    BAD_FILE_PTR = 1,
    DISASM_FAIL = 2
};

struct DisAsmContext_t {
    const char *DisAsm_file;
};

DisAssemblyErr_t DisAssembly(DisAsmContext_t DisAsmContext, Processor_t *Processor);
DisAssemblyErr_t DisAssembleInstruct(FILE *DisAsm_ptr, Processor_t *Processor);

#endif