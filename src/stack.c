#include "stack.h"
#include <string.h>

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

STACK stack_init(stack_freeITEM freeItem)
{
    STACK stack = (STACK) malloc(sizeof(struct Stack));
    stack->first = NULL;
    stack->free = freeItem;

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

STACK_CODE stack_push(STACK stack, STACK_ITEM item, size_t bytes)
{
    if (stack == NULL)
        return STACK_NULL_POINTER;
    if (item == NULL)
        return STACK_NULL_POINTER;

    struct StackNode *node = (struct StackNode*) malloc(sizeof(struct StackNode));
    if (node == NULL)
        return STACK_MALLOC_ERROR;

    if (bytes == 0)
    {
        node->info = item;
        node->next = stack->first;
        stack->first = node;

        return STACK_NO_ERROR;
    }

    node->info = malloc(bytes);
    if (node->info == NULL)
        return STACK_MALLOC_ERROR;

    memcpy(node->info, item, bytes);
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

STACK_CODE stack_for_each(STACK stack, stack_processesITEM func)
{
    if (stack == NULL)
        return STACK_NULL_POINTER;

    while (stack_empty(stack) == STACK_NOT_EMPTY)
    {
        func(stack_top(stack));
        stack_pop(stack);
    }
}

