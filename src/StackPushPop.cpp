#include "Includes.h"
#include "Stack.h"
 
StackErr_t StackPush(Stack_t *DblStk, StackElem_t value)
{
    StackErr_t err = StackVerify(DblStk);
    err = STACK_OK;

    if ((DblStk -> size) == (DblStk -> capacity))
        err = ResizeUp(DblStk);

    if (err)
        return err;

    DblStk -> data[DblStk -> size++] = value;

    err = StackVerify(DblStk);
    return err;
}

StackErr_t StackPop(Stack_t *DblStk, StackElem_t *ptr)
{
    StackErr_t err = StackVerify(DblStk);

    if ((DblStk -> size) == 0)
        return STACK_UNDERFLOW;

    if (((DblStk -> size) <= (DblStk -> capacity / 4)) && ((DblStk -> size) > STACK_MIN_CAPACITY))
        err = ResizeDown(DblStk);

    if (err)
        return err;
    
    *ptr = (DblStk -> data[--(DblStk -> size)]);
    DblStk -> data[(DblStk -> size)] = POIZON_DBL;

    err = StackVerify(DblStk);
    return err;
}