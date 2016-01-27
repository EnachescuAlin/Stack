#include <cut.h>
#include <stack.h>
#include <stdlib.h>

struct Number
{
    int x;
};

struct Number* numberCreate(int x)
{
    struct Number *number = malloc(sizeof(struct Number));
    number->x = x;

    return number;
}

void numberDelete(STACK_ITEM number)
{
    free(number);
}

STACK_ITEM numberCopy(const STACK_ITEM number)
{
    struct Number *_number = malloc(sizeof(struct Number));
    _number->x = ((struct Number*)number)->x;

    return _number;
}

CUT_DEFINE_TEST(test_stack_init)
{
    STACK stack;

    stack = stack_init(NULL, NULL);
    CUT_CHECK(stack != NULL);
    CUT_CHECK(stack_empty(stack) == STACK_EMPTY);
    stack_delete(&stack);

    stack = stack_init(numberDelete, NULL);
    CUT_CHECK(stack != NULL);
    CUT_CHECK(stack_empty(stack) == STACK_EMPTY);
    stack_delete(&stack);

    stack = stack_init(NULL, numberCopy);
    CUT_CHECK(stack != NULL);
    CUT_CHECK(stack_empty(stack) == STACK_EMPTY);
    stack_delete(&stack);

    stack = stack_init(numberDelete, numberCopy);
    CUT_CHECK(stack != NULL);
    CUT_CHECK(stack_empty(stack) == STACK_EMPTY);
    stack_delete(&stack);
}

CUT_DEFINE_TEST(test_stack_delete)
{
    STACK stack = NULL;

    CUT_CHECK(stack_delete(NULL) == STACK_NULL_POINTER);
    CUT_CHECK(stack_delete(&stack) == STACK_NULL_POINTER);

    stack = stack_init(NULL, NULL);
    CUT_CHECK(stack_delete(&stack) == STACK_NO_ERROR);
    CUT_CHECK(stack == NULL);
}

CUT_DEFINE_TEST(test_stack_push)
{
    STACK stack = stack_init(numberDelete, numberCopy);
    STACK_ITEM item = numberCreate(5);

    CUT_CHECK(stack_push(NULL, NULL) != STACK_NO_ERROR);
    CUT_CHECK(stack_push(stack, NULL) == STACK_NULL_POINTER);
    CUT_CHECK(stack_push(NULL, item) == STACK_NULL_POINTER);
    CUT_CHECK(stack_push(stack, item) == STACK_NO_ERROR);
    CUT_CHECK(stack_empty(stack) == STACK_NOT_EMPTY);

    stack_delete(&stack);
}

CUT_DEFINE_TEST(test_stack_empty)
{
    STACK stack = stack_init(numberDelete, numberCopy);

    CUT_CHECK(stack_empty(NULL) == STACK_NULL_POINTER);
    CUT_CHECK(stack_empty(stack) == STACK_EMPTY);
    stack_push(stack, numberCreate(1));
    CUT_CHECK(stack_empty(stack) == STACK_NOT_EMPTY);

    stack_delete(&stack);
}

CUT_DEFINE_TEST(test_stack_top)
{
    STACK stack = stack_init(numberDelete, numberCopy);

    CUT_CHECK(stack_top(stack) == NULL);
    CUT_CHECK(stack_top(NULL) == NULL);
    stack_push(stack, numberCreate(1));
    stack_push(stack, numberCreate(2));
    stack_push(stack, numberCreate(3));
    CUT_CHECK(((struct Number*)stack_top(stack))->x == 3);

    stack_delete(&stack);
}

CUT_DEFINE_TEST(test_stack_pop)
{
    STACK stack = stack_init(numberDelete, numberCopy);

    CUT_CHECK(stack_pop(stack) == STACK_EMPTY);
    CUT_CHECK(stack_pop(NULL) == STACK_NULL_POINTER);
    stack_push(stack, numberCreate(1));
    stack_push(stack, numberCreate(2));
    stack_push(stack, numberCreate(3));
    CUT_CHECK(stack_pop(stack) == STACK_NO_ERROR);
    CUT_CHECK(((struct Number*)stack_top(stack))->x == 2);
    CUT_CHECK(stack_pop(stack) == STACK_NO_ERROR);
    CUT_CHECK(((struct Number*)stack_top(stack))->x == 1);
    CUT_CHECK(stack_pop(stack) == STACK_NO_ERROR);
    CUT_CHECK(stack_pop(stack) == STACK_EMPTY);

    stack_delete(&stack);
}

