#include <iostream>
#include "board.h"
#include "movegen.h"
using namespace std;

int main(){
Board board;
init_board(board);
cout<<"White Pawns: "<<board.pieces[WHITE_PAWN]<<endl;
cout<<"Black Pawns: "<<board.pieces[BLACK_PAWN]<<endl;
cout<<"White to move: "<<board.white_to_move<<endl;
print_board(board);
uint64_t white = get_white_pieces(board);
uint64_t black = get_black_pieces(board);
uint64_t all = get_all_pieces(board);
uint64_t empty = get_empty_squares(board);

cout << "White pieces: " << white << endl;
cout << "Black pieces: " << black << endl;
cout << "All pieces: " << all << endl;
cout << "Empty squares: " << empty << endl;
vector<Move> moves;
generate_pawn_moves(board, moves);
cout<<"Total pawn moves from start :"<<moves.size()<<endl;
moves.clear();
generate_knight_moves(board, moves);
cout<<"Total knight moves from start :"<<moves.size()<<endl;
moves.clear();
generate_bishop_moves(board, moves);
std::cout << "Bishop moves from start: " << moves.size() << std::endl;
moves.clear();
generate_rook_moves(board, moves);
std::cout << "Rook moves from start: " << moves.size() << std::endl;
moves.clear();  
generate_queen_moves(board, moves);
std::cout << "Queen moves from start: " << moves.size() << std::endl;
moves.clear();
generate_king_moves(board, moves);
std::cout << "King moves from start: " << moves.size() << std::endl;
moves.clear();
generate_all_moves(board, moves);
std::cout << "Total moves from start: " << moves.size() << std::endl;
std::cout << "White in check: " << is_in_check(board, true) << std::endl;
std::cout << "Black in check: " << is_in_check(board, false) << std::endl;
moves.clear();
generate_legal_moves(board, moves);
std::cout << "Legal moves from start: " << moves.size() << std::endl;

return 0;
}
