CFLAGS  = -std=c99 -Wall -o
INCLUDE = -I../include
LIBS    = ../bin/libstack.a -lcut

all: test

test: test.c
	gcc $(CFLAGS) $@ $< $(LIBS) $(INCLUDE)

clean:
	rm test
