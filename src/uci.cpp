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
    }
}

void printBoard(const Board& board) {
    std::string black = "\e[37m\e[40m";
    std::string white = "\e[37m\e[47m";
    std::string reset= "\033[0m";

}


}

void uciLoop() {
}