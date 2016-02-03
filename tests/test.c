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

int _array[10] = {0};
void _processing(STACK_ITEM item)
{
    static int i = 0;
    _array[i++] = ((struct Number*)item)->x;
}

CUT_DEFINE_TEST(test_stack_for_each)
{
    STACK stack = stack_init(numberDelete, numberCopy);

    stack_push(stack, numberCreate(17));
    stack_push(stack, numberCreate(25));
    stack_push(stack, numberCreate(93));

    // Special cases
    CUT_CHECK(stack_for_each(NULL, STACK_TOP_POP_PROCESSING, processing) == STACK_NULL_POINTER);
    CUT_CHECK(stack_for_each(stack, STACK_TOP_POP_PROCESSING, NULL) == STACK_NULL_POINTER);

    CUT_CHECK(stack_for_each(stack, 123, processing) == STACK_INVALID_PROCESSING_TYPE);
    CUT_CHECK(stack_for_each(NULL, 346, NULL) != STACK_NO_ERROR);

    CUT_CHECK(stack_for_each(NULL, STACK_TOP_PROCESSING_POP, processing) == STACK_NULL_POINTER);
    CUT_CHECK(stack_for_each(stack, STACK_TOP_PROCESSING_POP, NULL) == STACK_NULL_POINTER);

    // Normal case - STACK_TOP_POP_PROCESSING
    CUT_CHECK(stack_for_each(stack, STACK_TOP_POP_PROCESSING, processing) == STACK_NO_ERROR);
    CUT_CHECK(stack_empty(stack) == STACK_EMPTY);

    CUT_CHECK(array[0] == 93);
    CUT_CHECK(array[1] == 25);
    CUT_CHECK(array[2] == 17);
    CUT_CHECK(array[3] == 0);

    stack_push(stack, numberCreate(167));
    stack_push(stack, numberCreate(326));
    stack_push(stack, numberCreate(795));

    // Normal case - STACK_TOP_PROCESSING_POP;
    CUT_CHECK(stack_for_each(stack, STACK_TOP_PROCESSING_POP, _processing) == STACK_NO_ERROR);
    CUT_CHECK(stack_empty(stack) == STACK_EMPTY);

    CUT_CHECK(_array[0] == 795);
    CUT_CHECK(_array[1] == 326);
    CUT_CHECK(_array[2] == 167);
    CUT_CHECK(_array[3] == 0);

    stack_delete(&stack);
}

//STACK_TOP_POP_PROCESSING
int ARRAY[10] = {0};
void PROCESSING(STACK_ITEM item, STACK stack, int y)
{
    static int i = 0;
    if (((struct Number*)item)->x == 133 && i == 1)
        stack_push(stack, numberCreate(y));

    ARRAY[i++] = ((struct Number*)item)->x;
}

//STACK_TOP_PROCESSING_POP
int _ARRAY[10] = {0};
void _PROCESSING(STACK_ITEM item, STACK stack)
{
    static int i = 0;
    if (((struct Number*)item)->x == 173 && i == 1)
        stack_push(stack, numberCreate(5));

    ARRAY[i++] = ((struct Number*)item)->x;
}

CUT_DEFINE_TEST(test_STACK_FOR_EACH)
{
    STACK stack = stack_init(numberDelete, numberCopy);

    //STACK_TOP_POP_PROCESSING
    stack_push(stack, numberCreate(167));
    stack_push(stack, numberCreate(133));
    stack_push(stack, numberCreate(795));

    STACK_FOR_EACH(stack, STACK_TOP_POP_PROCESSING, PROCESSING, stack, 100);

    CUT_CHECK(stack_empty(stack) == STACK_EMPTY);
    CUT_CHECK(ARRAY[0] == 795);
    CUT_CHECK(ARRAY[1] == 133);
    CUT_CHECK(ARRAY[2] == 100);
    CUT_CHECK(ARRAY[3] == 167);
    CUT_CHECK(ARRAY[4] == 0);

    //STACK_TOP_PROCESSING_POP
    stack_push(stack, numberCreate(292));
    stack_push(stack, numberCreate(173));
    stack_push(stack, numberCreate(135));

    STACK_FOR_EACH(stack, STACK_TOP_PROCESSING_POP, _PROCESSING, stack);

    CUT_CHECK(stack_empty(stack) == STACK_EMPTY);
    CUT_CHECK(ARRAY[0] == 135);
    CUT_CHECK(ARRAY[1] == 173);
    CUT_CHECK(ARRAY[2] == 173);
    CUT_CHECK(ARRAY[3] == 292);
    CUT_CHECK(ARRAY[4] == 0);

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
    CUT_CALL_TEST(test_STACK_FOR_EACH);
CUT_END_MAIN

