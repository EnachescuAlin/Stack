#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

char names[5][10] = {"Name1", "Name2", "Name3", "Name4", "Name5"};

struct Student
{
    char *name;
    size_t age;
};

void ProcessesStudent(void *st)
{
    struct Student *student = st;
    printf("%s: %zd\n", student->name, student->age);
}

void _ProcessesStudent(void *st, int number)
{
    struct Student *student = st;
    printf("%s: %zd %d\n", student->name, student->age, number);
}

void __ProcessesStudent(void *st, int number, char *str)
{
    struct Student *student = st;
    printf("%s: %zd %d %s\n", student->name, student->age, number, str);
}

void* CopyStudent(const void *st)
{
    const struct Student *student = st;
    struct Student *newStudent = malloc(sizeof(struct Student));

    newStudent->name = malloc(strlen(student->name) + 1);
    newStudent->age  = student->age;
    memcpy(newStudent->name, student->name, strlen(student->name) + 1);

    return newStudent;
}

void DeleteStudent(void *st)
{
    struct Student *student = st;
    free(student->name);
    free(st);
}

void* CreateStudent(int i)
{
    struct Student *student = malloc(sizeof(struct Student));
    student->name = malloc(strlen(names[i]) + 1);
    memcpy(student->name, names[i], strlen(names[i]) + 1);
    student->age = 18 + i;

    return student;
}

int main(void)
{
    STACK stack = stack_init(DeleteStudent, (stack_copyItemFn)CopyStudent);

    for (int i = 0; i < 5; i++)
        stack_push(stack, CreateStudent(i));

    STACK newStack = stack_copy(stack);
    STACK_FOR_EACH(stack, TOP_PROCESSING_POP, ProcessesStudent);
    printf("\n\n\n");
    STACK_FOR_EACH(newStack, TOP_PROCESSING_POP, _ProcessesStudent, 5);

    stack_delete(&stack);
    stack_delete(&newStack);

    stack = stack_init(DeleteStudent, NULL);
    for (int i = 0; i < 5; i++)
        stack_push(stack, CreateStudent(i));

    while (1)
    {
        STACK_ITEM si = stack_top(stack);
        struct Student *student = (struct Student*) si;
        if (student->age == 18)
        {
            printf("Found\n");
            break;
        }
        stack_pop(stack);
    }

    stack_delete(&stack);

    return 0;
}

