#include "Includes.h"
#include "Stack.h"

StackErr_t ResizeUp(Stack_t *DblStk)
{
    StackErr_t err = StackVerify(DblStk);

    StackElem_t *temp = (StackElem_t *)realloc(DblStk -> buf, (DblStk -> capacity + 2) * sizeof(StackElem_t) * RESIZE_COEFF);
    if (temp)
    {
        DblStk -> buf = temp;
        DblStk -> data = temp + 1;
    }

    err = StackVerify(DblStk);
    if (err)
        return err;

    if (DblStk -> capacity * RESIZE_COEFF > STACK_MAX_CAPACITY)
        return STACK_OVERFLOW;
    
    DblStk -> capacity *= RESIZE_COEFF;
    
    for (size_t i = DblStk -> size; i < DblStk -> capacity; i++)
    {
        (DblStk -> data)[i] = POIZON_DBL;
    } 
    *(DblStk -> data + DblStk -> capacity) = CHICKEN_UPPER;

    err = StackVerify(DblStk);
    return err;
}

StackErr_t ResizeDown(Stack_t *DblStk)
{
    StackErr_t err = StackVerify(DblStk);

    StackElem_t *temp = (StackElem_t *)realloc(DblStk -> buf, (DblStk -> capacity + 2) * sizeof(StackElem_t) / RESIZE_COEFF);
    if (temp)
    {
        DblStk -> buf = temp;
        DblStk -> data = temp + 1;
    }

    err = StackVerify(DblStk);
    if (err)
        return err;
 
    DblStk -> capacity /= RESIZE_COEFF;
    *(DblStk -> data + DblStk -> capacity + 1) = CHICKEN_UPPER;

    err = StackVerify(DblStk);
    return err;
}