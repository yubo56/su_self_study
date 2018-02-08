CC = ghc -dynamic
CCFLAGS = -Wall

4: 4structures_take2.bin
	./$<

3: 3structures.bin
	./$<

2: 2algs.bin
	./$<

1: 1types.bin
	./$<

0: 0hello_world.bin
	./$<

%.bin: %.o
	$(CC) $(CCFLAGS) -o $@ $<

%.o: %.hs
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.hi *.o *.bin

.PHONY: all clean

