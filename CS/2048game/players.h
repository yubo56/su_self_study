#ifndef PLAYERS_H_R1F0JVSO
#define PLAYERS_H_R1F0JVSO

#include <stdlib.h>
#include <unistd.h>
#include "board.h"

void autoplay_rand(GameState* game, int to_display, int lookahead);
void autoplay_basic(GameState* game, int to_display, int lookahead);
void autoplay_heuristic(GameState* game, int to_display, int lookahead);

#endif
