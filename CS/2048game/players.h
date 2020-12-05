#ifndef PLAYERS_H_R1F0JVSO
#define PLAYERS_H_R1F0JVSO

#include "board.h"
#include <unistd.h>

void autoplay_rand(GameState* game, int to_display, int lookahead);
void autoplay_basic(GameState* game, int to_display, int lookahead);
void autoplay_heuristic(GameState* game, int to_display, int lookahead);

#endif /* end of include guard: PLAYERS_H_R1F0JVSO */
