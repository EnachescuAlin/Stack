#ifndef PHILIB_STACK_H
#define PHILIB_STACK_H

typedef void (*stack_processesITEM)(void*);
typedef void (*stack_freeITEM)(void*);
typedef void* (*stack_copyITEM)(const void*);

struct Stack
{
    struct StackNode
    {
        struct StackNode *next;
        void *info;
    } *first;
    stack_freeITEM free;
    stack_copyITEM copy;
};

#define STACK       struct Stack*
#define STACK_CODE  int
#define STACK_ITEM  void*

STACK       stack_init(stack_freeITEM, stack_copyITEM);
STACK       stack_copy(const STACK);
STACK_CODE  stack_pop(STACK);
STACK_CODE  stack_push(STACK, STACK_ITEM);
STACK_CODE  stack_empty(const STACK);
STACK_CODE  stack_delete(STACK*);
STACK_CODE  stack_for_each(STACK, stack_processesITEM);
STACK_ITEM  stack_top(const STACK);

/*
 * errors codes
 */
#define STACK_NULL_POINTER    0
#define STACK_NOT_EMPTY       0
#define STACK_EMPTY           1
#define STACK_NO_ERROR        2
#define STACK_MALLOC_ERROR    3

#define STACK_FOR_EACH(stack, func, ...)                    \
        do                                                  \
        {                                                   \
            while (stack_empty(stack) == STACK_NOT_EMPTY)   \
            {                                               \
                func(stack_top(stack), ##__VA_ARGS__);      \
                stack_pop(stack);                           \
            }                                               \
        }                                                   \
        while (0)

#endif

