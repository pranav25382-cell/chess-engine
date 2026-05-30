#pragma once
#include <cstdint>
#include <vector>
enum Piece{
    WHITE_PAWN,WHITE_KNIGHT,WHITE_BISHOP,WHITE_ROOK,WHITE_QUEEN,WHITE_KING,
    BLACK_PAWN,BLACK_KNIGHT,BLACK_BISHOP,BLACK_ROOK,BLACK_QUEEN,BLACK_KING,PIECE_COUNT
};//This is numbered such that whitepawn is 0 and blackking is 11, which allows us to easily determine the color of a piece by checking if its index is less than 6 (white) or not (black).
struct Board{
    uint64_t pieces[PIECE_COUNT];
    bool white_to_move;
    bool castling_rights[4];
    int en_passant_square;
    int halfmove_clock;
    int fullmove_number;
};// enpassant -1 if cant be done otherwise the square index of the pawn that can be captured en passant. castling_rights[0] = white kingside, castling_rights[1] = white queenside, castling_rights[2] = black kingside, castling_rights[3] = black queenside
//setting a bit on a square
inline void set_bit(uint64_t &board, int square){
    board |= (1ULL << square);
}
//clearing a bit on a square
inline void clear_bit(uint64_t &board, int square){
    board &= ~(1ULL << square);
}
inline bool get_bit(uint64_t board, int square){
    return (board >> square) & 1;
}
void init_board(Board &board);
void print_board(const Board &board);
uint64_t get_white_pieces(Board &board);
uint64_t get_black_pieces(Board &board); 
uint64_t get_all_pieces(Board &board);
uint64_t get_empty_squares(Board &board);
struct Move{
    int from;
    int to;
    int piece;
    int captured_piece; // -1 if no piece captured
    int promoted_piece; // -1 if none
    bool is_castling;
    bool is_en_passant;
};
int get_piece_on_square(Board &board,int square);
