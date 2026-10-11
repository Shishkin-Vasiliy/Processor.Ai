#include "Includes.h"
#include "Stack.h"
 
StackErr_t StackPush(Stack_t *stk, StackElem_t value)
{
    StackErr_t err = StackVerify(stk);
    err = STACK_OK;

    if ((stk -> size) == (stk -> capacity))
        err = ResizeUp(stk);

    if (err)
        return err;

    stk -> data[stk -> size++] = value;

    err = StackVerify(stk);
    return err;
}

StackErr_t StackPop(Stack_t *stk, StackElem_t *ptr)
{
    StackErr_t err = StackVerify(stk);

    if ((stk -> size) == 0)
        return STACK_UNDERFLOW;

    if (((stk -> size) <= (stk -> capacity / 4)) && ((stk -> size) > STACK_MIN_CAPACITY))
        err = ResizeDown(stk);

    if (err)
        return err;
    
    *ptr = (stk -> data[--(stk -> size)]);
    stk -> data[(stk -> size)] = POIZON_DBL;

    err = StackVerify(stk);
    return err;
}