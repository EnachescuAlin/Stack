CFLAGS = -std=c99 -Wall
INCLUDE = -I../include

all: main

main: main.c
	gcc $(CFLAGS) -o $@ $< ../bin/libstack.a $(INCLUDE)

clean:
	rm main
