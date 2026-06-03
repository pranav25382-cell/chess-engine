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
                int from= (get_bit((pawns<<7)& not_h_file, to) ? to -7 : to -9);
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
                m.is_castling=false;
                m.is_en_passant=false;
                moves.push_back(m);
                temp &= temp -1;
            }
            temp=promo_capture_left;
            while(temp){
                int to = __builtin_ctzll(temp);
                int from = to  -7;
                Move m;
                m.from= from;
                m.to=to;
                m.piece=WHITE_PAWN;
                m.captured_piece = get_piece_on_square(board, to);
                m.promoted_piece=promo;
                m.is_castling= false;
                m.is_en_passant= false;
                moves.push_back(m);
                temp &= temp -1;
            }
            temp=promo_capture_right;
            while(temp){
                int to= __builtin_ctzll(temp);
                int from = to -9;
                Move m;
                m.from=from;
                m.to=to;
                m.piece= WHITE_PAWN;
                m.captured_piece= get_piece_on_square(board, to);
                m.promoted_piece= promo;
                m.is_castling=false;
                m.is_en_passant= false;
                moves.push_back(m);
                temp &=temp-1;
            }
        }
    }
    else{
        uint64_t one_step = (pawns >> 8) & empty;
        uint64_t rank6 = 0x0000FF0000000000ULL;
        uint64_t two_step = ((one_step & rank6) >> 8) & empty;
        // Convert one_step to moves
        uint64_t temp = one_step;
        while(temp){
            int to = __builtin_ctzll(temp);
            int from = to +8;
            Move m;
            m.from=from;
            m.to=to;
            m.piece= BLACK_PAWN;
            m.captured_piece=-1;
            m.promoted_piece=-1;
            m.is_castling=false;
            m.is_en_passant=false;
            moves.push_back(m);
            temp &= temp -1;
        }
        // Convert two_step to moves
        temp = two_step;
        while(temp){
            int to = __builtin_ctzll(temp);
            int from = to +16;
            Move m;
            m.from=from;
            m.to = to;
            m.piece= BLACK_PAWN;
            m.captured_piece=-1;
            m.promoted_piece=-1;
            m.is_castling=false;
            m.is_en_passant=false;
            moves.push_back(m);
            temp &= temp -1;
        }
        //Pawn captures
        uint64_t not_a_file = 0xFEFEFEFEFEFEFEFEULL;
        uint64_t not_h_file = 0x7F7F7F7F7F7F7F7FULL;
        uint64_t capture_left=(pawns>> 9) & enemies & not_h_file;
        uint64_t capture_right= (pawns >>7) & enemies & not_a_file;
        temp = capture_left;
        while (temp){
            int to = __builtin_ctzll(temp);
            int from = to +9;
            Move m;
            m.from = from;
            m.to = to;
            m.piece= BLACK_PAWN;
            m.captured_piece = get_piece_on_square(board,to);
            m.promoted_piece = -1;
            m.is_castling = false;
            m.is_en_passant = false;
            moves.push_back(m);
            temp &= temp-1;
        }
        temp = capture_right;
        while (temp){
            int to = __builtin_ctzll(temp);
            int from= to +7;
            Move m;
            m.from = from;
            m.to = to;
            m.piece= BLACK_PAWN;
            m.captured_piece = get_piece_on_square(board,to);
            m.promoted_piece = -1;
            m.is_castling = false;
            m.is_en_passant = false;
            moves.push_back(m);
            temp &= temp-1;
        }
        if(board.en_passant_square != -1){
            uint64_t ep_square = 1ULL << board.en_passant_square;
            uint64_t ep_capture = (pawns >> 9) & ep_square & not_h_file;
            ep_capture |= (pawns >> 7) & ep_square & not_a_file;
            temp= ep_capture;
            while(temp){
                int to = __builtin_ctzll(temp);
                int from = (get_bit((pawns >> 9) & not_h_file,to) ? to +9 : to +7);
                Move m; 
                m.from=from;
                m.to=to;    
                m.piece= BLACK_PAWN;
                m.captured_piece= WHITE_PAWN;
                m.promoted_piece=-1;
                m.is_castling=false;
                m.is_en_passant=true;
                moves.push_back(m);
                temp &= temp -1;
            }
        }
        
            uint64_t rank2 = 0x000000000000FF00ULL;
            uint64_t promo_step= (pawns & rank2)>>8 & empty;
            uint64_t promo_capture_left =(pawns & rank2)>>9 & enemies & not_h_file;
            uint64_t promo_capture_right =(pawns & rank2)>> 7 & enemies & not_a_file;
            int promo_pieces[]= {BLACK_QUEEN, BLACK_ROOK, BLACK_BISHOP, BLACK_KNIGHT};
            for (int promo : promo_pieces){ 
                temp= promo_step;
                while(temp){
                    int to = __builtin_ctzll(temp);
                    int from= to +8;
                    Move m;
                    m.from=from;
                    m.to=to;
                    m.piece= BLACK_PAWN;
                    m.captured_piece =-1;
                    m.promoted_piece = promo;
                    m.is_castling=false;
                    m.is_en_passant=false;
                    moves.push_back(m);
                    temp &= temp -1;
                }
                temp=promo_capture_left;
                while(temp){
                    int to = __builtin_ctzll(temp);
                    int from = to  +9;
                    Move m;
                    m.from= from;
                    m.to=to;
                    m.piece=BLACK_PAWN;
                    m.captured_piece = get_piece_on_square(board, to);
                    m.promoted_piece=promo;
                    m.is_castling= false;
                    m.is_en_passant= false;
                    moves.push_back(m);
                    temp &= temp -1;
                }
                temp=promo_capture_right;
                while(temp){
                    int to= __builtin_ctzll(temp);
                    int from = to +7;
                    Move m;
                    m.from=from;
                    m.to=to;
                    m.piece= BLACK_PAWN;
                    m.captured_piece= get_piece_on_square(board, to);
                    m.promoted_piece= promo;
                    m.is_castling=false;
                    m.is_en_passant= false;
                    moves.push_back(m);
                    temp &=temp-1;
                }
            }
        }
    }
