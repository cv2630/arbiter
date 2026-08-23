#include "eval.h"

using namespace chess;

namespace {

constexpr int PAWN_VAL   = 100;
constexpr int KNIGHT_VAL = 320;
constexpr int BISHOP_VAL = 330;
constexpr int ROOK_VAL   = 500;
constexpr int QUEEN_VAL  = 900;

int pieceValue(PieceType pt) {
    switch (pt.internal()) {
        case PieceType::PAWN:   return PAWN_VAL;
        case PieceType::KNIGHT: return KNIGHT_VAL;
        case PieceType::BISHOP: return BISHOP_VAL;
        case PieceType::ROOK:   return ROOK_VAL;
        case PieceType::QUEEN:  return QUEEN_VAL;
        default:                return 0; // king
    }
}
//add point values next
}

int evaluate(const Board& board) {
    Color us = board.sideToMove();
    Color them = ~us;

    int score = material(board, us) - material(board, them);
    score += knightPlacement(board, us) - knightPlacement(board, them);

    return score;
}