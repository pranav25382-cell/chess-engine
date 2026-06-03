#include "board.h"
#include <iostream>
using namespace std;
void init_board(Board &board){
    //clear everything 
    for(int i=0;i<PIECE_COUNT;i++){
        board.pieces[i] = 0ULL;
    }
    board.pieces[WHITE_PAWN] = 0x000000000000FF00ULL;
    board.pieces[WHITE_KNIGHT] = 0x0000000000000042ULL;
    board.pieces[WHITE_BISHOP] = 0x0000000000000024ULL;
    board.pieces[WHITE_ROOK] = 0x0000000000000081ULL;
    board.pieces[WHITE_QUEEN] = 0x0000000000000008ULL;
    board.pieces[WHITE_KING] = 0x0000000000000010ULL;
    board.pieces[BLACK_PAWN] = 0x00FF000000000000ULL;
    board.pieces[BLACK_KNIGHT] = 0x4200000000000000ULL;
    board.pieces[BLACK_BISHOP] = 0x2400000000000000ULL;
    board.pieces[BLACK_ROOK] = 0x8100000000000000ULL;
    board.pieces[BLACK_QUEEN] = 0x0800000000000000ULL;
    board.pieces[BLACK_KING] = 0x1000000000000000ULL;
    board.white_to_move = true;
    board.castling_rights[0] = board.castling_rights[1]= true;
    board.castling_rights[2] = board.castling_rights[3] = true;
    board.en_passant_square = -1;
    board.halfmove_clock = 0;
    board.fullmove_number = 1;
}
void print_board(const Board &board){
    cout << " \n";
    for(int rank = 7; rank >= 0; rank--){
        cout << rank + 1 << " ";
        for(int file = 0; file < 8; file++){
            int square = rank * 8 + file;
            char piece = '.';
            if (get_bit(board.pieces[WHITE_PAWN], square)) piece = 'P';
            else if (get_bit(board.pieces[WHITE_KNIGHT], square)) piece = 'N';
            else if (get_bit(board.pieces[WHITE_BISHOP], square)) piece = 'B';
            else if (get_bit(board.pieces[WHITE_ROOK], square)) piece = 'R';
            else if (get_bit(board.pieces[WHITE_QUEEN], square)) piece = 'Q';
            else if (get_bit(board.pieces[WHITE_KING], square)) piece = 'K';
            else if (get_bit(board.pieces[BLACK_PAWN], square)) piece = 'p';
            else if (get_bit(board.pieces[BLACK_KNIGHT], square)) piece = 'n';
            else if (get_bit(board.pieces[BLACK_BISHOP], square)) piece = 'b';
            else if (get_bit(board.pieces[BLACK_ROOK], square)) piece = 'r';
            else if (get_bit(board.pieces[BLACK_QUEEN], square)) piece = 'q';
            else if (get_bit(board.pieces[BLACK_KING], square)) piece = 'k';
            cout << piece << " ";
        }
        cout << "\n";
    }
    cout << "\n  a b c d e f g h\n\n";
}
uint64_t get_white_pieces(Board &board){
    return board.pieces[WHITE_PAWN] | board.pieces[WHITE_KNIGHT] | board.pieces[WHITE_BISHOP] | board.pieces[WHITE_ROOK] | board.pieces[WHITE_QUEEN] | board.pieces[WHITE_KING];
}
uint64_t get_black_pieces(Board &board){
    return board.pieces[BLACK_PAWN] | board.pieces[BLACK_KNIGHT] | board.pieces[BLACK_BISHOP] | board.pieces[BLACK_ROOK] | board.pieces[BLACK_QUEEN] | board.pieces[BLACK_KING];
}
uint64_t get_all_pieces(Board &board){
    return get_white_pieces(board) | get_black_pieces(board);
}
uint64_t get_empty_squares(Board &board){
    return ~get_all_pieces(board);
}
int get_piece_on_square(Board &board,int square){
    for (int i=0;i<PIECE_COUNT;i++){
        if(get_bit(board.pieces[i],square)) return i;
    }
    return -1;
}
bool is_square_attacked(Board &board, int square, bool by_white) {
    // Check pawn attacks
    uint64_t not_a_file = 0xFEFEFEFEFEFEFEFEULL;
    uint64_t not_h_file = 0x7F7F7F7F7F7F7F7FULL;
    uint64_t sq = 1ULL << square;
    
    if (by_white) {
        uint64_t pawn_attacks = ((sq >> 7) & not_a_file) | ((sq >> 9) & not_h_file);
        if (pawn_attacks & board.pieces[WHITE_PAWN]) return true;
    } else {
        uint64_t pawn_attacks = ((sq << 7) & not_h_file) | ((sq << 9) & not_a_file);
        if (pawn_attacks & board.pieces[BLACK_PAWN]) return true;
    }
    // Check knight attacks
    uint64_t not_b_file = 0xFDFDFDFDFDFDFDFDULL;
    uint64_t not_g_file = 0xBFBFBFBFBFBFBFBFULL;
    uint64_t not_ab_file = not_a_file & not_b_file;
    uint64_t not_gh_file = not_g_file & not_h_file;
    
    uint64_t knight_attacks =
        ((sq << 17) & not_a_file)  |
        ((sq << 15) & not_h_file)  |
        ((sq << 10) & not_ab_file) |
        ((sq << 6)  & not_gh_file) |
        ((sq >> 17) & not_h_file)  |
        ((sq >> 15) & not_a_file)  |
        ((sq >> 10) & not_gh_file) |
        ((sq >> 6)  & not_ab_file);
    
    uint64_t knights = by_white ? board.pieces[WHITE_KNIGHT] : board.pieces[BLACK_KNIGHT];
    if (knight_attacks & knights) return true;

    // Check king attacks
    uint64_t king_attacks =
        ((sq << 8)) | ((sq >> 8)) |
        ((sq << 1) & not_a_file) | ((sq >> 1) & not_h_file) |
        ((sq << 9) & not_a_file) | ((sq << 7) & not_h_file) |
        ((sq >> 7) & not_a_file) | ((sq >> 9) & not_h_file);
    
    uint64_t king = by_white ? board.pieces[WHITE_KING] : board.pieces[BLACK_KING];
    if (king_attacks & king) return true;
    // Check bishop/queen diagonal attacks
    uint64_t bishops_queens = by_white ? 
        (board.pieces[WHITE_BISHOP] | board.pieces[WHITE_QUEEN]) :
        (board.pieces[BLACK_BISHOP] | board.pieces[BLACK_QUEEN]);
    
    // Cast rays in 4 diagonal directions
    int diag_steps[] = {9, 7, -7, -9};
    uint64_t diag_masks[] = {not_a_file, not_h_file, not_a_file, not_h_file};
    
    for (int i = 0; i < 4; i++) {
        int to = square + diag_steps[i];
        uint64_t ray = 1ULL << to;
        while (to >= 0 && to < 64 && (ray & diag_masks[i])) {
            if (ray & bishops_queens) return true;
            if (ray & get_all_pieces(board)) break;
            to += diag_steps[i];
            ray = 1ULL << to;
        }
    }

    // Check rook/queen straight attacks
    uint64_t rooks_queens = by_white ?
        (board.pieces[WHITE_ROOK] | board.pieces[WHITE_QUEEN]) :
        (board.pieces[BLACK_ROOK] | board.pieces[BLACK_QUEEN]);
    
    int straight_steps[] = {8, -8, 1, -1};
    uint64_t straight_masks[] = {
        0xFFFFFFFFFFFFFFFFULL,
        0xFFFFFFFFFFFFFFFFULL,
        not_h_file,
        not_a_file
    };
    
    for (int i = 0; i < 4; i++) {
        int to = square + straight_steps[i];
        uint64_t ray = 1ULL << to;
        while (to >= 0 && to < 64 && (ray & straight_masks[i])) {
            if (ray & rooks_queens) return true;
            if (ray & get_all_pieces(board)) break;
            to += straight_steps[i];
            ray = 1ULL << to;
        }
    }

    return false;
}
bool is_in_check(Board &board, bool white_king) {
    int king_square = __builtin_ctzll(white_king ? board.pieces[WHITE_KING] : board.pieces[BLACK_KING]);
    return is_square_attacked(board, king_square, !white_king);
}