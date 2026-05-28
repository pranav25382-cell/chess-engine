#include <iostream>
#include "board.h"
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
    return 0;
}
