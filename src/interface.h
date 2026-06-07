#pragma once
#include "search.h"
#include <string>

int square_from_string(const std::string &s);
std::string square_to_string(int square);
void play_game(Board &board, int depth);