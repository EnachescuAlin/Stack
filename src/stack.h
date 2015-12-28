#ifndef PHILIB_STACK_H
#define PHILIB_STACK_H

#include <stdlib.h>

typedef void (*stack_processesITEM)(void*);
typedef void (*stack_freeITEM)(void*);

struct Stack
{
    struct Node
    {
        struct Node *next;
        void *info;
    } *first;
    stack_freeITEM free;
};

#define STACK       struct Stack*
#define STACK_CODE  int
#define STACK_ITEM  void*

STACK       stack_init(stack_freeITEM);
STACK_CODE  stack_pop(STACK);
STACK_CODE  stack_push(STACK, STACK_ITEM, size_t);
STACK_CODE  stack_empty(const STACK);
STACK_CODE  stack_delete(STACK*);
STACK_CODE  stack_for_each(STACK, stack_processesITEM);
STACK_ITEM  stack_top(const STACK);

/*
 * errors codes
 */
#define NULL_POINTER    0
#define NOT_EMPTY       0
#define EMPTY           1
#define NO_ERROR        2
#define MALLOC_ERROR    3

#define STACK_FOR_EACH(stack, func, ...)                \
        do                                              \
        {                                               \
            while (stack_empty(stack) == NOT_EMPTY)     \
            {                                           \
                func(stack_top(stack), ##__VA_ARGS__);  \
                stack_pop(stack);                       \
            }                                           \
        }                                               \
        while (0)

#endif

