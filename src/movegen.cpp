#include "movegen.h"

void generate_pawn_moves(Board &board, std::vector<Move> &moves){
    //TODO
    uint64_t pawns = board.white_to_move ? board.pieces[WHITE_PAWN] : board.pieces[BLACK_PAWN];
    uint64_t empty = get_empty_squares(board);
    uint64_t enemies = board.white_to_move ? get_black_pieces(board) : get_white_pieces(board);
    if (board.white_to_move){
        //one square forward
        uint64_t one_step = (pawns << 8) & empty;
        //two squares forward
        uint64_t rank3 = 0x0000000000FF0000ULL;
        uint64_t two_step = ((one_step & rank3) << 8) & empty;
        
        // Convert one_step to moves
        uint64_t temp = one_step;
        while (temp) {
            int to = __builtin_ctzll(temp);
            int from = to - 8;  // pawn came from one rank below
            Move m;
            m.from = from;
            m.to = to;
            m.piece = WHITE_PAWN;
            m.captured_piece = -1;
            m.promoted_piece = -1;
            m.is_castling = false;
            m.is_en_passant = false;
            moves.push_back(m);
            temp &= temp - 1;
        }
        // Convert two_step to moves
        temp = two_step;
        while (temp) {
            int to = __builtin_ctzll(temp);
            int from = to - 16;  // pawn came from two ranks below
            Move m;
            m.from = from;
            m.to = to;
            m.piece = WHITE_PAWN;
            m.captured_piece = -1;
            m.promoted_piece = -1;
            m.is_castling = false;
            m.is_en_passant = false;
            moves.push_back(m);
            temp &= temp - 1;
        }
    }
}