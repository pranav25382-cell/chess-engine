#pragma once
#include "movegen.h"

int evaluate(Board &board);
int alphaBeta(Board &board, int depth, int alpha, int beta);
int get_best_move(Board &board, int depth, Move &best_move);