CUT_DEFINE_TEST(test_stack_copy)
{
    STACK stack = stack_init(NULL, NULL);
    CUT_CHECK(stack_copy(NULL) == NULL);
    CUT_CHECK(stack_copy(stack) == NULL);
    stack_delete(&stack);

    stack = stack_init(numberDelete, numberCopy);
    stack_push(stack, numberCreate(1));
    stack_push(stack, numberCreate(2));
    stack_push(stack, numberCreate(3));

    STACK newStack = stack_copy(stack);

    CUT_CHECK(stack != newStack);
    CUT_CHECK(stack != NULL);
    CUT_CHECK(((struct Number*)stack_top(newStack))->x == 3);
    stack_pop(newStack);
    CUT_CHECK(((struct Number*)stack_top(newStack))->x == 2);
    stack_pop(newStack);
    CUT_CHECK(((struct Number*)stack_top(newStack))->x == 1);
    stack_pop(newStack);
    CUT_CHECK(stack_pop(newStack) == STACK_EMPTY);

    stack_delete(&stack);
    stack_delete(&newStack);
}

CUT_DEFINE_TEST(test_stack_reverse)
{
    STACK stack = stack_init(numberDelete, numberCopy);

    CUT_CHECK(stack_reverse(NULL) == STACK_NULL_POINTER);
    CUT_CHECK(stack_reverse(stack) == STACK_EMPTY);

    stack_push(stack, numberCreate(1));
    stack_push(stack, numberCreate(2));
    stack_push(stack, numberCreate(3));

    CUT_CHECK(stack_reverse(stack) == STACK_NO_ERROR);
    CUT_CHECK(((struct Number*)stack_top(stack))->x == 1);
    stack_pop(stack);
    CUT_CHECK(((struct Number*)stack_top(stack))->x == 2);
    stack_pop(stack);
    CUT_CHECK(((struct Number*)stack_top(stack))->x == 3);
    stack_pop(stack);
    CUT_CHECK(stack_pop(stack) == STACK_EMPTY);

    stack_delete(&stack);
}

int array[10] = {0};
void processing(STACK_ITEM item)
{
    static int i = 0;
    array[i++] = ((struct Number*)item)->x;
}

CUT_DEFINE_TEST(test_stack_for_each)
{
    STACK stack = stack_init(numberDelete, numberCopy);

    stack_push(stack, numberCreate(1));
    stack_push(stack, numberCreate(2));
    stack_push(stack, numberCreate(3));

    CUT_CHECK(stack_for_each(NULL, STACK_TOP_POP_PROCESSING, processing) == STACK_NULL_POINTER);
    CUT_CHECK(stack_for_each(stack, STACK_TOP_POP_PROCESSING, NULL) == STACK_NULL_POINTER);
    CUT_CHECK(stack_for_each(stack, 123, processing) == STACK_INVALID_PROCESSING_TYPE);
    CUT_CHECK(stack_for_each(stack, STACK_TOP_POP_PROCESSING, processing) == STACK_NO_ERROR);
    CUT_CHECK(stack_empty(stack) == STACK_EMPTY);

    CUT_CHECK(array[0] == 3);
    CUT_CHECK(array[1] == 2);
    CUT_CHECK(array[2] == 1);
    CUT_CHECK(array[3] == 0);

    stack_delete(&stack);
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_stack_init);
    CUT_CALL_TEST(test_stack_delete);
    CUT_CALL_TEST(test_stack_push);
    CUT_CALL_TEST(test_stack_empty);
    CUT_CALL_TEST(test_stack_top);
    CUT_CALL_TEST(test_stack_pop);
    CUT_CALL_TEST(test_stack_copy);
    CUT_CALL_TEST(test_stack_reverse);
    CUT_CALL_TEST(test_stack_for_each);
CUT_END_MAIN

