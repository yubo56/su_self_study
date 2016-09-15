CC = gcc
CFLAGS = -g -ansi -pedantic

#command: dependencies
#	 $(CC) $(CFLAGS) programs.c

all: main

main: newton.o main.c
	$(CC) $^ -o $@ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o main
			
.PHONY: all clean

