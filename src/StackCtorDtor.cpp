#include "Includes.h"
#include "Stack.h"

StackErr_t StackCTor(Stack_t *stk, size_t capacity
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

    StackErr_t err = StackIsEmpty(stk);
    stk -> buf = (StackElem_t *) calloc(capacity + 2, sizeof(StackElem_t));
    stk -> data = stk -> buf + 1; 

    *(stk -> buf) = CHICKEN_LOWER;
    *(stk -> buf + capacity + 1) = CHICKEN_UPPER;

    stk -> capacity = capacity;
    
    stk -> chicken_left = CHICKEN_LEFT;
    stk -> chicken_right = CHICKEN_RIGHT;
    
    err = StackVerify(stk);

    ON_DBG (stk -> stack_name = stack_name;
            stk -> file_name = file_name;
            stk -> func_name = func_name;
            stk -> line = line;
            stk -> time = time;
            stk -> date = date;)

    for (size_t i = 0; i < (stk -> capacity); i++)
    {
        (stk -> data)[i] = POIZON_DBL;
    }
    StackDump(stk);
    return err;
}

StackErr_t StackDTor(Stack_t *stk)
{
    StackErr_t err = StackVerify(stk);

    if (err)
        return err;

    printf("Destroying Stack\n");
    StackDump(stk);

    for (size_t i = 0; i < stk -> capacity + 2; i++)
    {
        (stk -> buf)[i] = POIZON_DBL;
    }

    free(stk -> buf);
    stk -> buf = POIZON_BUF_PTR;
    stk -> data = POIZON_DATA_PTR;

    return STACK_OK;
}



