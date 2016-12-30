CC = gcc
CFLAGS = -g -Wall -ansi -pedantic -Wstrict-prototypes -lm
BIN = match

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: clean main run

%.o: %.c
	$(CC) $(CFLAGS) -c $<

main: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(BIN)

run: main
	@echo ''
	./$(BIN)

clean:
	rm -f *.o $(BIN)

.PHONY: all clean

