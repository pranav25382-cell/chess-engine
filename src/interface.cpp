#include "interface.h"
#include <iostream>
#include <string>
#include "board.h"
#include "search.h"
#include "movegen.h"

int square_from_string(const std::string &s) {
    int file = s[0] - 'a';
    int rank = s[1] - '1';
    return rank * 8 + file;
}

std::string square_to_string(int square) {
    std::string s = "";
    s += (char)('a' + square % 8);
    s += (char)('1' + square / 8);
    return s;
}

void play_game(Board &board, int depth) {
    while (true) {
        print_board(board);
        
        if (is_checkmate(board, board.white_to_move)) {
            std::cout << (board.white_to_move ? "Black" : "White") << " wins by checkmate!\n";
            break;
        }
        if (is_stalemate(board, board.white_to_move)) {
            std::cout << "Stalemate — draw!\n";
            break;
        }
        
        if (board.white_to_move) {
            // Human turn
            std::string input;
            std::cout << "Your move (e.g. e2e4): ";
            std::cin >> input;
            
            int from = square_from_string(input.substr(0, 2));
            int to = square_from_string(input.substr(2, 2));
            
            // Find matching legal move
            std::vector<Move> moves;
            generate_legal_moves(board, moves);
            bool found = false;
            for (Move &m : moves) {
                if (m.from == from && m.to == to) {
                    int saved_ep;
                    make_move(board, m, saved_ep);
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Illegal move, try again.\n";
            }
        } else {
            // Engine turn
            std::cout << "Engine thinking...\n";
            Move best;
            get_best_move(board, depth, best);
            std::cout << "Engine plays: " << square_to_string(best.from) 
                      << square_to_string(best.to) << "\n";
            int saved_ep;
            make_move(board, best, saved_ep);
        }
    }
}