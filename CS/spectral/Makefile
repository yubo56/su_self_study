BIN = main

CC = g++
CFLAGS = -g -Wall
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

.PHONY: all
all: $(BIN) run

.PHONY: cleanrun
cleanrun: clean $(BIN) run

.PHONY: run
run: $(BIN)
	@echo ''
	./$(BIN)

.PHONY: clean
clean:
	rm -f *.o $(BIN)

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(BIN)
