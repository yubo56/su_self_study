/* NB: bit of repeated code in here, might be able to abstract using fptrs but
 * fptrs are slightly slower than compile-time duplication (c++ templates avoids
 * this). Alt is to do something clever with #define's, but this is a short proj
 */
#include "players.h"
#define MAX_SCORE 1e9
#define DISP_DELAY 10000

void autoplay_rand(GameState* game, int to_display, int _lookahead)
{
    int num_legals, move;
    int directions[4];

    num_legals = get_legal_moves(game->board, directions);
    while (num_legals > 0)
    {
        move = directions[arc4random_uniform(num_legals)];
        move_board(game, move);
        num_legals = get_legal_moves(game->board, directions);
        if (to_display == 1)
        {
            print_board(game);
            printw("Playing %c\n", DIRECTIONS[move]);
            refresh();
            usleep(DISP_DELAY);
        }
    }
}

/* 3-ply DFS lookahead, minmax w/ heuristic: maximize largest square */
int get_score(GameState* game, int num_ply, int (*scorer)(GameState* game))
{
    GameState child;
    int i, j, num_legals, num_empties, score, score2;
    int directions[4];
    char* empties[LEN * LEN];

    if (num_ply <= 0)
    {
        return scorer(game);
    }

    /* maximize score, minimizing over each of empties (place 2s only) */
    score = 0;
    num_legals = get_legal_moves(game->board, directions);
    for (i = 0; i < num_legals; i++)
    {
        memcpy(&child, game, sizeof(GameState));
        merge_board(child.board, directions[i], &(child.score));
        num_empties = get_empties(child.board, empties);

        score2 = MAX_SCORE;
        for (j = 0; j < num_empties; j++)
        {
            (*empties[j]) = 1;
            score2 = MIN(get_score_basic(&child, num_ply - 1), score2);
            (*empties[j]) = 0;
        }
        score = MAX(score, score2);
    }
    return score;
}
int get_score_from_game(GameState* game) { return game->score; }

int get_score_basic(GameState* game, int num_ply)
{
    return get_score(game, num_ply, get_score_from_game);
}
int get_best_move(GameState* game, int* directions, int num_legals, int lookahead)
{
    GameState child;
    int i, j, best_move, best_score, score2, num_empties;
    char* empties[LEN * LEN];

    best_move = 0;
    best_score = LEN * LEN;
    for (i = 0; i < num_legals; i++)
    {
        memcpy(&child, game, sizeof(GameState));
        merge_board(child.board, directions[i], &(child.score));

        num_empties = get_empties(child.board, empties);

        score2 = 0;
        for (j = 0; j < num_empties; j++)
        {
            (*empties[j]) = 1;
            score2 = MAX(get_score_basic(&child, lookahead - 1), score2);
            (*empties[j]) = 0;
        }
        if (score2 > best_score)
        {
            best_score = score2;
            best_move = i;
        }
    }
    return best_move;
}
void autoplay_basic(GameState* game, int to_display, int lookahead)
{
    int num_legals, best_move;
    int directions[4];

    num_legals = get_legal_moves(game->board, directions);
    while (num_legals > 0)
    {
        best_move = get_best_move(game, directions, num_legals, lookahead);
        move_board(game, directions[best_move]);
        if (to_display == 1)
        {
            clear();
            print_board(game);
            printw("Playing %c\n", DIRECTIONS[directions[best_move]]);
            refresh();
            usleep(DISP_DELAY);
        }

        num_legals = get_legal_moves(game->board, directions);
    }
}

/**

For random play (~1.66s):
- 1 runs with max cell 8 (0.01%)
- 12 runs with max cell 16 (0.12%)
- 524 runs with max cell 32 (5.24%)
- 3333 runs with max cell 64 (33.33%)
- 5113 runs with max cell 128 (51.13%)
- 1015 runs with max cell 256 (10.15%)
- 2 runs with max cell 512 (0.02%)

This seems to match the SO result somebody got:
https://math.stackexchange.com/questions/727076/probability-that-random-moves-in-the-game-2048-will-win

For 2-ply basic (~12s),
3 runs with max cell 128 (0.30%)
66 runs with max cell 256 (6.60%)
512 runs with max cell 512 (51.20%)
412 runs with max cell 1024 (41.20%)
7 runs with max cell 2048 (0.70%)

For 3-ply basic (~40s)
29 runs with max cell 512 (29.00%)
57 runs with max cell 1024 (57.00%)
14 runs with max cell 2048 (14.00%)

**/
