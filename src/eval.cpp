#include "eval.h"

using namespace chess;

namespace {

constexpr int PAWN_VAL   = 100;
constexpr int KNIGHT_VAL = 320;
constexpr int BISHOP_VAL = 330;
constexpr int ROOK_VAL   = 500;
constexpr int QUEEN_VAL  = 900;

int pieceValue(PieceType pt) {
    int values[] = {PAWN_VAL, KNIGHT_VAL, BISHOP_VAL, ROOK_VAL, QUEEN_VAL, 0}; //king not caputred
    return values[static_cast<int>(pt.internal())];
}

/* class PieceType {
   public:
    enum class underlying : std::uint8_t {
        PAWN,
        KNIGHT,
        BISHOP,
        ROOK,
        QUEEN,
        KING,
        NONE,
    }; //from chess.hpp
    */

//assign diff values based on stage?
// SEE FOLLOWING
// https://chessprogramming.org/Simplified_Evaluation_Function


int KNIGHT_PST[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50,
};

int PAWN_PST[64] = {
      0,  0,  0,  0,  0,  0,  0,  0, 
     50, 50, 50, 50, 50, 50, 50, 50,
     10, 10, 20, 30, 30, 20, 10, 10,
      5,  5, 10, 25, 25, 10,  5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5, -5,-10,  0,  0,-10, -5,  5,
      5, 10, 10,-20,-20, 10, 10,  5,
      0,  0,  0,  0,  0,  0,  0,  0
};

int BISHOP_PST[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};


int ROOK_PST[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10, 10, 10, 10, 10,  5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
      0,  0,  0,  5,  5,  0,  0,  0  //castle
};

int QUEEN_PST[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

int KING_PST[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20  //castle?
};

int KING_ENDGAME_PST[64] = {
    -50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
};


//skeleton of fns needed
//point counter
int material(const Board& board, Color side) {
    static const PieceType types[] = {
        PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP,
        PieceType::ROOK, PieceType::QUEEN
    };

    //evaluation part 1
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total+=board.pieces(types[i], side).count()*pieceValue(types[i]);
    }
    return total;
}

//each peice's position on board + is it well placed, scale wrt pt value
int pawnPlacement(const Board& board, Color side) {
    int total = 0;
    Bitboard knights = board.pieces(PieceType::PAWN, side);
    while(knights) {
        int sq = knights.pop();
        int idx = sq;
        if(side==Color::BLACK) {
            idx = sq ^ 56;
        }
        total += PAWN_PST[idx];
    }
    return total;
}

int knightPlacement(const Board& board, Color side) {
    int total = 0;
    Bitboard knights = board.pieces(PieceType::KNIGHT, side);
    while(knights) {
        int sq = knights.pop();
        int idx = sq;
        if(side==Color::BLACK) {
            idx = sq ^ 56;
        }
        total += KNIGHT_PST[idx];
    }
    return total;
}

int bishopPlacement(const Board& board, Color side) {
    int total = 0;
    Bitboard knights = board.pieces(PieceType::BISHOP, side);
    while(knights) {
        int sq = knights.pop();
        int idx = sq;
        if(side==Color::BLACK) {
            idx = sq ^ 56;
        }
        total += BISHOP_PST[idx];
    }
    return total;
}

int rookPlacement(const Board& board, Color side) {
    int total = 0;
    Bitboard knights = board.pieces(PieceType::ROOK, side);
    while(knights) {
        int sq = knights.pop();
        int idx = sq;
        if(side==Color::BLACK) {
            idx = sq ^ 56;
        }
        total += ROOK_PST[idx];
    }
    return total;
}

int queenPlacement(const Board& board, Color side) {
    int total = 0;
    Bitboard knights = board.pieces(PieceType::QUEEN, side);
    while(knights) {
        int sq = knights.pop();
        int idx = sq;
        if(side == Color::BLACK) {
            idx = sq ^ 56;
        }
        total += QUEEN_PST[idx];
    }
    return total;
}
}

int evaluate(const Board& board) {
    Color us = board.sideToMove();
    Color them = ~us;
    int score = material(board, us)-material(board, them);

        score += pawnPlacement(board, us)-pawnPlacement(board, them); //repeat for each
    score += knightPlacement(board, us)-knightPlacement(board, them); //repeat for each
    score += bishopPlacement(board, us)-bishopPlacement(board, them); //repeat for each
    score += rookPlacement(board, us)-rookPlacement(board, them); //repeat for each
    score += queenPlacement(board, us)-queenPlacement(board, them);
    //add king midgame/endgame placement scores after rest is written

    return score;
}