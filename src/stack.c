#include "stack.h"
#include <string.h>

CODE stack_delete(STACK *stack)
{
    if (stack == NULL)
        return NULL_POINTER;
    if (*stack == NULL)
        return NULL_POINTER;

    while (stack_empty(*stack) == NOT_EMPTY)
        stack_pop(*stack);

    free(*stack);
    *stack = NULL;
}

STACK stack_init(FreeITEM freeItem)
{
    STACK stack = (STACK) malloc(sizeof(struct Stack));
    stack->first = NULL;
    stack->free = freeItem;

    return stack;
}

CODE stack_empty(const STACK stack)
{
    if (stack == NULL)
        return NULL_POINTER;

    if (stack->first == NULL)
        return EMPTY;
    else
        return NOT_EMPTY;
}

CODE stack_push(STACK stack, ITEM item, size_t bytes)
{
    if (stack == NULL)
        return NULL_POINTER;
    if (item == NULL)
        return NULL_POINTER;

    struct Node *node = (struct Node*) malloc(sizeof(struct Node));
    if (node == NULL)
        return MALLOC_ERROR;

    if (bytes == 0)
    {
        node->info = item;
        node->next = stack->first;
        stack->first = node;

        return NO_ERROR;
    }

    node->info = malloc(bytes);
    if (node->info == NULL)
        return MALLOC_ERROR;

    memcpy(node->info, item, bytes);
    node->next = stack->first;
    stack->first = node;

    return NO_ERROR;
}

CODE stack_pop(STACK stack)
{
    if (stack == NULL)
        return NULL_POINTER;

    if (stack->first == NULL)
        return NULL_POINTER;

    struct Node *node = stack->first;
    stack->first = stack->first->next;

    if (stack->free != NULL)
        stack->free(node->info);

    node->info = NULL;
    free(node);
    node = NULL;

    return NO_ERROR;
}

ITEM stack_top(const STACK stack)
{
    if (stack == NULL)
        return NULL;

    if (stack->first == NULL)
        return NULL;

    return stack->first->info;
}

CODE stack_for_each(STACK stack, ProcessesITEM func)
{
    if (stack == NULL)
        return NULL_POINTER;

    while (stack_empty(stack) == NOT_EMPTY)
    {
        func(stack_top(stack));
        stack_pop(stack);
    }
}