void generate_knight_moves(Board &board, std :: vector<Move> &moves){
    uint64_t knights = board.white_to_move ? board.pieces[WHITE_KNIGHT] : board.pieces[BLACK_KNIGHT];
    uint64_t own_pieces = board.white_to_move ? get_white_pieces(board) : get_black_pieces(board);
    int piece = board.white_to_move ?WHITE_KNIGHT : BLACK_KNIGHT;
    uint64_t not_a_file  = 0xFEFEFEFEFEFEFEFEULL;
    uint64_t not_b_file  = 0xFDFDFDFDFDFDFDFDULL;
    uint64_t not_g_file  = 0xBFBFBFBFBFBFBFBFULL;
    uint64_t not_h_file  = 0x7F7F7F7F7F7F7F7FULL;
    uint64_t not_ab_file = not_a_file & not_b_file;
    uint64_t not_gh_file = not_g_file & not_h_file;
    while(knights){
        int from = __builtin_ctzll(knights);
        uint64_t k = 1ULL << from;
        uint64_t attacks = 
            ((k << 17) & not_a_file) |
            ((k << 15) & not_h_file) |
            ((k << 10) & not_ab_file) |
            ((k << 6) & not_gh_file) |
            ((k >> 17) & not_h_file) |
            ((k >> 15) & not_a_file) |
            ((k >> 10) & not_gh_file) |
            ((k >> 6) & not_ab_file);
        attacks &= ~own_pieces;
        while (attacks){
            int to =__builtin_ctzll(attacks);
            Move m;
            m.from=from;
            m.to= to;
            m.piece = piece;
            m.captured_piece = get_piece_on_square(board, to);
            m.promoted_piece = -1;
            m.is_castling = false;
            m.is_en_passant = false;
            moves.push_back(m);
            attacks &= attacks -1;
        }
        knights &= knights -1;
    }
}
    void slide_ray(Board &board, std::vector<Move> &moves, int from, int piece, int step,uint64_t mask) {
        uint64_t own_pieces = board.white_to_move ? get_white_pieces(board) : get_black_pieces(board);
        uint64_t enemies = board.white_to_move ? get_black_pieces(board) : get_white_pieces(board);
        
        int to = from+ step;
        uint64_t ray = 1ULL << to;
        while(to>=0 && to < 64 && ( ray & mask)){
            if(get_bit(own_pieces, to))break;
            Move m;
            m.from=from;
            m.to=to;
            m.piece=piece;
            m.captured_piece = get_piece_on_square(board, to);
            m.promoted_piece=-1;
            m.is_castling=false;
            m.is_en_passant=false;
            moves.push_back(m);
            if(get_bit(enemies,to)) break;
            to+=step;
            ray =1ULL<< to;
            }
        }
        void generate_bishop_moves(Board &board, std::vector<Move> &moves){
            uint64_t bishops = board.white_to_move ? board.pieces[WHITE_BISHOP] : board.pieces[BLACK_BISHOP];
            int piece = board.white_to_move ? WHITE_BISHOP : BLACK_BISHOP;
            uint64_t not_a_file = 0xFEFEFEFEFEFEFEFEULL;
            uint64_t not_h_file = 0x7F7F7F7F7F7F7F7FULL;
            uint64_t not_ab_file = not_a_file & 0xFDFDFDFDFDFDFDFDULL;
            uint64_t not_gh_file = not_h_file & 0xBFBFBFBFBFBFBFBFULL;
            while(bishops){ 
                int from = __builtin_ctzll(bishops);
                slide_ray(board, moves, from, piece, 9, not_a_file);
                slide_ray(board, moves, from, piece, 7, not_h_file);
                slide_ray(board, moves, from, piece, -7, not_a_file);
                slide_ray(board, moves, from, piece, -9, not_h_file);
                bishops &= bishops -1;
            }
        }
        void generate_rook_moves(Board &board, std::vector<Move> &moves) {
            uint64_t rooks = board.white_to_move ? board.pieces[WHITE_ROOK] : board.pieces[BLACK_ROOK];
            int piece = board.white_to_move ? WHITE_ROOK : BLACK_ROOK;
            
            uint64_t not_a_file = 0xFEFEFEFEFEFEFEFEULL;
            uint64_t not_h_file = 0x7F7F7F7F7F7F7F7FULL;

            while (rooks) {
                int from = __builtin_ctzll(rooks);
                slide_ray(board, moves, from, piece, 8,  0xFFFFFFFFFFFFFFFFULL); // north
                slide_ray(board, moves, from, piece, -8, 0xFFFFFFFFFFFFFFFFULL); // south
                slide_ray(board, moves, from, piece, 1,  not_h_file);            // east
                slide_ray(board, moves, from, piece, -1, not_a_file);            // west
                rooks &= rooks - 1;
            }
        }
        void generate_queen_moves(Board &board, std::vector<Move> &moves) {
            uint64_t queens = board.white_to_move ? board.pieces[WHITE_QUEEN] : board.pieces[BLACK_QUEEN];
            int piece = board.white_to_move ? WHITE_QUEEN : BLACK_QUEEN;
            
            uint64_t not_a_file = 0xFEFEFEFEFEFEFEFEULL;
            uint64_t not_h_file = 0x7F7F7F7F7F7F7F7FULL;

            while (queens) {
                int from = __builtin_ctzll(queens);
                // Diagonal (bishop-like)
                slide_ray(board, moves, from, piece, 9,  not_a_file);
                slide_ray(board, moves, from, piece, 7,  not_h_file);
                slide_ray(board, moves, from, piece, -7, not_a_file);
                slide_ray(board, moves, from, piece, -9, not_h_file);
                // Straight (rook-like)
                slide_ray(board, moves, from, piece, 8,  0xFFFFFFFFFFFFFFFFULL);
                slide_ray(board, moves, from, piece, -8, 0xFFFFFFFFFFFFFFFFULL);
                slide_ray(board, moves, from, piece, 1,  not_h_file);
                slide_ray(board, moves, from, piece, -1, not_a_file);
                queens &= queens - 1;
            }
        }
        void generate_king_moves(Board &board, std::vector<Move> &moves) {
            uint64_t king = board.white_to_move ? board.pieces[WHITE_KING] : board.pieces[BLACK_KING];
            uint64_t own_pieces = board.white_to_move ? get_white_pieces(board) : get_black_pieces(board);
            int piece = board.white_to_move ? WHITE_KING : BLACK_KING;

            uint64_t not_a_file = 0xFEFEFEFEFEFEFEFEULL;
            uint64_t not_h_file = 0x7F7F7F7F7F7F7F7FULL;

            int from = __builtin_ctzll(king);
            uint64_t k = 1ULL << from;

            uint64_t attacks =
                ((k << 8))                    |  // north
                ((k >> 8))                    |  // south
                ((k << 1) & not_a_file)       |  // east
                ((k >> 1) & not_h_file)       |  // west
                ((k << 9) & not_a_file)       |  // northeast
                ((k << 7) & not_h_file)       |  // northwest
                ((k >> 7) & not_a_file)       |  // southeast
                ((k >> 9) & not_h_file);         // southwest

            attacks &= ~own_pieces;

            while (attacks) {
                int to = __builtin_ctzll(attacks);
                Move m;
                m.from = from;
                m.to = to;
                m.piece = piece;
                m.captured_piece = get_piece_on_square(board, to);
                m.promoted_piece = -1;
                m.is_castling = false;
                m.is_en_passant = false;
                moves.push_back(m);
                attacks &= attacks - 1;
            }
        }
        void generate_all_moves(Board &board, std::vector<Move> &moves) {
            generate_pawn_moves(board, moves);
            generate_knight_moves(board, moves);
            generate_bishop_moves(board, moves);
            generate_rook_moves(board, moves);
            generate_queen_moves(board, moves);
            generate_king_moves(board, moves);
        }



                