CC = ghc

SRCS=$(wildcard *.hs)
BINS=$(SRCS:%.hs=%.bin)

all: 0

.PHONY: 0
0: 0hello_world.hs
	ghc $< -o $(<:.hs=.bin)
	./$(<:.hs=.bin)

clean:
	rm -f *.hi *.o $(BINS)

.PHONY: all clean

