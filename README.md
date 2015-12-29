# Stack

In main.c is a small example how to use this library.

Notifications!
======
1. This stack was designed specifically for data structures and you need a function
that creates a data structure and allocates dynamically this structure. Also,
you need a functions that frees the data structure memory and this function
must passed as first argument in stack_init().
2. If the first parameter of stack_init() is NULL then you might have memory leaks.
3. You need a functions which make a copy of a data structure. This function
must passed as the second argument in stack_init(). This function is used when
you call stack_copy(), if you do not call stack_copy() then you can to passed
NULL as the second parameter for stack_init().
4. When is called stack_pop() will be freed the data structure memory which will
be removed from stack (just if exists a function that frees the data structure
memory).
5. When is called stack_delete(), if in stack exists items then they will be
removed and their memory will be freed (just if exists a function that frees
the data structure memory).
6. stack_push() adds in stack the pointer passed as argument. The function
doesn't make a copy of the data structure.
