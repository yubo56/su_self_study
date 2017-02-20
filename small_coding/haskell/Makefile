CC = ghc

1: 1*.bin
	./$<

0: 0*.bin
	./$<

%.bin: %.o
	ghc -o $@ $<

%.o: %.hs
	ghc -c $<

clean:
	rm -f *.hi *.o *.bin

.PHONY: all clean

