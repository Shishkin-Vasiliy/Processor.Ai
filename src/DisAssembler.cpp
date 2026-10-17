#include "Includes.h"
#include "Stack.h"
#include "Processor.h"
#include "DisAssembler.h"

DisAssemblyErr_t DisAssembly(DisAsm_files DisAsm_Files, Processor_t *Processor)
{
    const char *DisAsm_file = DisAsm_Files.DisAsm_file;
    FILE *DisAsm_ptr = fopen(DisAsm_file, "w");

    printf("CODELEN = %d\n", Processor -> CodeLen);

    DisAssemblyErr_t DisAsmErr = DISASSEMBLY_SUCCESS;
    while (Processor -> IP < Processor -> CodeLen) 
    { 
        DisAsmErr = DisAssembleInstruct(DisAsm_ptr, Processor);
        if (DisAsmErr)
        {
            if (DisAsm_ptr)
                fclose(DisAsm_ptr);
            return DisAsmErr;
        }
    } 

    if (DisAsm_ptr)
        fclose(DisAsm_ptr);

    Processor -> IP = 0;
    return DISASSEMBLY_SUCCESS;
}

DisAssemblyErr_t DisAssembleInstruct(FILE *DisAsm_ptr, Processor_t *Processor)
{
    if (!DisAsm_ptr)
        return BAD_FILE_PTR;

    InstructNum_t InstructNum = UNKNOWN_num;

    InstructNum = (InstructNum_t)(Processor -> CodeSegment)[Processor -> IP];
    printf("InstructNum = %d\n", InstructNum);
    switch(InstructNum)
    {
        case HLT_num:
        {
            fprintf(DisAsm_ptr, "HLT\n");
            Processor -> IP += 1;
            return DISASSEMBLY_SUCCESS;
        }
        
        case PUSH_num:
        {
            StackElem_t InstructVal = (Processor -> CodeSegment)[Processor -> IP + 1];
            fprintf(DisAsm_ptr, "PUSH %lg\n", InstructVal);
            Processor -> IP += 2;
            return DISASSEMBLY_SUCCESS;
        }

        case POP_num:
        {
            fprintf(DisAsm_ptr, "POP\n");
            Processor -> IP += 1;
            return DISASSEMBLY_SUCCESS;
        }

        case ADD_num:
        {
            fprintf(DisAsm_ptr, "ADD\n");
            Processor -> IP += 1;
            return DISASSEMBLY_SUCCESS;
        }

        case SUB_num:
        {
            fprintf(DisAsm_ptr, "SUB\n");
            Processor -> IP += 1;
            return DISASSEMBLY_SUCCESS;
        }

        case DIV_num:
        {
            fprintf(DisAsm_ptr, "DIV\n");
            Processor -> IP += 1;
            return DISASSEMBLY_SUCCESS;
        }

        case MUL_num:
        {
            fprintf(DisAsm_ptr, "MUL\n");
            Processor -> IP += 1;
            return DISASSEMBLY_SUCCESS;
        }

        case PUSHr_num:
        {
            REGS RegNum = (REGS)(Processor -> CodeSegment)[Processor -> IP + 1];
            fprintf(DisAsm_ptr, "PUSHr %d\n", RegNum);
            Processor -> IP += 2;
            return DISASSEMBLY_SUCCESS;
        }

        case POPr_num:
        {
            REGS RegNum = (REGS)(Processor -> CodeSegment)[Processor -> IP + 1];
            fprintf(DisAsm_ptr, "POPr %d\n", RegNum);
            Processor -> IP += 2;
            return DISASSEMBLY_SUCCESS;
        }

        case JMP_num:
        {
            int JmpPos = (int)(Processor -> CodeSegment)[Processor -> IP + 1];
            fprintf(DisAsm_ptr, "JMP %d\n", JmpPos);
            Processor -> IP += 2;
            return DISASSEMBLY_SUCCESS;
        }

        case JA_num:
        {
            int JmpPos = (int)(Processor -> CodeSegment)[Processor -> IP + 1];
            fprintf(DisAsm_ptr, "JA %d\n", JmpPos);
            Processor -> IP += 2;
            return DISASSEMBLY_SUCCESS;
        }

        case JAE_num:
        {
            int JmpPos = (int)(Processor -> CodeSegment)[Processor -> IP + 1];
            fprintf(DisAsm_ptr, "JAE %d\n", JmpPos);
            Processor -> IP += 2;
            return DISASSEMBLY_SUCCESS;
        }

        case JB_num:
        {
            int JmpPos = (int)(Processor -> CodeSegment)[Processor -> IP + 1];
            fprintf(DisAsm_ptr, "JB %d\n", JmpPos);
            Processor -> IP += 2;
            return DISASSEMBLY_SUCCESS;
        }

        case JBE_num:
        {
            int JmpPos = (int)(Processor -> CodeSegment)[Processor -> IP + 1];
            fprintf(DisAsm_ptr, "JBE %d\n", JmpPos);
            Processor -> IP += 2;
            return DISASSEMBLY_SUCCESS;
        }

        case JE_num:
        {
            int JmpPos = (int)(Processor -> CodeSegment)[Processor -> IP + 1];
            fprintf(DisAsm_ptr, "JE %d\n", JmpPos);
            Processor -> IP += 2;
            return DISASSEMBLY_SUCCESS;
        }

        case JNE_num:
        {
            int JmpPos = (int)(Processor -> CodeSegment)[Processor -> IP + 1];
            fprintf(DisAsm_ptr, "JNE %d\n", JmpPos);
            Processor -> IP += 2;
            return DISASSEMBLY_SUCCESS;
        }

        case OUT_num:
        {
            fprintf(DisAsm_ptr, "OUT\n");
            Processor -> IP += 1;
            return DISASSEMBLY_SUCCESS;
        }

        case IN_num:
        {
            fprintf(DisAsm_ptr, "IN\n");
            Processor -> IP += 1;
            return DISASSEMBLY_SUCCESS;
        }

        case CALL_num:
        {
            int CallPos = (int)(Processor -> CodeSegment)[Processor -> IP + 1];
            fprintf(DisAsm_ptr, "CALL %d\n", CallPos);
            Processor -> IP += 2;
            return DISASSEMBLY_SUCCESS;
        }

        case RET_num:
        {
            fprintf(DisAsm_ptr, "RET\n");
            Processor -> IP += 1;
            return DISASSEMBLY_SUCCESS;
        }

        case PUSHm_num:
        {
            fprintf(DisAsm_ptr, "RET\n");
            Processor -> IP += 2;
            return DISASSEMBLY_SUCCESS;
        }

        case POPm_num:
        {   
            fprintf(DisAsm_ptr, "RET\n");
            Processor -> IP += 1;
            return DISASSEMBLY_SUCCESS;
        }

        case UNKNOWN_num:
            return DISASM_FAIL;
        
        default:
            return DISASM_FAIL;
    }
}

void DisAssemblyErrMsg(DisAssemblyErr_t DisAsmErr)
{
    switch (DisAsmErr)
    {
        case DISASSEMBLY_SUCCESS:
            break;

        case BAD_FILE_PTR:
            printf("DisAssembly Error: File for DisAssembler opening failed\n");
            break;

        case DISASM_FAIL:
            printf("DisAssembly Error: DisAssembly failed\n");
            break;

        default:
            ;
    }
}