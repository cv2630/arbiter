#include "uci.h"
#include "search.h"
#include "chess.hpp"
#include <iostream>
#include <sstream>
#include <vector>
using namespace chess;

namespace {
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
    std::string black = "\e[37m\e[40m";
    std::string white = "\e[30m\e[47m";
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
/*
note: the uci command parsing in uci.cpp follows the standard 
token-based approach for this protocol (istringstream + dispatch on first word)
*/

void uciLoop() {
    chess::Board board;
    std::string line;

    while (std::getline(std::cin, line)) {
        std::istringstream stream(line);
        std::string token;
        stream >> token;

        if(token == "uci") {
            std::cout << "id name SimpleChessEngine\n";
            std::cout << "id author Me\n";
            std::cout << "uciok\n";
        } 
        else if(token == "isready") {
            std::cout << "readyok\n";
        } 
        else if(token == "position") {
            std::string subtoken;
            stream >> subtoken;
            if(subtoken == "startpos") {
                board.setFen(constants::STARTPOS);
                if(stream >> subtoken && subtoken == "moves") {
                    std::string moveStr;
                    while (stream >> moveStr) {
                        chess::Move move = chess::uci::uciToMove(board, moveStr);
                        board.makeMove(move);
                    }
                }
            } 
            else if(subtoken == "fen") {
                std::string fenParts;
                // Accumulate FEN string (6 fields)
                std::string fen = "";
                for (int i = 0; i < 6; ++i) {
                    stream >> subtoken;
                    fen += subtoken + (i < 5 ? " " : "");
                }
                board.setFen(fen);
                if(stream >> subtoken && subtoken == "moves") {
                    std::string moveStr;
                    while (stream >> moveStr) {
                        chess::Move move = chess::uci::uciToMove(board, moveStr);
                        board.makeMove(move);
                    }
                }
            }
        } 
        else if(token == "go") {
            SearchLimits limits;
            std::string sub;
            while (stream >> sub) {
                if (sub == "depth") stream >> limits.depth;
                else if (sub == "movetime") stream >> limits.movetime;
                else if (sub == "wtime") stream >> limits.wtime;
                else if (sub == "btime") stream >> limits.btime;
                else if (sub == "winc") stream >> limits.winc;
                else if (sub == "binc") stream >> limits.binc;
                else if (sub == "infinite") limits.infinite = true;
            }
            Move best = findBestMove(board, limits);
            std::cout << "bestmove " << uci::moveToUci(best) << "\n";
        }
        else if (token == "quit") {
            break;
        }
    }
}

void playLoop() {
    Board board;
    board.setFen(constants::STARTPOS);

    while(true) {
        printBoard(board);

        Movelist legal;
        movegen::legalmoves(legal, board);

        if(legal.empty()) {
            if(board.inCheck()) {
                std::cout<<(board.sideToMove() == Color::WHITE ? "Black" : "White")
                    << " wins by checkmate!\n";
            }
            else {
             std::cout << "Draw by stalemate.\n";
            }
            break;
        }

        if(board.sideToMove() == Color::WHITE) {
            std::cout << "Your move (e.g. e2e4), or 'quit': ";
            std::string input;
            if(!(std::cin >> input) || input == "quit"){
                break;
            }

            Move chosen= Move::NO_MOVE;
            for (const auto& m:legal) {
                if(uci::moveToUci(m) == input) {
                    chosen = m;
                    break;
                }
            }
            if(chosen == Move::NO_MOVE) {
                std::cout<<"Not a legal move, try again.\n";
                continue;
            }
            board.makeMove(chosen);
        } 
        else {
            std::cout<<"Engine thinking...\n";
            SearchLimits limits;
            limits.depth=5;
            Move best = findBestMove(board, limits);
            std::cout<<"Engine plays: "<<uci::moveToUci(best)<<"\n";
            board.makeMove(best);
        }
    }
}