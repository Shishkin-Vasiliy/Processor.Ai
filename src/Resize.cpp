#include "Includes.h"
#include "Stack.h"

StackErr_t ResizeUp(Stack_t *stk)
{
    StackErr_t err = StackVerify(stk);

    StackElem_t *temp = (StackElem_t *)realloc(stk -> buf, (stk -> capacity + 2) * sizeof(StackElem_t) * RESIZE_COEFF);
    if (temp)
    {
        stk -> buf = temp;
        stk -> data = temp + 1;
    }

    err = StackVerify(stk);
    if (err)
        return err;

    if (stk -> capacity * RESIZE_COEFF > STACK_MAX_CAPACITY)
        return STACK_OVERFLOW;
    
    stk -> capacity *= RESIZE_COEFF;
    
    for (size_t i = stk -> size; i < stk -> capacity; i++)
    {
        (stk -> data)[i] = POIZON_DBL;
    } 
    *(stk -> data + stk -> capacity) = CHICKEN_UPPER;

    err = StackVerify(stk);
    return err;
}

StackErr_t ResizeDown(Stack_t *stk)
{
    StackErr_t err = StackVerify(stk);

    StackElem_t *temp = (StackElem_t *)realloc(stk -> buf, (stk -> capacity + 2) * sizeof(StackElem_t) / RESIZE_COEFF);
    if (temp)
    {
        stk -> buf = temp;
        stk -> data = temp + 1;
    }

    err = StackVerify(stk);
    if (err)
        return err;
 
    stk -> capacity /= RESIZE_COEFF;
    *(stk -> data + stk -> capacity + 1) = CHICKEN_UPPER;

    err = StackVerify(stk);
    return err;
}