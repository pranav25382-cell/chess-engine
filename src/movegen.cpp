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
        //Pawn captures
        uint64_t not_a_file = 0xFEFEFEFEFEFEFEFEULL;
        uint64_t not_h_file = 0x7F7F7F7F7F7F7F7FULL;
        uint64_t capture_left=(pawns<< 7) & enemies & not_h_file;
        uint64_t capture_right= (pawns <<9) &enemies & not_a_file;
        temp = capture_left;
        while (temp){
            int to = __builtin_ctzll(temp);
            int from = to -7;
            Move m;
            m.from = from;
            m.to=  to;
            m.piece =WHITE_PAWN;
            m.captured_piece = get_piece_on_square(board,to);
            m.promoted_piece = -1;
            m.is_castling = false;
            m.is_en_passant = false;
            moves.push_back(m);
            temp &= temp - 1;
        }
        temp= capture_right;
        while(temp){
            int to=__builtin_ctzll(temp);
            int from = to -9;
            Move m;
            m.from=from;
            m.to= to;
            m.piece= WHITE_PAWN;
            m.captured_piece= get_piece_on_square(board, to);
            m.promoted_piece =-1;
            m.is_castling = false;
            m.is_en_passant = false;
            moves.push_back(m);
            temp &= temp-1;
        }
        if(board.en_passant_square != -1){
            uint64_t ep_square = 1ULL << board.en_passant_square;
            uint64_t ep_capture = (pawns<<7) & ep_square & not_h_file;
            ep_capture |= (pawns<<9) & ep_square & not_a_file;
            temp=ep_capture;
            while(temp){
                int to= __builtin_ctzll(temp);
                int from= (ep_square == (pawns<<7) & ep_square & not_h_file) ? to -7 : to -9;
                Move m;
                m.from=from;
                m.to= to;
                m.piece=WHITE_PAWN;
                m.captured_piece= BLACK_PAWN;
                m.promoted_piece=-1;
                m.is_castling=false;
                m.is_en_passant=true;
                moves.push_back(m);
                temp &= temp -1;
        }
        uint64_t rank7 = 0x00FF000000000000ULL;
        uint64_t promo_step= (pawns & rank7)<<8 & empty;
        uint64_t promo_capture_left =(pawns & rank7)<<7 & enemies & not_h_file;
        uint64_t promo_capture_right =(pawns & rank7)<< 9 & enemies & not_a_file;
        int promo_pieces[]= {WHITE_QUEEN, WHITE_ROOK, WHITE_BISHOP, WHITE_KNIGHT};
        for (int promo : promo_pieces){
            temp= promo_step;
            while(temp){
                int to = __builtin_ctzll(temp);
                int from= to -8;
                Move m;
                m.from=from;
                m.to=to;
                m.piece= WHITE_PAWN;
                m.captured_piece =-1;
                m.promoted_piece = promo;
                moves.push_back(m);
            }
        }
    }
}