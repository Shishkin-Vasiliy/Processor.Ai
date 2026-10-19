#include "Includes.h"
#include "Reading.h"
#include "Stack.h"
#include "Processor.h"
#include "Assembler.h"
#include "DisAssembler.h"
#include "VideoMemory.h"

void DrawMemory(Processor_t *Processor)
{
    StackElem_t *RAM = Processor -> RAM;

    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            if (DblCmp(RAM[10 * i + j], 0) == 0)
                printf(".");
            else 
                printf("*");
        }
        printf("\n");
    }
}