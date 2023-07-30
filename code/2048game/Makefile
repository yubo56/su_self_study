# shorten commands
GCC=gcc -Wstrict-prototypes -ansi -pedantic -D_GNU_SOURCE
build_fast:
	${GCC} board.c players.c 2048game.c -Ofast -lm -lcurses -o 2048game
