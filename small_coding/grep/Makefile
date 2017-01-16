CC = gcc
CFLAGS = -g -Wall -ansi -pedantic -Wstrict-prototypes -lm -D_GNU_SOURCE
BIN = match

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: main

%.o: %.c
	$(CC) $(CFLAGS) -c $<

main: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(BIN)

test: main
	@echo ''
	./$(BIN) test

clean:
	rm -f *.o $(BIN)

.PHONY: all clean

