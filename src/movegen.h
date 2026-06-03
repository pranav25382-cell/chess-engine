#pragma once
#include "board.h"
#include <vector>

void generate_pawn_moves(Board &board, std::vector<Move> &moves);
void generate_knight_moves(Board &board, std::vector<Move> &moves);
void generate_bishop_moves(Board &board, std::vector<Move> &moves);
void generate_rook_moves(Board &board, std::vector<Move> &moves);
void generate_queen_moves(Board &board, std::vector<Move> &moves);
void generate_king_moves(Board &board, std::vector<Move> &moves);
void generate_all_moves(Board &board, std::vector<Move> &moves);
void slide_ray(Board &board, std::vector<Move> &moves, int from, int piece, int step);
void generate_legal_moves(Board &board, std::vector<Move> &moves);