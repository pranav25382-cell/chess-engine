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
    return 0;
}
