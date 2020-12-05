#ifndef PLAYERS_H_R1F0JVSO
#define PLAYERS_H_R1F0JVSO

#include "board.h"
#include <unistd.h>

void autoplay_rand(GameState* game, int to_display);
void autoplay_basic(GameState* game, int to_display);
int get_best_move(GameState* game, int* directions, int num_legals);
int get_score_basic(GameState* game, int num_ply);

#endif /* end of include guard: PLAYERS_H_R1F0JVSO */
