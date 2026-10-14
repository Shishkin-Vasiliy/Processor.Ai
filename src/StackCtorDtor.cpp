#include "Includes.h"
#include "Stack.h"

StackErr_t StackCTor(Stack_t *DblStk, size_t capacity
                ON_DBG(, const char *stack_name, const char *file_name, const char *func_name, int line, const char *time, const char *date))
{
    printf("Initiating Stack\n");

    if (capacity < STACK_MIN_CAPACITY)
    {
        capacity = STACK_MIN_CAPACITY;
    }
    else if (capacity > STACK_MAX_CAPACITY)
    {
        capacity = STACK_MAX_CAPACITY;
    }

    StackErr_t err = StackIsEmpty(DblStk);
    DblStk -> buf = (StackElem_t *) calloc(capacity + 2, sizeof(StackElem_t));
    DblStk -> data = DblStk -> buf + 1; 

    *(DblStk -> buf) = CHICKEN_LOWER;
    *(DblStk -> buf + capacity + 1) = CHICKEN_UPPER;

    DblStk -> capacity = capacity;
    
    DblStk -> chicken_left = CHICKEN_LEFT;
    DblStk -> chicken_right = CHICKEN_RIGHT;
    
    err = StackVerify(DblStk);

    ON_DBG (DblStk -> stack_name = stack_name;
            DblStk -> file_name = file_name;
            DblStk -> func_name = func_name;
            DblStk -> line = line;
            DblStk -> time = time;
            DblStk -> date = date;)

    for (size_t i = 0; i < (DblStk -> capacity); i++)
    {
        (DblStk -> data)[i] = POIZON_DBL;
    }
    StackDump(DblStk);
    return err;
}

StackErr_t StackDTor(Stack_t *DblStk)
{
    StackErr_t err = StackVerify(DblStk);

    if (err)
        return err;

    printf("Destroying Stack\n");
    StackDump(DblStk);

    for (size_t i = 0; i < DblStk -> capacity + 2; i++)
    {
        (DblStk -> buf)[i] = POIZON_DBL;
    }

    free(DblStk -> buf);
    DblStk -> buf = POIZON_BUF_PTR;
    DblStk -> data = POIZON_DATA_PTR;

    return STACK_OK;
}



