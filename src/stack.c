#include <string.h>
#include <stdlib.h>

#include "stack.h"

STACK_CODE stack_delete(STACK *stack)
{
    if (stack == NULL)
        return STACK_NULL_POINTER;
    if (*stack == NULL)
        return STACK_NULL_POINTER;

    while (stack_empty(*stack) == STACK_NOT_EMPTY)
        stack_pop(*stack);

    free(*stack);
    *stack = NULL;
}

STACK stack_init(stack_freeItemFn freeItem, stack_copyItemFn copyItem)
{
    STACK stack = (STACK) malloc(sizeof(struct Stack));
    stack->first = NULL;
    stack->free = freeItem;
    stack->copy = copyItem;

    return stack;
}

STACK_CODE stack_empty(const STACK stack)
{
    if (stack == NULL)
        return STACK_NULL_POINTER;

    if (stack->first == NULL)
        return STACK_EMPTY;
    else
        return STACK_NOT_EMPTY;
}

STACK_CODE stack_push(STACK stack, STACK_ITEM item)
{
    if (stack == NULL)
        return STACK_NULL_POINTER;
    if (item == NULL)
        return STACK_NULL_POINTER;

    struct StackNode *node = (struct StackNode*) malloc(sizeof(struct StackNode));
    if (node == NULL)
        return STACK_MALLOC_ERROR;

    node->info = item;
    node->next = stack->first;
    stack->first = node;

    return STACK_NO_ERROR;
}

STACK_CODE stack_pop(STACK stack)
{
    if (stack == NULL)
        return STACK_NULL_POINTER;

    if (stack->first == NULL)
        return STACK_NULL_POINTER;

    struct StackNode *node = stack->first;
    stack->first = stack->first->next;

    if (stack->free != NULL)
        stack->free(node->info);

    node->info = NULL;
    free(node);
    node = NULL;

    return STACK_NO_ERROR;
}

STACK_ITEM stack_top(const STACK stack)
{
    if (stack == NULL)
        return NULL;

    if (stack->first == NULL)
        return NULL;

    return stack->first->info;
}

STACK_CODE stack_for_each(STACK stack, stack_processesItemFn func)
{
    if (stack == NULL)
        return STACK_NULL_POINTER;

    while (stack_empty(stack) == STACK_NOT_EMPTY)
    {
        func(stack_top(stack));
        stack_pop(stack);
    }
}

STACK stack_copy(const STACK stack)
{
    if (stack->copy == NULL)
        return NULL;

    STACK newStack = (STACK) malloc(sizeof(struct Stack));
    newStack->first = NULL;
    newStack->free = stack->free;
    newStack->copy = stack->copy;

    if (stack->first)
    {
        newStack->first = (struct StackNode*) malloc(sizeof(struct StackNode));
        newStack->first->next = NULL;
        newStack->first->info = stack->copy(stack->first->info);
    }

    struct StackNode *sn  = stack->first->next;
    struct StackNode *nsn = newStack->first;

    while (sn)
    {
        nsn->next = (struct StackNode*) malloc(sizeof(struct StackNode));
        nsn->next->info = stack->copy(sn->info);
        nsn->next->next = NULL;
        nsn = nsn->next;
        sn  = sn->next;
    }

    return newStack;
}

