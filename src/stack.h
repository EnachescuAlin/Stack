#ifndef PHILIB_STACK_H
#define PHILIB_STACK_H

#include <stdlib.h>

typedef void (*ProcessesITEM)(void*);
typedef void (*FreeITEM)(void*);

struct Stack
{
    struct Node
    {
        struct Node *next;
        void *info;
    } *first;
    FreeITEM free;
};

#define STACK struct Stack*
#define CODE int
#define ITEM void*

STACK stack_init(FreeITEM);

CODE stack_pop(STACK);
CODE stack_push(STACK, ITEM, size_t);
CODE stack_empty(const STACK);
CODE stack_delete(STACK*);
CODE stack_for_each(STACK, ProcessesITEM);

ITEM stack_top(const STACK);

/*
 * errors codes
 */
#define NULL_POINTER    0
#define NOT_EMPTY       0
#define EMPTY           1
#define NO_ERROR        2
#define MALLOC_ERROR    3

#define STACK_FOR_EACH(stack, func, ...)                \
            while (stack_empty(stack) == NOT_EMPTY)     \
            {                                           \
                func(stack_top(stack), ##__VA_ARGS__);  \
                stack_pop(stack);                       \
            }

#endif

