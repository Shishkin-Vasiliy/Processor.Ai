#include "Includes.h"
#include "Stack.h"

StackErr_t StackVerify(Stack_t *DblStk)
{
    #ifdef STACK_DEBUG

    if ((!DblStk) || !(DblStk -> buf))
        return STACK_BAD_PTR;

    StackErr_t err = StackStructChickenCheck(DblStk);
    if (err)
        return err;

    err = StackDataChickenCheck(DblStk);
    if (err)
        return err;

    size_t size = DblStk -> size;
    size_t capacity = DblStk -> capacity;

    if (size > capacity)
        return STACK_BAD_SIZE;

    #endif

    return STACK_OK;
}

int DblCmp(StackElem_t a, StackElem_t b)
{
    const double EPSILON = 1e-14;

    if (fabs(a - b) < EPSILON)
        return 0;
    else if (a > b)
        return 1;
    else
        return -1;
}

StackErr_t StackStructChickenCheck(Stack_t *DblStk)
{
    if (DblStk -> chicken_left != CHICKEN_LEFT) 
        return STACK_STRUCT_LEFT_CHICKEN_ATTACKED;
    else if (DblStk -> chicken_right != CHICKEN_RIGHT)
        return STACK_STRUCT_RIGHT_CHICKEN_ATTACKED;
    else
        return STACK_OK;
}

StackErr_t StackDataChickenCheck(Stack_t *DblStk)
{
    size_t capacity = DblStk -> capacity;

    if (DblCmp(*(DblStk -> buf), CHICKEN_LOWER) != 0 || isnan(*(DblStk -> buf)))
        return STACK_DATA_LOWER_CHICKEN_ATTACKED;
    else if (DblCmp(*(DblStk -> buf + capacity + 1), CHICKEN_UPPER) != 0 || isnan(*(DblStk -> buf + capacity + 1)))
        return STACK_DATA_UPPER_CHICKEN_ATTACKED;
    else
        return STACK_OK;
}

StackErr_t StackIsEmpty(Stack_t *DblStk)
{
    #ifdef STACK_DEBUG

    if (!DblStk)   
        return STACK_BAD_PTR;
    
    StackErr_t err = StackStructChickenCheck(DblStk);
    if (err)
        return err;

    size_t size = DblStk -> size;
    size_t capacity = DblStk -> capacity;
    
    if (size != 0 && capacity != 0)
        return EMPTY_STACK_BAD_SIZE;
    else
        return STACK_OK;

    #else
    return STACK_OK;
    #endif
}


void StackDump(Stack_t *DblStk)
{
    #ifdef STACK_DEBUG

    const char *dump_file_name = "dump.txt";
    FILE *dump_file_ptr = fopen(dump_file_name, "w");

    const char *name = DblStk -> stack_name;
    StackElem_t *buf = DblStk -> buf;
    StackElem_t *data = DblStk -> data;
    const char *func = DblStk -> func_name;
    const char *file = DblStk -> file_name;
    int line = DblStk -> line;
    const char *time = DblStk -> time;
    const char *date = DblStk -> date;
    size_t capacity = DblStk -> capacity;
    size_t size = DblStk -> size;
    unsigned long long chicken_left = DblStk -> chicken_left;
    unsigned long long chicken_right = DblStk -> chicken_right;

    fprintf(dump_file_ptr, "STACK_DUMP\n");
    fprintf(dump_file_ptr, "****************************************\n");
    fprintf(dump_file_ptr, "Stack_t <%s> [%p], %s at %s: %d\n", name, DblStk, func, file, line);
    fprintf(dump_file_ptr, "Created at %s on %s\n", time, date);
    fprintf(dump_file_ptr, "{\n");
    fprintf(dump_file_ptr, "chicken_left = %llX\n", chicken_left);
    fprintf(dump_file_ptr, "chicken_left = %llX\n", chicken_right);
    fprintf(dump_file_ptr, "capacity     = %lu\n", capacity);
    fprintf(dump_file_ptr, "size         = %lu\n", size);
    fprintf(dump_file_ptr, "buf[%p]\n", buf);
    if (buf)
    {
        fprintf(dump_file_ptr, "    {\n");
        fprintf(dump_file_ptr, "    [ChickenLower] = %lg\n", buf[0]);
        for (size_t i = 0; i < size; i++)
            fprintf(dump_file_ptr, "    *[%lu] = %lg\n", i, data[i]);
        for (size_t j = size; j < capacity; j++)
            fprintf(dump_file_ptr, "     [%lu] = %lg (POIZON)\n", j, data[j]);
        fprintf(dump_file_ptr, "    [ChickenUpper] = %lg\n", buf[capacity + 1]);
        fprintf(dump_file_ptr, "    }\n");
    }
    fprintf(dump_file_ptr, "}\n");
    fprintf(dump_file_ptr, "****************************************\n\n");

    fclose(dump_file_ptr);

    #endif
}

void StackErrMsg(StackErr_t err)
{
    switch(err)
    {
        case STACK_OK:
            break;

        case STACK_BAD_PTR:
            printf("Error: failed to allocate memory for stack\n");
            break;
        
        case EMPTY_STACK_BAD_SIZE:
            printf("Error: (invalid Stack) empty Stack has capacity and size unequal to 0\n");
            break;

        case STACK_BAD_SIZE:
            printf("Error: size > capacity\n");
            break;

        case STACK_STRUCT_LEFT_CHICKEN_ATTACKED:
            printf("Danger: Stack Struct left chicken got attacked\n");
            break;
            
        case STACK_STRUCT_RIGHT_CHICKEN_ATTACKED:
            printf("Danger: Stack Struct left chicken got attacked\n");
            break;

        case STACK_DATA_LOWER_CHICKEN_ATTACKED:
            printf("Danger: Stack Data lower chicken got attacked\n");
            break;

        case STACK_DATA_UPPER_CHICKEN_ATTACKED:
            printf("Danger: Stack Data upper chicken got attacked\n");
            break;

        case STACK_UNDERFLOW:
            printf("Stack UnderFlow: cannot do pop() from an empty Stack\n");
            break;

        case STACK_OVERFLOW:
            printf("Stack OverFlow: cannot push() to an overflowing Stack\n");
            break;

        default:
            ;
    }
}
