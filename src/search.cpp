#include "search.h"
#include <climits>

// Piece values in centipawns
const int PIECE_VALUES[PIECE_COUNT] = {
    100, 320, 330, 500, 900, 20000,   // white
    100, 320, 330, 500, 900, 20000    // black
};

int evaluate(Board &board) {
    int score = 0;
    for (int i = 0; i < PIECE_COUNT; i++) {
        uint64_t pieces = board.pieces[i];
        while (pieces) {
            if (i < 6) score += PIECE_VALUES[i];
            else score -= PIECE_VALUES[i];
            pieces &= pieces - 1;
        }
    }
    return board.white_to_move ? score : -score;
}



int alphaBeta(Board &board, int depth, int alpha, int beta) {
    if (depth == 0) return evaluate(board);
    
    std::vector<Move> moves;
    generate_legal_moves(board, moves);
    
    if (moves.empty()) {
        if (is_in_check(board, board.white_to_move)) return -100000;
        return 0;
    }
    
    for (Move &m : moves) {
        int saved_ep;
        make_move(board, m, saved_ep);
        int score = -alphaBeta(board, depth - 1, -beta, -alpha);
        unmake_move(board, m, saved_ep);
        
        if (score >= beta) return beta;  // beta cutoff
        if (score > alpha) alpha = score;
    }
    return alpha;
}
int get_best_move(Board &board, int depth, Move &best_move) {
    std::vector<Move> moves;
    generate_legal_moves(board, moves);
    
    int best_score = -100001;
    for (Move &m : moves) {
        int saved_ep;
        make_move(board, m, saved_ep);
        int score = -alphaBeta(board, depth - 1, -100001, -best_score);
        unmake_move(board, m, saved_ep);
        if (score > best_score) {
            best_score = score;
            best_move = m;
        }
    }
    return best_score;
}