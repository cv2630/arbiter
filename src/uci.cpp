#include "uci.h"
#include "search.h"
#include "chess.hpp"
using namespace chess;

namespace {

//add

    std::string pieceSymbol(Piece p) {
    switch (p.internal()) {
        case Piece::WHITEPAWN:   return "♙";
        case Piece::WHITEKNIGHT: return "♘";
        case Piece::WHITEBISHOP: return "♗";
        case Piece::WHITEROOK:   return "♖";
        case Piece::WHITEQUEEN:  return "♕";
        case Piece::WHITEKING:   return "♔";
        case Piece::BLACKPAWN:   return "♟";
        case Piece::BLACKKNIGHT: return "♞";
        case Piece::BLACKBISHOP: return "♝";
        case Piece::BLACKROOK:   return "♜";
        case Piece::BLACKQUEEN:  return "♛";
        case Piece::BLACKKING:   return "♚";
        default:                 return " ";
    } //decide: shld i reverse?
}

void printBoard(const Board& board) {
    /*
    disservin writes pieces w following idx
    index = file + 8 * rank
    */

    //google ```echo -e "\e[30;43m This text has a yellow background \e[0m"```

    std::string black = "\e[37m\e[40m";
    std::string white = "\e[37m\e[47m";
    std::string reset= "\e[0m";
for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
        int sq = (7-row)*8 +col; //chess.hpp sq idx, rank * 8 + file
        Piece p= board.at(Square(sq));

        std::string bg;
        //rank a and file 1 is the dark color BLACK
        // this corresponds to row = 7, col = 0
        if ((row + col)%2 == 1) { //0+7
            bg = black;
        } 
        else {
            bg = white;
        }
        std::cout << bg << " " << pieceSymbol(p) << " " << reset;
    }
    std::cout << "\n";
} }


}

void uciLoop() {
}

/*int main()
{
    std::cout<<"Hello World";
    printBoard();

    return 0;
*/