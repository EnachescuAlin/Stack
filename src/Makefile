CFLAGS = -std=c99 -Wall -c
INCLUDE = -I../include

all: stack.o libstack.a libstack.la

stack.o: stack.c
	mkdir -p ../bin
	gcc $(CFLAGS) -o ../bin/stack.o stack.c $(INCLUDE)

libstack.a:
	ar rcs ../bin/libstack.a ../bin/stack.o

libstack.la:
	ar rcs ../bin/libstack.la ../bin/stack.o

clean:
	rm ../bin/stack.o ../bin/libstack.a ../bin/libstack.la
