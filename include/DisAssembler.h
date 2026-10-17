#ifndef DISASM_H
#define DISASM_H

enum DisAssemblyErr_t {
    DISASSEMBLY_SUCCESS = 0,
    BAD_FILE_PTR = 1,
    DISASM_FAIL = 2
};

struct DisAsm_files {
    const char *DisAsm_file;
};

DisAssemblyErr_t DisAssembly(DisAsm_files DisAsm_Files, Processor_t *Processor);
DisAssemblyErr_t DisAssembleInstruct(FILE *DisAsm_ptr, Processor_t *Processor);
void DisAssemblyErrMsg(DisAssemblyErr_t DisAsmErr);

#endif