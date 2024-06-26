#ifndef BOARD_H_abcabc
#define BOARD_H_abcabc

#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define LEN 4
#define MAX(a, b) (a) > (b) ? (a) : (b)
#define MIN(a, b) (a) < (b) ? (a) : (b)

extern const char DIRECTIONS[5];
typedef char Board[LEN * LEN]; /* board is 4x4 */
typedef struct _GameState
{
    Board board;
    int score;
    int last_direction;
} GameState;

void reset_game(GameState* game);
void copy_game(const GameState* src, GameState* dest);
void print_board(GameState* state);
void print_legals(GameState* state);

/* 2 steps to interacting w/ a board: (i) merge in a direction & updates core
 * (ii) add pieces
 */
char merge_board(Board board, int direction, int* game_score);
int get_empties(Board board, char** empties);

/* given a game state, copy board to temp, try all directions except last,
 * populate legal ones in directions and return the # of legal directions */
int get_legal_moves(const Board game, int* directions);

/* actual complete step of moving the board */
void move_board(GameState* game, int direction);

#endif
