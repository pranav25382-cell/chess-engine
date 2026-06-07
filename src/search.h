#pragma once
#include "movegen.h"

int evaluate(Board &board);
int minimax(Board &board, int depth);
int get_best_move(Board &board, int depth, Move &best_move);